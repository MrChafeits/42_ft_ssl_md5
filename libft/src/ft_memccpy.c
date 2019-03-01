/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 16:30:29 by callen            #+#    #+#             */
/*   Updated: 2018/09/21 16:30:30 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	t_uint8	*tmp;
	size_t	i;

	i = -1;
	tmp = (t_uint8 *)dst;
	while (++i < n)
	{
		*(tmp + i) = *((t_uint8 *)src + i);
		if (*((t_uint8 *)src + i) == (t_uint8)c)
			return (dst + i + 1);
	}
	return (NULL);
}
