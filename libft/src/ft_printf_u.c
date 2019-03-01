/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:55:31 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:55:32 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_initopt(int opt[5], int size, t_flags *flags, int zero)
{
	PREC = flags->prec - size;
	ZERO = (FLAGS & FPREC) && (flags->prec == 0) && zero;
	BLANKS = flags->blanks - size - PREC * (PREC > 0);
	if (!(FLAGS & FPREC) && (FLAGS & FZE) && !(FLAGS & FSUB))
	{
		PREC = flags->blanks - size;
		BLANKS = 0;
	}
}

static int	ft_addopt(int size, char *str, t_flags *flags, int zero)
{
	int		opt[5];

	ft_initopt(opt, size, flags, zero);
	if (!(FLAGS & FSUB) && BLANKS > 0)
		ft_printf_strnew(' ', BLANKS, FD);
	if (PREC > 0)
		ft_printf_strnew('0', PREC, FD);
	if (ZERO)
		size -= 1;
	else
		ft_buttfucc(FD, str, size);
	if ((FLAGS & FSUB) && BLANKS > 0)
		ft_printf_strnew(' ', BLANKS, FD);
	return (size + PREC * (PREC > 0) + BLANKS * (BLANKS > 0));
}

int			ft_printf_u(t_flags *flags, va_list ap)
{
	int		size;
	char	*str;
	t_ull	nbr;

	nbr = va_arg(ap, t_ull);
	ft_getutype(TYPE, &nbr);
	size = ft_printf_itoa_base(nbr, 10, &str, 0);
	size = ft_addopt(size, str, flags, nbr == 0);
	free(str);
	return (size);
}
