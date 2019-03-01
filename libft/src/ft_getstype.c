/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getstype.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:30:12 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:30:14 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		ft_typesint(t_sll *nbr)
{
	*nbr = (int)(*nbr);
}

void		ft_typeshh(t_sll *nbr)
{
	*nbr = (char)(*nbr);
}

void		ft_typesh(t_sll *nbr)
{
	*nbr = (short)(*nbr);
}

static void	ft_fill_tab(void (***f)(t_sll *))
{
	if (!(*f = malloc(sizeof(**f) * 7)))
		ft_puterr("error malloc in ft_getsype");
	(*f)[0] = ft_typesint;
	(*f)[FHH] = ft_typeshh;
	(*f)[FH] = ft_typesh;
	(*f)[FJ] = ft_typesj;
	(*f)[FZ] = ft_typesz;
	(*f)[FL] = ft_typesl;
	(*f)[FLL] = ft_typesll;
}

void		ft_getstype(t_uint32 type, t_sll *nbr)
{
	static void	(**f)(t_sll *);

	if (!f)
		ft_fill_tab(&f);
	(*f[type])(nbr);
}
