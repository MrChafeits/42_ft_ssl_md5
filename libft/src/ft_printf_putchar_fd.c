/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_putchar_fd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:54:57 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:54:59 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_return(int fd, void *c, int ret)
{
	ft_buttfucc(fd, c, ret);
	return (ret);
}

int			ft_printf_putchar_fd(wchar_t c, int fd)
{
	unsigned int	octet;

	if (!(c >> (7 + (MB_CUR_MAX == 1))))
		return (ft_return(fd, &c, 1));
	else if (!(c >> 11))
	{
		octet = (((c & 0x3f) << 8) | (c >> 6)) | 0x80c0;
		return (ft_return(fd, &octet, 2));
	}
	else if (!(c >> 16))
	{
		octet = (((c & 0x3f) << 16) | (((c >> 6) & 0x3f) << 8)
				| (c >> 12)) | 0x8080e0;
		return (ft_return(fd, &octet, 3));
	}
	else if (!(c >> 21))
	{
		octet = (((((c & 0x3f) << 24) | (((c >> 6) & 0x3f) << 16))
				| (((c >> 12) & 0x3f) << 8)) | (c >> 18)) | 0x808080f0;
		return (ft_return(fd, &octet, 4));
	}
	return (-1);
}
