/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/04/07 21:17:57 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal.h>
#include "libft.h"

void	ft_ssl_command_usage(t_hash *h)
{
	if (h->ac <= 2 && !h->digest)
	{
		ft_dprintf(2, "Invalid command \'%s\'; type \"help\" for a list.\n",
		h->av[1]);
	}
	else if (h->digest)
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[0], h->av[optind]);
		ft_dprintf(2, "%s: Use -h for summary.\n", h->av[0]);
	}
	else
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[1], h->av[optind]);
		ft_dprintf(2, "%s: Use -h for summary.\n", h->av[1]);
	}
	exit(EXIT_FAILURE);
}

void	init_hash(t_hash *h)
{
	int		c;

	if (h->id.y != 1)
		h->id = get_command_(h, h->av[1]);
	while (h->ac > 2 && (c = ft_getopt(h->ac - 1, h->av + 1, "chpqrs:")) != -1)
		doopt(h, c);
	h->shell = (optind == 1 && h->ac < 3 && h->id.x && !h->string) ? 1 : 0;
	h->init = g_init[h->id.x];
	h->update = g_update[h->id.x];
	h->final = g_final[h->id.x];
	h->dgst_len = g_dgst_size[h->id.x];
	hash_process(h);
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl github
*/

static void	(*g_process[])(t_hash*) = {
	std_process,
	init_hash,
	std_process,
	NULL
};

void	shell_prompt(t_hash *h)
{
	char	*ln;
	int		b;

	h->shell = 1;
	ft_printf("ft_ssl> ");
	while ((b = get_next_line(0, &ln)) > 0)
	{
		h->av = ft_strsplit(ln, ' ');
		h->ac = len_strtab(h->av);
		free(ln);
		h->id = get_command_(h, h->av[0]);
		g_process[h->id.y](h);
		ft_free_strtab(&h->av);
		ft_printf("ft_ssl> ");
	}
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
		hash.id = get_command_(&hash, hash.av[1]);
		g_process[hash.id.y](&hash);
	}
	return (0);
}

#undef STRMODE
