/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dtoa_ent.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:36:24 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:36:26 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_init(double *nbr, int *size, int *i, int *sub)
{
	if (*nbr < 0)
	{
		*nbr = -*nbr;
		*sub = 1;
		*size = 2;
		*i = 1;
	}
	else
	{
		*i = 0;
		*sub = 0;
		*size = 1;
	}
}

static void	ft_squint(char **str, int size)
{
	if (!(*str = (char *)malloc(sizeof(**str) * (size + 1))))
		ft_puterr("error malloc in ft_printf_itoa");
}

int			ft_printf_dtoa_ent(double nbr, char **str)
{
	int			size;
	double		div;
	int			i;
	int			sub;

	ft_init(&nbr, &size, &i, &sub);
	div = 1;
	while (div <= nbr / 10)
	{
		div *= 10;
		size += 1;
	}
	ft_squint(str, size + 1);
	if (sub)
		(*str)[0] = '-';
	while (div >= 1)
	{
		(*str)[i] = (int)(nbr / div) + '0';
		while (nbr >= div)
			nbr -= div;
		div /= 10;
		i++;
	}
	(*str)[i] = '\0';
	return (size);
}
