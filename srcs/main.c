/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/07/20 23:42:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"

void	ft_ssl_command_usage(t_hash *h)
{
	fprintf(stderr, "DEBUG: ac(%d) optind(%d) h->digest(%d)\n", h->ac, optind, h->digest);
	if (h->ac && h->ac <= 2 && !h->digest)
	{
		ft_dprintf(2, "Invalid command \'%s\'; type \"help\" for a list.\n",
		h->av[!h->shell ? 1 : 0]);
	}
	else if (h->digest)
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[0], h->av[optind]);
		ft_dprintf(2, "%s: Use -h for summary.\n", h->av[0]);
	}
	else if (h->ac >= 2 && optind < h->ac)
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[1], h->av[optind]);
		ft_dprintf(2, "%s: Use -h for summary.\n", h->av[1]);
	}
	!h->shell ? exit(EXIT_FAILURE) : 0;
}

void	init_hash(t_hash *h)
{
	int		c;

	if (h->id.y != 1)
		h->id = get_command_(h, h->av[1]);
	while (h->ac > 2 && (c = ft_getopt(h->ac - 1, h->av + 1, "chpqrs:")) != -1)
		doopt(h, c);
	h->shell = (optind == h->ac && h->ac < 3 && h->id.x && !h->string) ? 1 :
		h->shell;
	h->init = g_init[h->id.x];
	h->update = g_update[h->id.x];
	h->final = g_final[h->id.x];
	h->dgst_len = g_dgst_size[h->id.x];
	hash_process(h);
}

/*
** {"dgst", "-md5", "-h", 0}
** {"md5", "-h", 0}
** before: av = {"md5", "-h", 0};
** after:  av = {"dgst", "-md5", "-h", 0};
*/

void	hashcmd_to_dgst(t_hash *h)
{
	char	**nv;
	int		i;

	if (!(nv = ft_memalloc(sizeof(*nv) * (h->ac + 1))))
	{
		panic_(-3, "in main.c:58: malloc failure");
		return ;
	}
	nv[h->ac] = 0;
	nv[0] = ft_strdup("dgst");
	nv[1] = ft_strjoin("-", h->av[0]);
	i = 0;
	while (++i < h->ac)
		nv[i + 1] = ft_strdup(h->av[i]);
	ft_free_strtab(&h->av);
	h->av = nv;
	h->ac++;
	h->hc2dgst = 1;
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl on github
*/

static void	(*g_process[])(t_hash*) = {
	std_process,
	init_hash,
	std_process,
	NULL
};

void	shell_prompt(t_hash *h)
{
	ssize_t	linelen;
	size_t	lcap=0;
	char	*ln=0;

	while (1)
	{
		h->shell = 1;
		h->help = 0;
		ft_putstr("ft_ssl> ");
		/* if ((get_next_line(0, &ln)) > 0) */
		if ((linelen = getline(&ln, &lcap, stdin)) > 0)
		{
			if (linelen > 0 && ln[linelen - 1] == '\n')
				ln[linelen - 1] = 0;
			h->av = ft_strsplit(ln, ' ');
			h->ac = len_strtab(h->av);
			h->id = get_command_(h, h->av[0]);
			// if (h->id.y == 1)
			// 	hashcmd_to_dgst(h);
			if (g_process[h->id.y] != NULL)
				g_process[h->id.y](h);
			ft_free_strtab(&h->av);
		}
		else
			break ;
	}
	ln ? ft_memdel((void**)&ln) : 0;
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
