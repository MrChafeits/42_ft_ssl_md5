/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:04:23 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <internal.h>
#include <libft.h>
#include <ft_md5.h>

static void	(*g__[8])(int, char**) = {
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
		ft_printf("Message Digest commands:\n%-18s%-18s%-18s%-18s\n%-18s%-18s\n\n",
		"md5", "sha1", "sha224", "sha256", "sha384", "sha512");
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
		g__[get_command_(t[0])](len, t);
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
		g__[get_command_(av[1])](ac, av);
	}
	exit(0);
}
