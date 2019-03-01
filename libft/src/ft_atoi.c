/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 15:19:17 by callen            #+#    #+#             */
/*   Updated: 2018/09/27 19:25:46 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_result(unsigned long res, int neg, const char *nptr)
{
	int i;

	i = 0;
	if (*nptr == '-')
		nptr++;
	if (res > FT_LONG_MAX)
		return (neg ? 0 : -1);
	return (neg ? -res : res);
}

int			ft_atoi(const char *nptr)
{
	unsigned long	res;
	int				neg;
	const char		*tmp;

	res = 0;
	neg = 0;
	while (ft_isspace(*nptr))
		++nptr;
	tmp = nptr;
	if (*tmp == '-' || *tmp == '+')
	{
		if (*tmp == '-')
			neg = 1;
		tmp++;
	}
	while (ft_isdigit(*tmp))
	{
		res = (res * 10) + (*tmp - '0');
		tmp++;
	}
	return (check_result(res, neg, nptr));
}
