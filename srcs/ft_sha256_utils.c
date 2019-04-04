/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha256_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:06:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:06:29 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <ft_ssl.h>
#include <ft_sha256.h>
#include <unistd.h>
#include <fcntl.h>

#define DGSTLEN (SHA256_DIGEST_SIZE)
#define BUFSIZE (1024 * DGSTLEN)

static inline void	ft_sha256_print(t_u8 *md)
{
	register int i;

	i = -1;
	while (++i < DGSTLEN)
		ft_printf("%02x", md[i]);
	ft_printf("\n");
}

static void			ft_dofd(int fd)
{
	t_sha256	c;
	t_u8		md[DGSTLEN];
	int			i;
	static t_u8	buf[BUFSIZE];

	ft_sha256_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break ;
		ft_sha256_update(&c, (t_u8*)(&buf), i);
	}
	ft_sha256_final(&c, md);
	ft_sha256_print(md);
}

static int			ft_sha256_fileargs(t_hash *h)
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
		ft_printf("SHA256(%s)= ", h->av[i]);
		ft_dofd(fd);
		close(fd);
	}
	return (err);
}

void				ft_sha256_process(t_hash *h)
{
	if (h->ac == 2)
		ft_dofd(STDIN_FILENO);
	else
	{
		h->ac--;
		h->av++;
		exit(ft_sha256_fileargs(h));
	}
}

#undef DGSTLEN
#undef BUFSIZE
