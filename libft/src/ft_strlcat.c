/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 23:15:49 by callen            #+#    #+#             */
/*   Updated: 2018/09/23 23:15:51 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t slen;
	size_t dlen;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	if (size <= dlen)
		return (size + slen);
	if (slen < size - dlen)
	{
		ft_memcpy(dst + dlen, src, slen);
		dst = dst + dlen + slen;
	}
	else
	{
		ft_memcpy(dst + dlen, src, size - dlen - 1);
		dst = dst + size - 1;
	}
	*dst = '\0';
	return (slen + dlen);
}
