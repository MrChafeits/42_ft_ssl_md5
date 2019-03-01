/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_strnew.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:54:39 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:54:41 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_printf_strnew(char c, int size, int fd)
{
	char	*str;

	if (!(str = (char *)malloc(sizeof(*str) * size)))
		ft_puterr("error malloc in ft_printf_strnew");
	ft_memset(str, c, size);
	ft_buttfucc(fd, str, size);
	free(str);
	return (str);
}
