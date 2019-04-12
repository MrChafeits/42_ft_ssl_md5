/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:47:57 by callen            #+#    #+#             */
/*   Updated: 2019/04/08 13:47:58 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal.h>
#include <libft.h>

/*
** Mimic OpenSSL dgst command. Requires -* or -h to be first option
*/

#ifdef INCARG
# undef INCARG
#endif
#define INCARG h->ac--; h->av++; h->av[1]++;

void	std_dgst(t_hash *h)
{
	const char	**avbak = (const char**)h->av;
	const int	acbak = (const int)h->ac;
	int			c;

	h->id.x = SHA256;
	h->id.y = 1;
	h->digest = 1;
	if (h->ac >= 3 && *h->av[2] == '-' && !ft_strequ("-h", h->av[2]))
	{
		INCARG;
		h->id = get_command_(h, h->av[1]);
	}
	else if (ft_strequ("-h", h->av[2]))
		std_dgst_help(h);
	while (h->ac > 2 && (c = ft_getopt(h->ac - 1, h->av + 1, "chpqrs:")) != -1)
		doopt(h, c);
	h->help ? std_dgst_help(h) : 0;
	h->shell = (optind == 1 && h->ac < 3 && h->id.x && !h->string) ? 1 : 0;
	h->init = g_init[h->id.x];
	h->update = g_update[h->id.x];
	h->final = g_final[h->id.x];
	h->dgst_len = g_dgst_size[h->id.x];
	hash_process(h);
	h->ac = (int)acbak;
	h->av = (char**)avbak;
}

#ifdef FMTSTR
# undef FMTSTR
#endif
#define FMTSTR ((!((i + 1) % 4) && i) ? "%-18s\n" : "%-18s")

void	std_help(t_hash *h)
{
	register int i;

	if (h->ac == 1 && !h->shell)
		ft_dprintf(2, "usage: %s command [command opts] [command args]\n",
			h->av[0]);
	else
	{
		ft_dprintf(2, "Standard commands:\n");
		i = -1;
		while (g_s[++i])
			ft_dprintf(2, FMTSTR, g_s[i]);
		(i + 1) % 4 ? ft_printf("\n") : ft_printf("\n\n");
		ft_dprintf(2, "Message Digest commands:\n");
		i = -1;
		while (g_h[++i])
			ft_dprintf(2, FMTSTR, g_h[i]);
		(i + 1) % 4 ? ft_printf("\n") : ft_printf("\n\n");
		ft_dprintf(2, "Cipher commands:\n");
		i = -1;
		while (g_c[++i])
			ft_dprintf(2, FMTSTR, g_c[i]);
		(i + 1) % 4 ? ft_printf("\n") : ft_printf("\n\n");
	}
	!h->shell ? exit(0) : 0;
}

void	std_version(t_hash *h)
{
	ft_printf("ft_ssl %s\n", FT_SSL_VERSION);
	!h->shell ? exit(0) : 0;
}

void	std_nyimpl(t_hash *h)
{
	exit(panic_(-2, h->av[1]));
}

static void	(*g_std[])(t_hash*) = {
	[DGST] = std_dgst,
	[HELP] = std_help,
	[VER] = std_version,
	std_nyimpl
};

void	std_process(t_hash *h)
{
	if (h->id.y == 0)
		g_std[h->id.x](h);
	else
		std_nyimpl(h);
}

#undef FMTSTR
#undef INCARG
