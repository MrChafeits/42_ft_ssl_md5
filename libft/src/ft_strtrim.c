/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 17:51:20 by callen            #+#    #+#             */
/*   Updated: 2018/09/24 17:51:22 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	can_trim(char const *str, int *i, size_t *j)
{
	while (ft_isspace(*(str + *i)))
		(*i)++;
	while (ft_isspace(*(str + *j)))
		(*j)--;
	if (*i || *j < ft_strlen(str))
		return (1);
	return (0);
}

char		*ft_strtrim(char const *s)
{
	size_t	rlen;
	char	*res;
	int		i;
	size_t	k;
	int		j;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	k = ft_strlen(s) - 1;
	if (!can_trim(s, &i, &k) || !ft_strlen(s))
		return ((char *)s);
	rlen = (i == (int)ft_strlen(s) ? 0 : ft_strlen(s) - (size_t)i - \
		(ft_strlen(s) - k));
	if (!(res = ft_strnew(rlen + 1)))
		return (NULL);
	while (i <= (int)k)
		*(res + j++) = *(s + i++);
	return (res);
}
