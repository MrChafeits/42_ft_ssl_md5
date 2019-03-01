/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_itoa_base.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:50:22 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:50:23 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	ft_getchar(int n, int up)
{
	if (n < 10)
		return (n + '0');
	return (n - 10 + 'a' - 32 * up);
}

int			ft_printf_itoa_base(t_ull nbr, int base, char **str, int up)
{
	int			size;
	t_ull		div;
	int			i;

	size = 1;
	div = 1;
	while (div <= nbr / base)
	{
		div *= base;
		size += 1;
	}
	if (!(*str = (char *)malloc(sizeof(**str) * (size + 1))))
		ft_puterr("error malloc in ft_printf_itoa_base");
	i = 0;
	while (div > 0)
	{
		(*str)[i] = ft_getchar(nbr / div, up);
		nbr %= div;
		div /= base;
		i++;
	}
	(*str)[i] = '\0';
	return (size);
}
