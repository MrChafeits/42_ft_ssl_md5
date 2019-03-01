/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_la.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:52:16 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:52:17 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_initopt(int opt[4], int size, t_flags *flags, char c)
{
	ADDORSPACE = (FLAGS & (FADD + FSPACE)) > 0 && c != '-';
	BLANKS = flags->blanks - size - ADDORSPACE;
	SUB = c == '-';
}

static int	ft_addopt(int size, char *str, t_flags *flags)
{
	int		opt[4];

	ft_initopt(opt, size, flags, str[0]);
	if (BLANKS > 0 && !(FLAGS & FSUB) && !(FLAGS & FZE))
		ft_printf_strnew(' ', BLANKS, FD);
	if (FLAGS & FADD && str[0] != '-')
		ft_buttfucc(FD, "+", 1);
	else if (FLAGS & FSPACE && str[0] != '-')
		ft_buttfucc(FD, " ", 1);
	if (SUB)
		ft_buttfucc(FD, str, 1);
	if (FLAGS & FUP)
		ft_buttfucc(FD, "0X", 2);
	else
		ft_buttfucc(FD, "0x", 2);
	if (BLANKS > 0 && !(FLAGS & FSUB) && (FLAGS & FZE))
		ft_printf_strnew('0', BLANKS, FD);
	ft_buttfucc(FD, str + SUB, size - SUB);
	if (BLANKS > 0 && (FLAGS & FSUB))
		ft_printf_strnew(' ', BLANKS, FD);
	return (size + ADDORSPACE + BLANKS * (BLANKS > 0) + 2);
}

static int	ft_prec(char *str)
{
	int		size;

	size = ft_strlen(str);
	if (*(str + size - 1) >= '5')
	{
		while (*(str - 1) == '9')
		{
			str--;
			*str = '0';
		}
		str--;
		if (*str != '.')
			*str += 1;
		else
			*(str - 1) += 1;
	}
	return (size);
}

static int	ft_fatwank(char *str)
{
	int		i;

	i = 0;
	while (*(str - i) == '0')
		i++;
	if (i > 0)
		ft_memmove(str - i + 1, str + 1, ft_strlen(str));
	return (i);
}

int			ft_printf_la(long double nbr, t_flags *flags)
{
	int		size;
	int		len;
	char	*str;

	len = 0;
	size = ft_printf_dtoa_la(nbr, flags, &str);
	if (flags->prec >= 0)
	{
		len = ft_prec(str + size);
		size -= ft_fatwank(str + size - 1);
	}
	size = ft_addopt(size + len - 1, str, flags);
	free(str);
	return (size);
}
