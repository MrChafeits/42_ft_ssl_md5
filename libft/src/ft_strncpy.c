/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 21:26:25 by callen            #+#    #+#             */
/*   Updated: 2018/10/19 23:38:35 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dest, const char *src, size_t len)
{
	size_t		i;
	const char	*buf;
	char		*ret;

	i = 0;
	buf = src;
	ret = dest;
	ft_bzero(ret, len);
	while (i < len && *buf)
	{
		*dest = *buf;
		i++;
		buf++;
		dest++;
	}
	return (ret);
}
