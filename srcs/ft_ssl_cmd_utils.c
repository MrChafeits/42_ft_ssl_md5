/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:47:57 by callen            #+#    #+#             */
/*   Updated: 2019/07/20 23:23:29 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"

/*
** Attempt to Mimic OpenSSL dgst command.
*/

static int	dgst_options(t_ssl_env *h)
{
	ft_ssl_getopt(h);
	if (h->help)
	{
		std_dgst_help(h);
		return (1);
	}
	h->init = g_init[h->id.x];
	h->update = g_update[h->id.x];
	h->final = g_final[h->id.x];
	h->dgst_len = g_dgst_size[h->id.x];
	hash_process(h);
	optreset = 1;
	return (0);
}

#define INCARG h->ac--; h->av++; h->av[1]++;

void	std_dgst(t_ssl_env *h)
{
	const char	**avbak = (const char**)h->av;
	const int	acbak = (const int)h->ac;
	// int			c;

	h->id.x = !h->id.x && (h->hc2dgst || h->ac == 1) ? SHA256 : h->id.x;
	h->id.y = 1;
	h->digest = 1;
	// md5 -s asdf
	// dgst -md5 -s asdf
	if (h->ac >= 3 && *h->av[2] == '-' && !ft_strequ("-help", h->av[2]))
	{
		h->ac -= !h->shell;
		h->av += !h->shell;
		h->av[1] += !h->shell;
		h->id = get_command_(h, h->av[1]);
	}
	else if (h->ac >= 3 && ft_strequ("-help", h->av[2]))
		std_dgst_help(h);
	if (!dgst_options(h) || h->help)
	{
		h->help = 0;
		return ;
	}
	// while (h->ac > 2 && (c = ft_getopt(h->ac - 1, h->av + 1, "chpqrs:")) != -1)
	// 	doopt(h, c);
	// optreset = 1;
	// h->help ? std_dgst_help(h) : 0;
	// h->shell = optind == 1 && h->ac < 3 && !h->string ? 1 : 0;
	// h->init = g_init[h->id.x];
	// h->update = g_update[h->id.x];
	// h->final = g_final[h->id.x];
	// h->dgst_len = g_dgst_size[h->id.x];
	// hash_process(h);
	h->ac = (int)acbak;
	h->av = (char**)avbak;
}

static void	(*g_std[])(t_ssl_env*) = {
	std_nyimpl,
	[DGST] = std_dgst,
	[HELP] = std_help,
	[VER] = std_version,
};

void	std_process(t_ssl_env *h)
{
	/* If id.y is a std_cmd */
	if (h->id.y == 0)
		g_std[h->id.x](h);
	if (h->std_cmd == DGST)
		g_std[DGST](h);
	else
		std_nyimpl(h);
}
