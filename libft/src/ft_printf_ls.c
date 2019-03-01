/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:54:22 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:54:23 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_addopt(int size, wchar_t *str, t_flags *flags)
{
	int		i;
	int		spaces;

	if (size > flags->prec && (FLAGS & FPREC))
		size = flags->prec;
	spaces = flags->blanks - size;
	if (!(flags->flags & FSUB) && spaces > 0)
		ft_printf_strnew(' ' + 16 * ((FLAGS & FZE) > 0), spaces, FD);
	i = 0;
	while (i < size)
	{
		i += ft_printf_putchar_fd(*str, FD);
		str++;
	}
	if (flags->flags & FSUB && spaces > 0)
		ft_printf_strnew(' ', spaces, FD);
	return (size + spaces * (spaces > 0));
}

static int	ft_sizechar(wchar_t c)
{
	if (!(c >> (7 + (MB_CUR_MAX == 1))))
		return (1);
	else if (!(c >> 11) && MB_CUR_MAX >= 2)
		return (2);
	else if (!(c >> 16) && MB_CUR_MAX >= 3)
		return (3);
	else if (!(c >> 21) && MB_CUR_MAX >= 4)
		return (4);
	return (-1);
}

int			ft_printf_wstrlen(wchar_t *str, t_flags *flags)
{
	int		i;
	int		size;
	int		tmp;

	size = 0;
	i = 0;
	while (str[i] && (size < flags->prec || !(FLAGS & FPREC)))
	{
		if ((tmp = ft_sizechar(str[i])) == -1)
			return (-1);
		size += tmp;
		i++;
	}
	if (size > flags->prec && (FLAGS & FPREC))
		size -= tmp;
	return (size);
}

int			ft_printf_ls(wchar_t *str, t_flags *flags)
{
	int		size;
	int		tmp;

	tmp = 0;
	if (!str)
	{
		size = 6;
		if (!(str = (wchar_t *)malloc(sizeof(*str) * 7)))
			ft_puterr("error malloc in ft_printf_ls");
		ft_wstrcpy(str, L"(null)");
		tmp = 1;
	}
	else if ((size = ft_printf_wstrlen(str, flags)) == -1)
		return (-1);
	size = ft_addopt(size, str, flags);
	if (tmp)
		free(str);
	return (size);
}
