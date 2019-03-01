/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_lg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:52:43 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:52:45 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ft_printf_lg(long double nbr, t_flags *flags)
{
	long double	prec;
	int			i;

	if (nbr < 0.0001 && nbr > -0.0001)
		return (ft_printf_lg_le(nbr, flags));
	if (!(FLAGS & FPREC))
	{
		FLAGS |= FPREC;
		flags->prec = 6;
	}
	prec = 1;
	i = 0;
	while (i < flags->prec)
	{
		prec *= 10;
		i++;
	}
	if (nbr > prec || nbr < -prec)
		return (ft_printf_lg_le(nbr, flags));
	else
		return (ft_printf_lg_lf(nbr, flags));
}
