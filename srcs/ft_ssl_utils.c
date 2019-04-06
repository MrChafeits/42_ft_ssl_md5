/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:07:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/05 22:44:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <libft.h>

int		panic_(int fd, char *str)
{
	ft_dprintf(2, "%s: %s: %s\n", "ft_ssl", str, strerror(errno));
	if (fd != -1)
		close(fd);
	return (-1);
}

t_i32v	get_command_(t_hash *h, const char *s)
{
	static char	**hecc[] = {(char**)&g_s, (char**)&g_h, (char**)&g_c, 0};
	t_i32v		i;

	if (ft_strequ("help", s))
		ft_ssl_help(h);
	i.y = -1;
	while (hecc[++i.y])
	{
		i.x = -1;
		while (hecc[i.y][++i.x])
			if (ft_strequ(hecc[i.y][i.x], s))
				break ;
		if (!hecc[i.y][i.x])
			continue ;
		else
		{
			i.x++;
			break ;
		}
	}
	return (i);
}

int		len_strtab(char **t)
{
	int i;

	i = 0;
	while (t[i])
		++i;
	return (i);
}

void	ft_free_strtab(char ***tab)
{
	register int i;

	i = 0;
	while ((*tab)[i])
	{
		free((*tab)[i]);
		i++;
	}
	free((*tab));
}
