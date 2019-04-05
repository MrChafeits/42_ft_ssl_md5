/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/04/04 13:20:05 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <internal.h>
#include "libft.h"

void	ft_ssl_help(t_hash *h)
{
	register int i;

	if (h->ac == 1)
		ft_dprintf(2, "usage: %s command [command opts] [command args]\n",
			h->av[0]);
	else
	{
		ft_dprintf(2, "%s: Error: \'%s\' is an invalid command.\n",
			h->av[0], h->av[1]);
		ft_dprintf(2, "\nStandard commands:\n");
		i = -1;
		while (g_s[++i])
			ft_dprintf(2, (i % 4 ? "%-18s\n" : "%-18s"), g_h[i]);
		ft_dprintf(2, "\nMessage Digest commands:\n");
		i = -1;
		while (g_h[++i])
			ft_dprintf(2, (i % 4 ? "%-18s\n" : "%-18s"), g_h[i]);
		ft_dprintf(2, "\nCipher commands:\n");
		i = -1;
		while (g_c[++i])
			ft_dprintf(2, (i % 4 ? "%-18s\n" : "%-18s"), g_c[i]);
		ft_putchar_fd('\n', 2);
	}
}

void	ft_ssl_command_usage(t_hash *h)
{
	if (h->ac <= 2)
		ft_dprintf(2, "Invalid command \'%s\'; type \"help\" for a list.\n",
			h->av[1]);
	else
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[1], h->av[optind]);
		ft_dprintf(2, "%s: Use -help for summary.\n", h->av[1]);
	}
	exit(EXIT_FAILURE);
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl github
*/

void	shell_prompt(t_hash *h)
{
	char	*ln;
	int		b;

	ft_printf("ft_ssl> ");
	while ((b = get_next_line(0, &ln)) > 0)
	{
		h->av = ft_strsplit(ln, ' ');
		h->ac = len_strtab(h->av);
		free(ln);
		h->id = get_command_(h->av[0]);
		hash_process(h);
		ft_free_strtab(&h->av);
		ft_printf("ft_ssl> ");
	}
}

void	init_hash(t_hash *h)
{
	int		ch;

	h->id = get_command_(h->av[1]);
	while ((ch = ft_getopt(h->ac, h->av + 1, "cpqrs")) != -1)
	{
		if (ch == 'c')
			h->check = 1;
		else if (ch == 'p')
			h->echo = 1;
		else if (ch == 'q')
			h->quiet = 1;
		else if (ch == 'r')
			h->bsd = 1;
		else if (ch == 's')
			h->string = 1;
		else
			ft_ssl_command_usage(h);
	}
	h->init = g_init[h->id];
	h->update = g_update[h->id];
	h->final = g_final[h->id];
	h->dgst_len = g_dgst_size[h->id];
}

int		main(int ac, char **av)
{
	static t_hash	hash;

	if (ac == 1)
		shell_prompt(&hash);
	if (ac >= 2)
	{
		hash.ac = ac;
		hash.av = av;
		init_hash(&hash);
		hash_process(&hash);
	}
	return (0);
}
