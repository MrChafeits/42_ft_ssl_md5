/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 16:13:09 by callen            #+#    #+#             */
/*   Updated: 2018/09/24 16:13:11 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t	i;
	char	*res;
	size_t	len;

	if (!s)
		return (NULL);
	i = -1;
	len = ft_strlen(s);
	if (!(res = (char *)ft_memalloc(len + 1)))
		return (NULL);
	while (++i < len)
		res[i] = f(s[i]);
	res[i] = '\0';
	return (res);
}
