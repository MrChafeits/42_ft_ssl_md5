/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_itoa_a.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:51:12 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:51:13 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_initial_nut(int *nbr, int *size, int *i, int *sub)
{
	*i = 1;
	*size = 2;
	if (*nbr < 0)
		*sub = 1;
	else
	{
		*nbr = -*nbr;
		*sub = 0;
	}
}

int			ft_printf_itoa_a(int nbr, char *str)
{
	int		size;
	int		div;
	int		i;
	int		sub;

	ft_initial_nut(&nbr, &size, &i, &sub);
	div = -1;
	while (div >= nbr / 10)
	{
		div *= 10;
		size += 1;
	}
	if (sub)
		str[0] = '-';
	else
		str[0] = '+';
	while (div < 0)
	{
		str[i] = nbr / div + '0';
		nbr %= div;
		div /= 10;
		i++;
	}
	return (size);
}
