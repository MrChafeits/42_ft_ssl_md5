/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dtoa_lf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:36:01 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:36:02 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ft_printf_dtoa_lf(long double nbr, t_flags *flags, char **str)
{
	int		size;
	int		i;
	char	*ent;
	char	*dec;

	size = ft_printf_dtoa_lent(nbr, &ent);
	flags->prec += (!(FLAGS & FPREC)) * 6;
	if (!flags->prec)
	{
		*str = ent;
		return (size);
	}
	if (!(dec = (char *)malloc(sizeof(*dec) * (flags->prec + 3))))
		ft_puterr("error malloc in ft_printf_dtoa_f");
	dec[0] = '.';
	i = 1;
	while (i < flags->prec + 2)
	{
		nbr = (nbr - (long)nbr) * 10 * (-2 * (nbr < 0) + 1);
		dec[i] = (long)(nbr) + '0';
		i++;
	}
	dec[i] = '\0';
	*str = ft_strjoin_free(ent, dec, 'L' + 'R');
	return (size + flags->prec + 1);
}
