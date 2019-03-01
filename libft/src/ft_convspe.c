/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convspe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:29:51 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:29:52 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_printf_2(t_flags *flags, va_list ap)
{
	(void)ap;
	(void)flags;
	return (-2);
}

int		ft_printf_3(t_flags *flags, va_list ap)
{
	(void)ap;
	(void)flags;
	return (-3);
}

int		ft_printf_per(t_flags *flags, va_list ap)
{
	(void)ap;
	return (ft_printf_ind('%', flags));
}
