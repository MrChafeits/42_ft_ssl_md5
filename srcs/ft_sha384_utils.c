/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha384_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:06:53 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:06:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <ft_ssl.h>
#include <ft_sha512.h>
#include <unistd.h>
#include <fcntl.h>

#define DGSTLEN (SHA384_DIGEST_SIZE)
#define BUFSIZE (1024 * DGSTLEN)

void	ft_sha384_init(t_sha512 *s)
{
	s->len = 0;
	s->h[0] = 0xcbbb9d5dc1059ed8ULL;
	s->h[1] = 0x629a292a367cd507ULL;
	s->h[2] = 0x9159015a3070dd17ULL;
	s->h[3] = 0x152fecd8f70e5939ULL;
	s->h[4] = 0x67332667ffc00b31ULL;
	s->h[5] = 0x8eb44a8768581511ULL;
	s->h[6] = 0xdb0c2e0d64f98fa7ULL;
	s->h[7] = 0x47b5481dbefa4fa4ULL;
}

static inline void	ft_sha384_print(t_u8 *md)
{
	register int i;

	i = -1;
	while (++i < DGSTLEN)
		ft_printf("%02x", md[i]);
	ft_printf("\n");
}

static void			ft_dofd(int fd)
{
	t_sha512	c;
	t_u8		md[DGSTLEN];
	int			i;
	static t_u8	buf[BUFSIZE];

	ft_sha384_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break ;
		ft_sha512_update(&c, (t_u8*)(&buf), i);
	}
	ft_sha512_final(&c, md);
	ft_sha384_print(md);
}

static int			ft_sha384_fileargs(t_hash *h)
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
		ft_printf("SHA384(%s)= ", h->av[i]);
		ft_dofd(fd);
		close(fd);
	}
	return (err);
}

void				ft_sha384_process(t_hash *h)
{
	if (h->ac == 2)
		ft_dofd(STDIN_FILENO);
	else
	{
		h->ac--;
		h->av++;
		exit(ft_sha384_fileargs(h));
	}
}

#undef DGSTLEN
#undef BUFSIZE
