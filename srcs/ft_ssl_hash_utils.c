/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 22:23:22 by callen            #+#    #+#             */
/*   Updated: 2019/04/06 01:49:52 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <internal.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#ifdef DGSTLEN
# undef DGSTLEN
#endif
#ifdef BUFSIZE
# undef BUFSIZE
#endif
#define DGSTLEN (1024 / 8)
#define BUFSIZE (1024 * DGSTLEN)

t_md5_ctx	g_ctxmd5;
t_s1ctx		g_ctxsha1;
t_sha256	g_ctxsha256;
t_sha512	g_ctxsha512;
t_sha3_ctx	g_ctxsha3;
t_tiger_ctx	g_ctxtiger;
t_whrl_ctx	g_ctxwhrl;
void	*g_ctx[] = {
	[INVAL] = (&g_ctxmd5),
	[MD5] = (&g_ctxmd5),
	[SHA1] = (&g_ctxsha1),
	[SHA224] = (&g_ctxsha256),
	[SHA256] = (&g_ctxsha256),
	[SHA384] = (&g_ctxsha512),
	[SHA512] = (&g_ctxsha512),
	[SHA512224] = (&g_ctxsha512),
	[SHA512256] = (&g_ctxsha512),
	[SHA3] = (&g_ctxsha3),
	[TIGER] = (&g_ctxtiger),
	[WHIRL] = (&g_ctxwhrl),
	NULL
};
static char	*g_pfx[] = {
	[INVAL] = "wot in ternation",
	[MD5] = "MD5(%s)= ",
	[SHA1] = "SHA1(%s)= ",
	[SHA224] = "SHA224(%s)= ",
	[SHA256] = "SHA256(%s)= ",
	[SHA384] = "SHA384(%s)= ",
	[SHA512] = "SHA512(%s)= ",
	[SHA512224] = "SHA512-224(%s)= ",
	[SHA512256] = "SHA512-256(%s)= ",
	[SHA3] = "SHA3-256(%s)= ",
	[TIGER] = "tiger(%s)= ",
	[WHIRL] = "whirlpool(%s)= ",
	NULL
};
static char	*g_strfx[] = {
	[INVAL] = "wot in ternation",
	[MD5] = "MD5(\"%s\")= ",
	[SHA1] = "SHA1(\"%s\")= ",
	[SHA224] = "SHA224(\"%s\")= ",
	[SHA256] = "SHA256(\"%s\")= ",
	[SHA384] = "SHA384(\"%s\")= ",
	[SHA512] = "SHA512(\"%s\")= ",
	[SHA512224] = "SHA512-224(\"%s\")= ",
	[SHA512256] = "SHA512-256(\"%s\")= ",
	[SHA3] = "SHA3-256(\"%s\")= ",
	[TIGER] = "tiger(\"%s\")= ",
	[WHIRL] = "whirlpool(\"%s\")= ",
	NULL
};

#ifdef STDINFD
# undef STDINFD
#endif
#ifdef CLOSEBR
# undef CLOSEBR
#endif
#ifdef READBF
# undef READBF
#endif
#ifdef TAILFREE
# undef TAILFREE
#endif
#ifdef CFDERROR
# undef CFDERROR
#endif
#ifdef HASNULLT
# undef HASNULLT
#endif
#ifdef ISOK
# undef ISOK
#endif
#define STDINFD (STDIN_FILENO)
#define CLOSEBR close(h->cfd); break;
#define READBF if((i[1] = read(h->cfd, buf, BUFSIZE)) <= 0){CLOSEBR}
#define TAILFREE free(h->cpth); ft_free_strtab(&h->t);
#define CFDERROR {perror(h->cpth); h->err++; free(h->cpth); continue;}
#define HASNULLT ((h->cfd = ft_strlen(h->cpth)) && h->cpth[h->cfd])
#define ISOK (!cmp_hash_str(h, h->t[0], (t_u8*)(&md)))

void	hash_digest_check(t_hash *h, int fd)
{
	int				i[2];
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->err = 0;
	while ((*i = get_next_line(fd, &h->cpth)) > 0)
	{
		h->init(g_ctx[h->id.x]);
		h->t = ft_strsplit(h->cpth, ' ');
		free(h->cpth);
		h->cpth = ft_strdup(h->t[1] + 1);
		HASNULLT ? h->cpth[h->cfd] = 0 : 0;
		h->cfd = open(h->cpth, O_RDONLY);
		if (lstat(h->cpth, &h->st) || h->cfd < 0 || !S_ISREG(h->st.st_mode))
		CFDERROR
		while (1)
		{
			READBF
			h->update(g_ctx[h->id.x], (t_u8*)(&buf), i);
		}
		h->final(g_ctx[h->id.x], (t_u8*)(&md));
		ft_printf(g_pfx[h->id.x], h->cpth);
		(ISOK) ? ft_printf("OK\n") : ft_printf("FAILED\n");
		TAILFREE
	}
}

void	hash_print(t_hash *h, int fd)
{
	register int	i;
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->init(g_ctx[h->id.x]);
	while (1)
	{
		if ((i = read(fd, buf, BUFSIZE)) <= 0)
			break ;
		h->update(g_ctx[h->id.x], (t_u8*)(&buf), i);
	}
	if (h->echo && fd == STDIN_FILENO)
		ft_printf("%s", buf);
	h->final(g_ctx[h->id.x], (t_u8*)(&md));
	if (!h->quiet && fd != STDIN_FILENO && !h->bsd)
		ft_printf(g_pfx[h->id.x], h->path);
	i = -1;
	while (++i < h->dgst_len)
		ft_printf("%02x", md[i]);
	(!h->quiet && fd != STDIN_FILENO && h->bsd) ? ft_printf(" *%s\n", h->path) :
		ft_putchar('\n');
}

int		hash_digest_files(t_hash *h)
{
	register int	i;
	register int	fd;
	struct stat		st;

	i = optind;
	h->err = 0;
	while (++i < h->ac)
	{
		fd = open(h->av[i], O_RDONLY);
		if (lstat(h->av[i], &st) || fd < 0 || !S_ISREG(st.st_mode))
		{
			perror(h->av[i]);
			h->err++;
			continue ;
		}
		h->path = h->av[i];
		h->check ? hash_digest_check(h, fd) : hash_print(h, fd);
		close(fd);
	}
	return (h->err);
}

void	hash_string(t_hash *h)
{
	register int	i;
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->init(g_ctx[h->id.x]);
	(void)ft_strncpy((char*)(&buf), h->strarg, BUFSIZE);
	h->update(g_ctx[h->id.x], (t_u8*)(&buf), ft_strlen((char*)(&buf)));
	h->final(g_ctx[h->id.x], (t_u8*)(&md));
	if (!h->quiet && !h->echo && !h->bsd)
		ft_printf(g_strfx[h->id.x], h->strarg);
	i = -1;
	while (++i < h->dgst_len)
		ft_printf("%02x", md[i]);
	(!h->quiet && !h->echo && h->bsd) ? ft_printf(" \"%s\"\n", h->strarg) :
		ft_putchar('\n');
}

void	hash_process(t_hash *h)
{
	if (h->shell || h->echo)
		hash_print(h, STDIN_FILENO);
	if (h->string)
		hash_string(h);
	exit(hash_digest_files(h));
}

#undef DGSTLEN
#undef BUFSIZE
#undef CTOI
#undef STOB
#undef ISXDIG
#undef STDINFD
#undef CLOSEBR
#undef TAILFREE
#undef CFDERROR
#undef HASNULLT
#undef ISOK
