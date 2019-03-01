/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getutype.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:32:43 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:32:45 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		ft_typeuint(t_ull *nbr)
{
	*nbr = (unsigned int)(*nbr);
}

void		ft_typeuhh(t_ull *nbr)
{
	*nbr = (unsigned char)(*nbr);
}

void		ft_typeuh(t_ull *nbr)
{
	*nbr = (unsigned short)(*nbr);
}

static void	ft_fill_tab(void (***f)(t_ull *))
{
	if (!(*f = malloc(sizeof(**f) * 7)))
		ft_puterr("error malloc in ft_getuype");
	(*f)[0] = ft_typeuint;
	(*f)[FHH] = ft_typeuhh;
	(*f)[FH] = ft_typeuh;
	(*f)[FJ] = ft_typeuj;
	(*f)[FZ] = ft_typeuz;
	(*f)[FL] = ft_typeul;
	(*f)[FLL] = ft_typeull;
}

void		ft_getutype(t_uint32 type, t_ull *nbr)
{
	static void	(**f)(t_ull *);

	if (!f)
		ft_fill_tab(&f);
	(*f[type])(nbr);
}
