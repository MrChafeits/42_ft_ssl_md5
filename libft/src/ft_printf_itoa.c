/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_itoa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:51:30 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:51:31 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_init(t_sll *nbr, int *size, int *i, int *sub)
{
	if (*nbr < 0)
	{
		*sub = 1;
		*size = 2;
		*i = 1;
	}
	else
	{
		*nbr = -*nbr;
		*i = 0;
		*sub = 0;
		*size = 1;
	}
}

int			ft_printf_itoa(t_sll nbr, char **str)
{
	int			size;
	t_sll		div;
	int			i;
	int			sub;

	ft_init(&nbr, &size, &i, &sub);
	div = -1;
	while (div >= nbr / 10)
	{
		div *= 10;
		size += 1;
	}
	if (!(*str = (char *)malloc(sizeof(**str) * (size + 1))))
		ft_puterr("error malloc in ft_printf_itoa");
	if (sub)
		(*str)[0] = '-';
	while (div < 0)
	{
		(*str)[i] = nbr / div + '0';
		nbr %= div;
		div /= 10;
		i++;
	}
	(*str)[i] = '\0';
	return (size);
}
