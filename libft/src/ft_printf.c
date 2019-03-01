/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:36:46 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:36:47 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	pf_nonut_norm(t_flags *flags, int *nb)
{
	FD = 1;
	*nb = 0;
}

int			ft_printf(char *str, ...)
{
	t_flags	flags;
	va_list	ap;
	int		nb;

	va_start(ap, str);
	pf_nonut_norm(&flags, &nb);
	while (*str)
	{
		if (*str == '%')
		{
			if ((nb += ft_per(&str, &flags, ap)) == -1)
				return (-1);
		}
		else if (*str == '{')
			nb += ft_color(&str, flags.fd);
		else
		{
			ft_buttfucc(flags.fd, str, 1);
			nb++;
		}
		str++;
	}
	va_end(ap);
	ft_buttfucc(flags.fd, NULL, -1);
	return (nb);
}
