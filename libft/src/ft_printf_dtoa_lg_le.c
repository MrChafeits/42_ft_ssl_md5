/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dtoa_lg_le.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:35:58 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:35:59 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_initdtoa_e(long double *nbr, int *sub, int *exp, int *i)
{
	if (*nbr < 0)
	{
		*sub = 1;
		*nbr = -*nbr;
	}
	else
		*sub = 0;
	*exp = 0;
	while (*nbr >= 10)
	{
		*nbr /= 10;
		(*exp)++;
	}
	while (*nbr < 1)
	{
		*nbr *= 10;
		(*exp)--;
	}
	if (*sub)
		*i = 1;
	else
		*i = 0;
}

static void	ft_signdtoa_e(char *str, int exp, int i, long double nbr)
{
	if (exp < 0)
	{
		str[i + 1] = '-';
		exp = -exp;
	}
	else
		str[i + 1] = '+';
	str[i + 2] = exp / 10 + '0';
	str[i + 3] = exp % 10 + '0';
	str[i + 4] = (int)((nbr - (int)nbr) * 10) + '0';
	str[i + 5] = '\0';
}

int			ft_printf_dtoa_lg_le(long double nbr, t_flags *flags, char **str)
{
	int		exp;
	int		sub;
	int		size;
	int		i;

	ft_initdtoa_e(&nbr, &sub, &exp, &i);
	flags->prec -= (flags->prec != 0);
	size = 5 + flags->prec + (flags->prec > 0) + sub;
	if (!(*str = (char *)malloc(sizeof(**str) * (size + 2 + sub))))
		ft_puterr("error malloc in ft_dtoa_e");
	if (sub)
		(*str)[0] = '-';
	(*str)[i] = (int)nbr + '0';
	i += 1 + (flags->prec > 0);
	if (flags->prec > 0)
		(*str)[i - 1] = '.';
	while (i < flags->prec + 1 + (flags->prec > 0) + sub)
	{
		nbr = (nbr - (int)nbr) * 10;
		(*str)[i] = (int)nbr + '0';
		i++;
	}
	(*str)[i] = 'e' - 32 * ((FLAGS & FUP) > 0);
	ft_signdtoa_e(*str, exp, i, nbr);
	return (size);
}
