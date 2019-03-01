/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 19:52:18 by callen            #+#    #+#             */
/*   Updated: 2018/09/11 18:14:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define FT_MASK01		0x0101010101010101UL
#define FT_MASK80		0x8080808080808080UL

size_t	ft_strlen(const char *s)
{
	const char		*p;
	const t_uint64	*lp;
	int				i;

	p = s;
	while ((t_uint64)p & (sizeof(long) - 1))
		if (*p++ == '\0')
			return (--p - s);
	lp = (const t_uint64*)p;
	while (1)
	{
		if ((*lp - FT_MASK01) & FT_MASK80)
		{
			p = (const char*)(lp);
			i = -1;
			while (++i < 8)
				if (!p[i])
					return (p - s + i);
		}
		lp++;
	}
	return (0);
}
