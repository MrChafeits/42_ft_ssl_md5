/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ktoa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 00:52:54 by callen            #+#    #+#             */
/*   Updated: 2018/11/14 00:52:55 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static time_t	ft_changehour(time_t year, time_t month, time_t num, time_t day)
{
	time_t	tmp;

	tmp = num;
	if (year < 1975 || month < 3 || month > 10)
		return (0);
	if (month == 3 && num < 25)
		return (0);
	if (month == 10 && num < 25)
		return (1);
	if (month == 3 || month == 10)
	{
		while (tmp < 31)
		{
			day++;
			if (day == 6)
				return (0);
			tmp++;
		}
		return (month == 3);
	}
	return (1);
}

static void		ft_finishstr(time_t date[6], char *str)
{
	if (date[2] < 10)
		str[8] = ' ';
	else
		str[8] = date[2] / 10 + '0';
	str[9] = date[2] % 10 + '0';
	str[10] = ' ';
	str[11] = date[3] / 10 + '0';
	str[12] = date[3] % 10 + '0';
	str[13] = ':';
	str[14] = date[4] / 10 + '0';
	str[15] = date[4] % 10 + '0';
	str[16] = ':';
	str[17] = date[5] / 10 + '0';
	str[18] = date[5] % 10 + '0';
	str[19] = ' ';
	str[20] = date[0] / 1000 + '0';
	str[21] = (date[0] % 1000) / 100 + '0';
	str[22] = (date[0] % 100) / 10 + '0';
	str[23] = date[0] % 10 + '0';
	str[24] = '\0';
}

static void		ft_filldate(time_t date[6], time_t tmp, time_t k, char *str)
{
	date[2] = tmp + 1;
	k %= 86400;
	date[3] = k / 3600;
	k %= 3600;
	date[4] = k / 60;
	k %= 60;
	date[5] = k;
	ft_finishstr(date, str);
}

int				ft_printf_ktoa(time_t k, char **str)
{
	time_t	date[6];
	time_t	bis;
	time_t	tmp;
	time_t	day;

	k += 3600;
	day = (k / 86400 + 3) % 7;
	ft_day(day, str);
	bis = 0;
	date[0] = k / 31556952 + 1970;
	if ((!(date[0] % 4) && (date[0] % 100)) || !(date[0] % 400))
		bis = 1;
	k -= (date[0] - 1970) * 31536000 + (((date[0] - 1969) / 4)
			- ((date[0] - 1900) / 100) + ((date[0] - 1600) / 400)) * 86400;
	tmp = k / 86400;
	date[1] = ft_getmonth(&tmp, bis);
	if (ft_changehour(date[0], date[1], tmp, day))
	{
		k += 3600;
		tmp = k / 86400;
		date[1] = ft_getmonth(&tmp, bis);
	}
	ft_month(date[1], *str);
	ft_filldate(date, tmp, k, *str);
	return (24);
}
