/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_check_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 21:33:11 by callen            #+#    #+#             */
/*   Updated: 2019/04/07 21:33:13 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "internal.h"

int		str_in_tab(const char *s, const char **t)
{
	int i;

	i = -1;
	while (t[++i])
		if (ft_strequ(s, t[i]))
			break ;
	return (t[i] != NULL ? i : -1);
}

int		str_ocr_strtab(const char *s, const char **t)
{
	int i;
	int total;

	i = -1;
	total = 0;
	while (t[++i])
		if (ft_strequ(s, t[i]))
			total++;
	return (total);
}

int		cmp_hash_str(t_ssl_env *h, const char *s, t_u8 *md)
{
	int			i;
	t_u8		b;
	t_u8		c;
	const char	*p = s;

	if ((int)ft_strlen(s) != h->dgst_len * 2)
		return (1);
	i = -1;
	while (++i < h->dgst_len)
	{
		c = (t_u8)ITOCB(md[i], 16);
		b = (t_u8)ITOCB((md[i] >> 4), 16);
		if (b != p[0] || c != p[1])
			return (1);
		p += 2;
	}
	return (0);
}

char	*ft_strchrnul(const char *s, int c)
{
	c = (unsigned char)c;
	if (!c)
		return ((char*)s + ft_strlen(s));
	while (*s && *(unsigned char*)s != c)
		s++;
	return ((char*)s);
}
