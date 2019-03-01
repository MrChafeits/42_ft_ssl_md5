/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_c.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:36:36 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:36:37 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_sizechar(wchar_t c)
{
	if (!(c >> (7 + (MB_CUR_MAX == 1))))
		return (1);
	else if (!(c >> 11))
		return (2);
	else if (!(c >> 16))
		return (3);
	else if (!(c >> 21))
		return (4);
	return (-1);
}

int			ft_printf_c(t_flags *flags, va_list ap)
{
	int		size;
	int		blanks;
	wchar_t	c;

	c = va_arg(ap, wchar_t);
	if (TYPE != FL)
		c = (unsigned char)c;
	else if (((MB_CUR_MAX == 1) && c > 255) || c < 0)
		return (-1);
	if ((c >= 0xd800 && c <= 0xdfff) || c > 0x10ffff)
		return (-1);
	if (TYPE == FL)
		size = ft_sizechar(c);
	else
		size = 1;
	blanks = flags->blanks - size;
	if (blanks > 0 && !(FLAGS & FSUB))
		ft_printf_strnew(' ' + 16 * ((FLAGS & FZE) > 0), blanks, FD);
	if (TYPE != FL && MB_CUR_MAX > 1)
		ft_buttfucc(FD, &c, 1);
	else
		ft_printf_putchar_fd(c, FD);
	if (blanks > 0 && (FLAGS & FSUB))
		ft_printf_strnew(' ', blanks, FD);
	return (size + blanks * (blanks > 0));
}
