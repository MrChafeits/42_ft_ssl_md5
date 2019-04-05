/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 22:23:22 by callen            #+#    #+#             */
/*   Updated: 2019/04/04 14:04:46 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <internal.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#define DGSTLEN (512)
#define BUFSIZE (1024 * DGSTLEN)

t_md5_ctx	g_ctxmd5;
t_s1ctx		g_ctxsha1;
t_sha256	g_ctxsha256;
t_sha512	g_ctxsha512;
t_sha3_ctx	g_ctxsha3;
t_tiger_ctx	g_ctxtiger;
t_whrl_ctx	g_ctxwhrl;
void	*g_ctx[] = {
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
};
static char	*g_pfx[] = {
	[MD5] = "MD5 (%s) = ",
	[SHA1] = "SHA1 (%s) = ",
	[SHA224] = "SHA224 (%s) = ",
	[SHA256] = "SHA256 (%s) = ",
	[SHA384] = "SHA384 (%s) = ",
	[SHA512] = "SHA512 (%s) = ",
	[SHA512224] = "SHA512224 (%s) = ",
	[SHA512256] = "SHA512256 (%s) = ",
	[SHA3] = "SHA3 (%s) = ",
	[TIGER] = "tiger (%s) = ",
	[WHIRL] = "whirlpool (%s) = ",
};

void	hash_print(t_hash *h, int fd)
{
	register int	i;
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->init(g_ctx[h->id]);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break ;
		h->update(g_ctx[h->id], (t_u8*)(&buf), i);
	}
	h->final(g_ctx[h->id], (t_u8*)(&md));
	if (!h->quiet && !h->echo && !h->bsd)
		ft_printf(g_pfx[h->id], h->path);
	i = -1;
	while (++i < h->dgst_len)
		ft_printf("%02x", md[i]);
	(!h->quiet && !h->echo && h->bsd) ? ft_printf("  %s\n", h->path) :
		ft_putchar('\n');
}

int		hash_digest(t_hash *h)
{
	register int	i;
	register int	err;
	register int	fd;
	struct stat		st;

	i = optind;
	err = 0;
	while (++i < h->ac)
	{
		fd = open(h->av[i], O_RDONLY);
		if (lstat(h->av[i], &st) || fd < 0 || !S_ISREG(st.st_mode))
		{
			perror(h->av[i]);
			err++;
			continue ;
		}
		h->path = h->av[i];
		hash_print(h, fd);
		close(fd);
	}
	return (err);
}

void	hash_process(t_hash *h)
{
	if (h->ac == 2)
		hash_digest(STDIN_FILENO);
	else
		exit(hash_digest(h));
}

#undef DGSTLEN
#undef BUFSIZE
