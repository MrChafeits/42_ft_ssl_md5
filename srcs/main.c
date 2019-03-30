/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/03/29 22:11:18 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

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
		ft_printf("Message Digest commands:\nopenssl\nfast\nslow\n\n");
		ft_printf("Cipher commands:\n\n");
	}
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl github
** awindham github awin ft_ssl_md5
*/

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
			if (!ft_strncmp("openssl ", ln, 8))
				ft_md5_openssl(1, &(ln+9));
			else if (!ft_strncmp("fast ", ln, 5))
				ft_fast_md5(ln+5, ft_strlen(ln+6));
			else if (!ft_strncmp("slow ", ln, 5))
				ft_slow_md5(ln+5, ft_strlen(ln+6));
			free(ln);
			ft_printf("ft_ssl> ");
		}
	}
	if (ac >= 2)
	{
		if (!ft_strcmp("openssl", av[1]))
			ft_md5_openssl(--ac, ++av);
		else if (ac == 3)
		{
			msg = av[2];
			len = ft_strlen(msg);
			if (!ft_strcmp("fast", av[1]))
				ft_fast_md5(msg, len);
			else if (!ft_strcmp("slow", av[1]))
				ft_slow_md5(msg, len);
		}
		else
			usage(av[0], av[1], 1);
	}
	exit(0);
}
