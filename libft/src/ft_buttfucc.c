/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buttfucc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:28:48 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:34:56 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_if(int fd, int size, t_uint8 **buf, int *p)
{
	if (!*buf)
	{
		if (!(*buf = (t_uint8 *)malloc(sizeof(**buf) * BUF_SIZE)))
			ft_puterr("error malloc in ft_buf");
	}
	if (size == -1)
	{
		write(fd, *buf, *p);
		*p = 0;
		return (0);
	}
	if (*p + size > BUF_SIZE)
	{
		write(fd, *buf, *p);
		*p = 0;
	}
	return (1);
}

static void	ft_buttfucccpy(t_uint8 *buf, int *p, void *str, int size)
{
	t_uint64	*tmp1;
	t_uint64	*tmp2;
	t_uint8		*tmp3;
	int			i;

	tmp1 = (t_uint64*)buf;
	tmp2 = (t_uint64*)str;
	tmp3 = (t_uint8 *)str;
	i = 0;
	while (size >= 8)
	{
		tmp1[i] = tmp2[i];
		i++;
		size -= 8;
		*p += 8;
	}
	i *= 8;
	while (size > 0)
	{
		buf[i] = tmp3[i];
		i++;
		size--;
		(*p)++;
	}
}

int			ft_buttfucc(int fd, void *str, int size)
{
	static t_uint8	*buf = NULL;
	static int		p = 0;

	if (!ft_if(fd, size, &buf, &p))
		return (p);
	if (size > BUF_SIZE)
	{
		write(fd, str, size);
		return (size);
	}
	ft_buttfucccpy(buf + p, &p, str, size);
	return (0);
}
