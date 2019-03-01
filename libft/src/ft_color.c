/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_color.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 20:29:16 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:34:59 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_end(char **str, int fd, int test)
{
	if (test)
	{
		while (**str != '}')
			(*str)++;
		return (0);
	}
	else
	{
		(*str)--;
		ft_buttfucc(fd, "{", 1);
	}
	return (1);
}

int			ft_color(char **str, int fd)
{
	int		i;

	i = 0;
	(*str)++;
	if (!ft_strncmp(*str, "black}", 6))
		i = ft_buttfucc(fd, BLK, 6);
	else if (!ft_strncmp(*str, "red}", 4))
		i = ft_buttfucc(fd, RED, 6);
	else if (!ft_strncmp(*str, "green}", 6))
		i = ft_buttfucc(fd, GRN, 6);
	else if (!ft_strncmp(*str, "yellow}", 7))
		i = ft_buttfucc(fd, YEL, 6);
	else if (!ft_strncmp(*str, "blue}", 5))
		i = ft_buttfucc(fd, BLU, 6);
	else if (!ft_strncmp(*str, "magenta}", 8))
		i = ft_buttfucc(fd, MAG, 6);
	else if (!ft_strncmp(*str, "cyan}", 5))
		i = ft_buttfucc(fd, CYN, 6);
	else if (!ft_strncmp(*str, "white}", 6))
		i = ft_buttfucc(fd, WHT, 6);
	else if (!ft_strncmp(*str, "eoc}", 4))
		i = ft_buttfucc(fd, RES, 5);
	return (ft_end(str, fd, i));
}
