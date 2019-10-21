/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 22:23:22 by callen            #+#    #+#             */
/*   Updated: 2019/10/19 23:14:53 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "internal.h"
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
//NORME
#define DGSTLEN (512 / 8)
#define BUFSIZE (1024 * DGSTLEN)

static t_ctx	g_ctxx;
void	*g_ctx[] = {
	[INVAL] = (&g_ctxx.md5),
	[MD5] = (&g_ctxx.md5),
	[SHA1] = (&g_ctxx.sha1),
	[SHA224] = (&g_ctxx.sha2),
	[SHA256] = (&g_ctxx.sha2),
	[SHA384] = (&g_ctxx.sha512),
	[SHA512] = (&g_ctxx.sha512),
	[SHA512224] = (&g_ctxx.sha512),
	[SHA512256] = (&g_ctxx.sha512),
	NULL
};

static char	*g_pfx[] = {
	[INVAL] = "wot in ternation(%s)= ",
	[MD5] = "MD5(%s)= ",
	[SHA1] = "SHA1(%s)= ",
	[SHA224] = "SHA224(%s)= ",
	[SHA256] = "SHA256(%s)= ",
	[SHA384] = "SHA384(%s)= ",
	[SHA512] = "SHA512(%s)= ",
	[SHA512224] = "SHA512-224(%s)= ",
	[SHA512256] = "SHA512-256(%s)= ",
	NULL
};

static char	*g_strfx[] = {
	[INVAL] = "wot in hash calculation(\"%s\")= ",
	[MD5] = "MD5(\"%s\")= ",
	[SHA1] = "SHA1(\"%s\")= ",
	[SHA224] = "SHA224(\"%s\")= ",
	[SHA256] = "SHA256(\"%s\")= ",
	[SHA384] = "SHA384(\"%s\")= ",
	[SHA512] = "SHA512(\"%s\")= ",
	[SHA512224] = "SHA512-224(\"%s\")= ",
	[SHA512256] = "SHA512-256(\"%s\")= ",
	NULL
};

void	read_and_check(t_ssl_env *h)
{
	int br;

	while (1)
	{
		if((br = read(h->cfd, h->buf, BUFSIZE)) <= 0)
		{
			close(h->cfd);
			break;
		}
		h->update(g_ctx[h->digest], (t_u8*)&h->buf, br);
	}
	h->final(g_ctx[h->digest], (t_u8*)&h->md);
	ft_printf(g_pfx[h->digest], h->cpth);
	if (!cmp_hash_str(h, h->t[0], (t_u8*)&h->md))
		ft_printf("OK\n");
	else
		ft_printf("FAILED\n");
	free(h->cpth);
	ft_free_strtab(&h->t);
	ft_bzero(&g_ctxx, sizeof(t_ctx));
}

void	hash_digest_check(t_ssl_env *h, int fd)
{
	h->err = 0;
	while (get_next_line(fd, &h->cpth) > 0)
	{
		h->init(g_ctx[h->digest]);
		h->t = ft_strsplit(h->cpth, ' ');
		free(h->cpth);
		h->cpth = ft_strdup(h->t[1] + (*h->t[1] == '*' || *h->t[1] == '?'));
		if ((h->cfd = ft_strlen(h->cpth)) && h->cpth[h->cfd])
			h->cpth[h->cfd] = 0;
		h->cfd = open(h->cpth, O_RDONLY);
		if (lstat(h->cpth, &h->st) || h->cfd < 0 || !S_ISREG(h->st.st_mode))
		{
			perror(h->cpth);
			h->err++;
			free(h->cpth);
			continue;
		}
		read_and_check(h);
	}
}

void	hash_print(t_ssl_env *h, int fd)
{
	int				i;
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->init(g_ctx[h->digest]);
	while (1)
	{
		if ((i = read(fd, buf, BUFSIZE)) <= 0)
			break ;
		h->update(g_ctx[h->digest], (t_u8*)(&buf), i);
	}
	if (h->echo && fd == STDIN_FILENO)
		ft_printf("%s", buf);
	h->final(g_ctx[h->digest], (t_u8*)(&md));
	if (!h->bsd && !h->quiet && fd != STDIN_FILENO)
		ft_printf(g_pfx[h->digest], h->path);
	i = -1;
	while (++i < h->dgst_len)
		ft_printf("%02x", md[i]);
	if (h->bsd && !h->quiet && fd != STDIN_FILENO)
		ft_printf(" *%s\n", h->path);
	else
		ft_putchar('\n');
	ft_bzero(&g_ctxx, sizeof(t_ctx));
}

#define CHK(m) (!S_ISREG(m) && !S_ISCHR(m))

int		hash_digest_files(t_ssl_env *h)
{
	int	ii;
	int	fd;

	ii = -1;
	h->err = 0;
	while (++ii < h->ac)
	{
		fd = open(h->av[ii], O_RDONLY);
		if (lstat(h->av[ii], &h->st) || fd < 0 || CHK(h->st.st_mode))
		{
			errno = S_ISDIR(h->st.st_mode) ? EISDIR : errno;
			ft_dprintf(2, "%s: ", g_dgst_cmd[h->digest]);
			perror(h->av[ii]);
			h->err++;
			continue ;
		}
		h->path = h->av[ii];
		h->check ? hash_digest_check(h, fd) : hash_print(h, fd);
		close(fd);
	}
	return (h->err);
}

void	hash_string_arg(t_ssl_env *h)
{
	int				i;
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->init(g_ctx[h->digest]);
	ft_strncpy((char*)(&buf), h->strarg, BUFSIZE);
	h->update(g_ctx[h->digest], (t_u8*)(&buf), ft_strlen((char*)(&buf)));
	h->final(g_ctx[h->digest], (t_u8*)(&md));
	if (!h->bsd && !h->quiet)
		ft_printf(g_strfx[h->digest], h->strarg);
	i = -1;
	while (++i < h->dgst_len)
		ft_printf("%02x", md[i]);
	(h->bsd && !h->quiet) ? ft_printf(" \"%s\"\n", h->strarg) :
		ft_putchar('\n');
	ft_bzero(&g_ctxx, sizeof(t_ctx));
}
