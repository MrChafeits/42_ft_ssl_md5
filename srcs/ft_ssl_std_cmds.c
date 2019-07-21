/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_std_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 11:36:18 by callen            #+#    #+#             */
/*   Updated: 2019/04/16 11:36:49 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"

#ifdef FMTSTR
# undef FMTSTR
#endif
#define FMTSTR ((!((i + 1) % 4) && i) ? "%-18s\n" : "%-18s")

void	std_help(t_hash *h)
{
	register int i;

	if (h->ac <= 2 && !h->help)
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
	ft_printf("(int) h->shell = %d\n", h->shell);
}

void	std_version(t_hash *h)
{
	ft_printf("ft_ssl %s\n", FT_SSL_VERSION);
	!h->shell ? exit(0) : h->shell;
}

void	std_nyimpl(t_hash *h)
{
	panic_(-2, h->av[1]);
	!h->shell ? exit(1) : h->shell;
}

#undef FMTSTR
