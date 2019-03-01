/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 16:09:18 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 18:31:04 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_bzero(void *s, size_t n)
{
	unsigned long	*p;
	unsigned char	*c;
	size_t			i;

	p = (unsigned long*)s;
	c = (unsigned char*)s;
	i = 0;
	while (n >= 8)
	{
		p[i++] = 0;
		n -= 8;
	}
	i *= 8;
	while (n > 0)
	{
		c[i++] = 0;
		n--;
	}
}
