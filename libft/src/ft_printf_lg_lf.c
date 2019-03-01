/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_lg_lf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:53:42 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:53:44 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_initopt(int opt[4], int size, t_flags *flags, char c)
{
	ADDORSPACE = (FLAGS & (FADD + FSPACE)) > 0 && c != '-';
	BLANKS = flags->blanks - size - ADDORSPACE;
	SUB = BLANKS > 0 && !(FLAGS & FSUB) && (FLAGS & FZE) && c == '-';
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
	if (BLANKS > 0 && !(FLAGS & FSUB) && (FLAGS & FZE))
		ft_printf_strnew('0', BLANKS, FD);
	ft_buttfucc(FD, str + SUB, size - SUB);
	if (BLANKS > 0 && (FLAGS & FSUB))
		ft_printf_strnew(' ', BLANKS, FD);
	return (size + ADDORSPACE + BLANKS * (BLANKS > 0));
}

static void	ft_pleppo(char *str)
{
	if (*str >= '5')
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
}

static int	ft_ffsevenwillnevercomeout(char *str)
{
	int		i;

	i = 0;
	while (*(str - i) == '0')
		i++;
	return (i);
}

int			ft_printf_lg_lf(long double nbr, t_flags *flags)
{
	int		size;
	char	*str;

	size = ft_printf_dtoa_lg_lf(nbr, flags, &str);
	if (flags->prec > 0)
	{
		ft_pleppo(str + size);
		size -= ft_ffsevenwillnevercomeout(str + size - 1);
	}
	size = ft_addopt(size, str, flags);
	free(str);
	return (size);
}
