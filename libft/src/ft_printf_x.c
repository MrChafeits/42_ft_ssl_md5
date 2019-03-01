/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_x.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:55:41 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:55:42 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_horsecock(int opt[5], int size, t_flags *flags, int zero)
{
	PREC = flags->prec - size;
	HASHTAG = ((FLAGS & FHT) && !zero) * 2;
	ZERO = ((FLAGS & FPREC) && (flags->prec == 0) && zero);
	BLANKS = flags->blanks - size - PREC * (PREC > 0) -
		HASHTAG + ZERO;
	if (!(FLAGS & FPREC) && (FLAGS & FZE) && !(FLAGS & FSUB))
	{
		PREC = flags->blanks - size - HASHTAG;
		BLANKS = 0;
	}
}

static int	ft_addopt(int size, char *str, t_flags *flags, int zero)
{
	int		opt[5];

	ft_horsecock(opt, size, flags, zero);
	if (!(FLAGS & FSUB) && BLANKS > 0)
		ft_printf_strnew(' ', BLANKS, FD);
	if (HASHTAG && !zero && !(FLAGS & FUP))
		ft_buttfucc(FD, "0x", 2);
	else if (HASHTAG && !zero)
		ft_buttfucc(FD, "0X", 2);
	if (PREC > 0)
		ft_printf_strnew('0', PREC, FD);
	if (ZERO)
		size -= 1;
	else
		ft_buttfucc(FD, str, size);
	if ((FLAGS & FSUB) && BLANKS > 0)
		ft_printf_strnew(' ', BLANKS, FD);
	return (size + PREC * (PREC > 0) +
			BLANKS * (BLANKS > 0) + HASHTAG);
}

int			ft_printf_x(t_flags *flags, va_list ap)
{
	int		size;
	char	*str;
	t_ull	nbr;

	nbr = va_arg(ap, t_ull);
	ft_getutype(TYPE, &nbr);
	size = ft_printf_itoa_base(nbr, 16, &str, (FLAGS & FUP) > 0);
	size = ft_addopt(size, str, flags, nbr == 0);
	free(str);
	return (size);
}
