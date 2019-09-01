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

int		cmp_hash_str(t_hash *h, const char *s, t_u8 *md)
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

void	doopt(t_hash *h, int c)
{
	if (c == 'c')
		h->check = 1;
	else if (c == 'h')
		ft_ssl_command_help(h);
	else if (c == 'p')
		h->echo = 1;
	else if (c == 'q')
	{
		h->quiet = 1;
		h->bsd = 0;
	}
	else if (c == 'r')
		h->bsd = 1;
	else if (c == 's')
	{
		h->string = 1;
		h->strarg = optarg;
	}
	else
		ft_ssl_command_usage(h);
}
