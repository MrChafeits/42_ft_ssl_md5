/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:23:15 by callen            #+#    #+#             */
/*   Updated: 2018/11/02 17:29:14 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_base(int n, int b)
{
	char			*result;
	int				size;
	unsigned int	tmp;

	size = ft_nbrlen_base(n, b);
	tmp = n;
	if (b < 2 || b > 16)
		return (NULL);
	if (n < 0)
	{
		tmp = -n;
		size++;
	}
	if ((result = ft_strnew(size)) == NULL)
		return (NULL);
	result[--size] = ITOCB(tmp, b);
	while (tmp /= b)
		result[--size] = ITOCB(tmp, b);
	if (n < 0)
		*result = '-';
	return (result);
}
