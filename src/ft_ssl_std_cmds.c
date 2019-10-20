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

#define FMTSTR ((!((i) % 4) && i) ? "%-18s\n" : "%-18s")

void	std_help(t_ssl_env *h)
{
	int i;

	if (h->ac <= 2 && !h->help)
	{
		ft_dprintf(2, "Standard commands:\n");
		i = 0;
		while (++i < MAX_STDCMD)
			ft_dprintf(2, FMTSTR, g_std_cmd[i]);
		(i) % 4 ? ft_printf("\n") : ft_printf("\n\n");
		ft_dprintf(2, "Message Digest commands:\n");
		i = 0;
		while (++i < MAX_MDCMD)
			ft_dprintf(2, FMTSTR, g_dgst_cmd[i]);
		(i) % 4 ? ft_printf("\n") : ft_printf("\n\n");
		ft_dprintf(2, "Cipher commands:\n");
		i = -1;
		while (g_enc_cmd[++i])
			ft_dprintf(2, FMTSTR, g_enc_cmd[i]);
		(i) % 4 ? ft_printf("\n") : ft_printf("\n\n");
	}
	ft_printf("(int) h->shell = %d\n", h->shell);
	!h->shell ? exit(0) : 0;
}

void	std_version(t_ssl_env *h)
{
	ft_printf("ft_ssl %s\n", FT_SSL_VERSION);
	!h->shell ? exit(0) : h->shell;
}

void	std_nyimpl(t_ssl_env *h)
{
	panic_(-2, h->av[1]);
	!h->shell ? exit(1) : h->shell;
}
