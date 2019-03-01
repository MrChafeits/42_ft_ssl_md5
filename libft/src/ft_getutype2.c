/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getutype2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:32:51 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:32:56 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_typeuj(t_ull *nbr)
{
	*nbr = (uintmax_t)(*nbr);
}

void	ft_typeuz(t_ull *nbr)
{
	*nbr = (size_t)(*nbr);
}

void	ft_typeul(t_ull *nbr)
{
	*nbr = (unsigned long)(*nbr);
}

void	ft_typeull(t_ull *nbr)
{
	(void)nbr;
}
