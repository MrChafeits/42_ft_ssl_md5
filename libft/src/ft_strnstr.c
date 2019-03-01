/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 23:35:01 by callen            #+#    #+#             */
/*   Updated: 2018/09/23 23:35:04 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *h, const char *n, size_t len)
{
	size_t i;
	size_t j;

	if (!*n)
		return ((char *)h);
	j = 0;
	while (h[j])
	{
		i = 0;
		while (h[i + j] == n[i] && i + j < len)
			if (!n[++i])
				return ((char *)h + j);
		j++;
	}
	return (NULL);
}
