/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:23:15 by callen            #+#    #+#             */
/*   Updated: 2018/09/24 19:23:17 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_itoa(int n)
{
	char			*result;
	int				size;
	unsigned int	tmp;

	size = ft_nbrlen_base(n, 10);
	tmp = n;
	if (n < 0)
	{
		tmp = -n;
		size++;
	}
	if ((result = ft_strnew(size)) == NULL)
		return (NULL);
	result[--size] = tmp % 10 + '0';
	while (tmp /= 10)
		result[--size] = tmp % 10 + '0';
	if (n < 0)
		*result = '-';
	return (result);
}
