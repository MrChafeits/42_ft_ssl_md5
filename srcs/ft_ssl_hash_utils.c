/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 22:23:22 by callen            #+#    #+#             */
/*   Updated: 2019/04/12 18:06:40 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "internal.h"
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

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
	[INVAL] = "wot in ternation (%s) = ",
	[MD5] = "MD5 (%s) = ",
	[SHA1] = "SHA1 (%s) = ",
	[SHA224] = "SHA224 (%s) = ",
	[SHA256] = "SHA256 (%s) = ",
	[SHA384] = "SHA384 (%s) = ",
	[SHA512] = "SHA512 (%s) = ",
	[SHA512224] = "SHA512-224 (%s) = ",
	[SHA512256] = "SHA512-256 (%s) = ",
	NULL
};

static char	*g_strfx[] = {
	[INVAL] = "wot in ternation (\"%s\") = ",
	[MD5] = "MD5 (\"%s\") = ",
	[SHA1] = "SHA1 (\"%s\") = ",
	[SHA224] = "SHA224 (\"%s\") = ",
	[SHA256] = "SHA256 (\"%s\") = ",
	[SHA384] = "SHA384 (\"%s\") = ",
	[SHA512] = "SHA512 (\"%s\") = ",
	[SHA512224] = "SHA512-224 (\"%s\") = ",
	[SHA512256] = "SHA512-256 (\"%s\") = ",
	NULL
};

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
		while (1)
		{
			if((i[1] = read(h->cfd, buf, BUFSIZE)) <= 0)
			{
				close(h->cfd);
				break;
			}
			h->update(g_ctx[h->id.x], (t_u8*)(&buf), i[1]);
		}
		h->final(g_ctx[h->id.x], (t_u8*)(&md));
		ft_printf(g_pfx[h->id.x], h->cpth);
		if (!cmp_hash_str(h, h->t[0], (t_u8*)(md)))
			ft_printf("OK\n");
		else
			ft_printf("FAILED\n");
		free(h->cpth);
		ft_free_strtab(&h->t);
		ft_bzero(&g_ctxx,sizeof(t_ctx));
	}
}

void	hash_print(t_hash *h, int fd)
{
	int				i;
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
	if (!h->bsd && !h->quiet && fd != STDIN_FILENO)
		ft_printf(g_pfx[h->id.x], h->path);
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

int		hash_digest_files(t_hash *h)
{
	int	i;
	int	fd;

	i = optind;
	h->err = 0;
	while (++i < h->ac)
	{
		fd = open(h->av[i], O_RDONLY);
		if (lstat(h->av[i], &h->st) || fd < 0 || CHK(h->st.st_mode))
		{
			errno = S_ISDIR(h->st.st_mode) ? EISDIR : errno;
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
	int				i;
	t_u8			md[DGSTLEN];
	static t_u8		buf[BUFSIZE];

	h->init(g_ctx[h->id.x]);
	ft_strncpy((char*)(&buf), h->strarg, BUFSIZE);
	h->update(g_ctx[h->id.x], (t_u8*)(&buf), ft_strlen((char*)(&buf)));
	h->final(g_ctx[h->id.x], (t_u8*)(&md));
	if (!h->bsd && !h->quiet)
		ft_printf(g_strfx[h->id.x], h->strarg);
	i = -1;
	while (++i < h->dgst_len)
		ft_printf("%02x", md[i]);
	(h->bsd && !h->quiet) ? ft_printf(" \"%s\"\n", h->strarg) :
		ft_putchar('\n');
	ft_bzero(&g_ctxx, sizeof(t_ctx));
}

void	hash_process(t_hash *h)
{
	int status;

	status = 0;
	if (h->ac <= optind || h->echo || !h->shell)
		hash_print(h, STDIN_FILENO);
	if (h->string)
		hash_string(h);
	if (h->ac - 1 > optind)
		status = hash_digest_files(h);
	!h->shell ? exit(status) : 0;
}
