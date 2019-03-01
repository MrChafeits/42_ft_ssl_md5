/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 22:51:23 by callen            #+#    #+#             */
/*   Updated: 2018/09/23 22:51:26 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const char	*sb;
	size_t		i;

	sb = (const char *)s;
	i = -1;
	while (++i < n)
		if (*(sb + i) == (char)c)
			return ((void *)sb + i);
	return (NULL);
}
