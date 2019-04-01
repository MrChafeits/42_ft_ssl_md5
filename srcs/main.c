/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/03/29 22:19:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_md5.h>

void	usage(char *bin, char *cmd, int opt)
{
	if (opt)
	{
		ft_printf("usage: %s command [command opts] [command args]\n", bin);
	}
	else
	{
		ft_printf("%s: Error: \'%s\' is an invalid command.\n\n", bin, cmd);
		ft_printf("Standard commands:\n\n");
		ft_printf("Message Digest commands:\nmd5\nsha1\nsha224\nsha256\nsha384\nsha512\n\n");
		ft_printf("Cipher commands:\n\n");
	}
}
# define MD5_DGST_LEN (16)
# define BUFSIZE (1024 * 16)
static inline void	md5_print(t_u8 *md)
{
	register int i;

	i = -1;
	while (++i < MD5_DGST_LEN)
		ft_printf("%02x", md[i]);
	ft_printf("\n");
}

void				ft_dofd(int fd)
{
	t_md5_ctx	c;
	t_u8		md[MD5_DGST_LEN];
	int			i;
	static t_u8	buf[BUFSIZE];

	md5_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break ;
		md5_update(&c, (t_u8*)(&buf), i);
	}
	md5_final(&c, md);
	md5_print(md);
}

int					ft_md5_fileargs(int ac, char **av)
{
	int			i;
	int			err;
	int			fd;
	struct stat	st;

	i = 0;
	err = 0;
	while (++i < ac)
	{
		fd = open(av[i], O_RDONLY);
		if (lstat(av[i], &st) || fd < 0 || !S_ISREG(st.st_mode))
		{
			if (!S_ISREG(st.st_mode))
				errno = EISDIR;
			perror(av[i]);
			err++;
			continue ;
		}
		ft_printf("MD5(%s)= ", av[i]);
		ft_dofd(fd);
		close(fd);
	}
	return (err);
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl github
** awindham github awin ft_ssl_md5
*/
void				ft_md5_process(int ac, char **av)
{
	if (ac == 1)
		ft_dofd(STDIN_FILENO);
	else
		exit(ft_md5_fileargs(ac, av));
}

int		main(int ac, char **av)
{
	char	*msg;
	int		b;
	char	*ln;
	size_t	len;

	if (ac == 1)
	{
		ft_printf("ft_ssl> ");
		while ((b = get_next_line(0, &ln)) > 0)
		{
			if (!ft_strncmp("md5 ", ln, 4))
				ft_md5_process(1, (ln));
			else
				usage(av[0], av[1], 0);
			free(ln);
			ft_printf("ft_ssl> ");
		}
	}
	if (ac >= 2)
	{
		if (!ft_strcmp("md5", av[1]))
			ft_md5_process(--ac, ++av);
		else
			usage(av[0], av[1], 1);
	}
	exit(0);
}
