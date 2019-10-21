/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strsplit_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 22:10:04 by callen            #+#    #+#             */
/*   Updated: 2019/10/19 22:10:05 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "internal.h"
// NORME
#define BITOP_A(a,b) ((a)[(size_t)(b) / (8 * sizeof(*(a)))])
#define BITOP_B(a,b) ((size_t)1 << ((size_t)(b) % (8 * sizeof(*(a)))))
#define BITOP_AND(a,b) (BITOP_A(a,b) & BITOP_B(a,b))
#define BITOP_OREQ(a,b) (BITOP_A(a,b) |= BITOP_B(a,b))

size_t	ft_strspn(const char *s, const char *c)
{
	const char		*a = s;
	size_t			byteset[32 / sizeof(size_t)];

	if (!c[0])
		return (0);
	if (!c[1])
	{
		while (*s == *c)
			s++;
		return (s - a);
	}
	ft_memset(byteset, 0, sizeof(byteset));
	while (*c && (BITOP_OREQ(byteset, *(unsigned char*)c)))
		c++;
	while (*s && (BITOP_AND(byteset, *(unsigned char*)s)))
		s++;
	return (s - a);
}

size_t	ft_strcspn(const char *s, const char *c)
{
	const char		*a = s;
	size_t			byteset[32 / sizeof(size_t)];

	if (!c[0] || !c[1])
		return (ft_strchrnul(s, *c) - a);
	ft_memset(byteset, 0, sizeof(byteset));
	while (*c && BITOP_OREQ(byteset, *(unsigned char*)c))
		c++;
	while (*s && !BITOP_AND(byteset, *(unsigned char*)s))
		s++;
	return (s - a);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s2;

	if ((s2 = malloc(n + 1)) == NULL)
		return (NULL);
	ft_memcpy(s2, s1, n);
	s2[n] = '\0';
	return (s2);
}

int		wordcount(const char *str, const char *sep)
{
	int total;
	int ii;

	total = 0;
	ii = 0;
	while (str[ii] != '\0')
	{
		ii += ft_strspn(str + ii, sep);
		if (str[ii] != '\0')
			total++;
		ii += ft_strcspn(str + ii, sep);
	}
	return (total);
}

char	**strsplit_str(const char *str, const char *sep)
{
	char	**ret;
	int		len;
	int		words;
	int		ii;
	int		jj;

	words = wordcount(str, sep);
	ret = malloc((words + 1) * sizeof(*ret));
	if (ret == NULL)
		return (NULL);
	ii = 0;
	jj = 0;
	while (str[ii] != '\0')
	{
		ii += ft_strspn(str + ii, sep);
		if ((len = ft_strcspn(str + ii, sep)) != 0)
		{
			ret[jj++] = ft_strndup(str + ii, len);
		}
		ii += len;
	}
	ret[jj] = 0;
	return (ret);
}
