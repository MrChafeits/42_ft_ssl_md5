/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_md5_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:04:42 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:04:43 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <libft.h>
#include <ft_md5.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void				ft_md5_init(t_md5_ctx *c)
{
	c->count[0] = 0;
	c->count[1] = 0;
	c->state[0] = 0x67452301;
	c->state[1] = 0xEFCDAB89;
	c->state[2] = 0x98BADCFE;
	c->state[3] = 0x10325476;
}

#define DGSTLEN (128 / 8)
#define BUFSIZE (1024 * DGSTLEN)

static inline void	ft_md5_print(t_u8 *md)
{
	register int i;

	i = -1;
	while (++i < DGSTLEN)
		ft_printf("%02x", md[i]);
	ft_printf("\n");
}

static void			ft_dofd(int fd)
{
	t_md5_ctx	c;
	t_u8		md[DGSTLEN];
	int			i;
	static t_u8	buf[BUFSIZE];

	ft_md5_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break ;
		ft_md5_update(&c, (t_u8*)(&buf), i);
	}
	ft_md5_final(&c, md);
	ft_md5_print(md);
}

static int			ft_md5_fileargs(t_hash *h)
{
	int			i;
	int			err;
	int			fd;
	struct stat	st;

	i = 0;
	err = 0;
	while (++i < h->ac)
	{
		fd = open(h->av[i], O_RDONLY);
		if (lstat(h->av[i], &st) || fd < 0 || !S_ISREG(st.st_mode))
		{
			if (!S_ISREG(st.st_mode))
				errno = EISDIR;
			perror(h->av[i]);
			err++;
			continue ;
		}
		ft_printf("MD5(%s)= ", h->av[i]);
		ft_dofd(fd);
		close(fd);
	}
	return (err);
}

void				ft_md5_process(t_hash *h)
{
	if (h->ac == 2)
		ft_dofd(STDIN_FILENO);
	else
	{
		h->ac--;
		h->av++;
		exit(ft_md5_fileargs(h));
	}
}

#undef DGSTLEN
#undef BUFSIZE
