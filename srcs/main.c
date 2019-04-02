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
#include <internal.h>
#include <libft.h>
#include <ft_md5.h>

static const void	(*g__[8])(int, char**) = {
	[INVAL] = ft_ssl_usage,
	[MD5] = ft_md5_process,
	[SHA1] = ft_sha1_process,
	[SHA224] = ft_sha224_process,
	[SHA256] = ft_sha256_process,
	[SHA384] = ft_sha384_process,
	[SHA512] = ft_sha512_process,
	[7] = ft_ssl_usage
};

void	ft_ssl_usage(int ac, char **av)
{
	if (ac == 1)
		ft_printf("usage: %s command [command opts] [command args]\n", av[0]);
	else
	{
		ft_printf("%s: Error: \'%s\' is an invalid command.\n\n", av[0], av[1]);
		ft_printf("Standard commands:\n\n");
		ft_printf("Message Digest commands:\n"
		"md5\nsha1\nsha224\nsha256\nsha384\nsha512\n\n");
		ft_printf("Cipher commands:\n\n");
	}
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl github
** awindham github awin ft_ssl_md5
*/

void	shell_prompt(void)
{
	char	*ln;
	char	**t;
	int		b;
	int		len;

	ft_printf("ft_ssl> ");
	while ((b = get_next_line(0, &ln)) > 0)
	{
		t = ft_strsplit(ln, ' ');
		len = len_strtab(t);
		free(ln);
		g__[get_command_(*t)](len, t+1);
		ft_free_strtab(&t);
		ft_printf("ft_ssl> ");
	}
}

int		main(int ac, char **av)
{
	if (ac == 1)
		shell_prompt();
	if (ac >= 2)
	{
		g__[get_command_(*av)](ac, av);
	}
	exit(0);
}
