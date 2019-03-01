/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:30:00 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:32:35 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_pars	ft_fill_tab_pars(t_uint32 u, void (*f)(t_flags *, t_uint32,
								char **, va_list))
{
	t_pars	ret;

	ret.u = u;
	ret.f = f;
	return (ret);
}

static void		ft_fill_tab(t_pars **f)
{
	if (!(*f = (t_pars *)malloc(sizeof(**f) * 128)))
		ft_puterr("error malloc in ft_flags");
	(*f)['#'] = ft_fill_tab_pars(FHT, ft_fillflags);
	(*f)['0'] = ft_fill_tab_pars(FZE, ft_fillflags);
	(*f)['1'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['2'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['3'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['4'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['5'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['6'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['7'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['8'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['9'] = ft_fill_tab_pars(0, ft_banger);
	(*f)['-'] = ft_fill_tab_pars(FSUB, ft_fillflags);
	(*f)['+'] = ft_fill_tab_pars(FADD, ft_fillflags);
	(*f)[' '] = ft_fill_tab_pars(FSPACE, ft_fillflags);
	(*f)['.'] = ft_fill_tab_pars(FPREC, ft_precs);
	(*f)['h'] = ft_fill_tab_pars(FH, ft_type);
	(*f)['l'] = ft_fill_tab_pars(FL, ft_type);
	(*f)['j'] = ft_fill_tab_pars(FJ, ft_type);
	(*f)['z'] = ft_fill_tab_pars(FZ, ft_type);
	(*f)['L'] = ft_fill_tab_pars(FL2, ft_fillflags);
	(*f)['*'] = ft_fill_tab_pars(0, ft_banger);
}

void			ft_flags(char **str, t_flags *flags, va_list ap)
{
	static t_pars *f = NULL;

	if (!f)
		ft_fill_tab(&f);
	(*f[(int)**str].f)(flags, f[(int)**str].u, str, ap);
}
