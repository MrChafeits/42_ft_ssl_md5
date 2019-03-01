/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:29:38 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:35:03 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_fill_tab2(int **tab)
{
	if (!(*tab = ft_memalloc(sizeof(**tab) * 128)))
		ft_puterr("error malloc in ft_conv");
	(*tab)['X'] = 1;
	(*tab)['F'] = 1;
	(*tab)['E'] = 1;
	(*tab)['G'] = 1;
	(*tab)['A'] = 1;
	(*tab)['B'] = 2;
	(*tab)['D'] = 2;
	(*tab)['O'] = 2;
	(*tab)['U'] = 2;
	(*tab)['C'] = 2;
	(*tab)['S'] = 2;
}

static void	ft_ismaj(char *c, t_flags *flags)
{
	static int	*tab = NULL;

	if (!tab)
		ft_fill_tab2(&tab);
	if (tab[(int)*c] == 1)
	{
		FLAGS |= FUP;
		*c += 32;
	}
	else if (tab[(int)*c] == 2)
	{
		if (TYPE < FL)
			TYPE = FL;
		*c += 32;
	}
}

static void	ft_fill_tab(int (***f)(t_flags *, va_list))
{
	int		i;

	if (!(*f = malloc(sizeof(**f) * 128)))
		ft_puterr("error malloc in ft_conv");
	i = -1;
	while (++i < 128)
		(*f)[i] = ft_printf_3;
	(*f)['\0'] = ft_printf_2;
	(*f)['%'] = ft_printf_per;
	(*f)['a'] = ft_printf_a;
	(*f)['b'] = ft_printf_b;
	(*f)['c'] = ft_printf_c;
	(*f)['d'] = ft_printf_d;
	(*f)['e'] = ft_printf_e;
	(*f)['f'] = ft_printf_f;
	(*f)['g'] = ft_printf_g;
	(*f)['i'] = ft_printf_d;
	(*f)['k'] = ft_printf_k;
	(*f)['o'] = ft_printf_o;
	(*f)['p'] = ft_printf_p;
	(*f)['s'] = ft_printf_s;
	(*f)['u'] = ft_printf_u;
	(*f)['x'] = ft_printf_x;
}

int			ft_conv(char c, va_list ap, t_flags *flags)
{
	static int	(**f)(t_flags *, va_list) = NULL;
	int			nb;

	if (!f)
		ft_fill_tab(&f);
	ft_ismaj(&c, flags);
	nb = (*f[(int)c])(flags, ap);
	return (nb);
}
