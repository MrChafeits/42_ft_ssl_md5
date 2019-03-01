/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 16:54:34 by callen            #+#    #+#             */
/*   Updated: 2018/11/10 11:56:27 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft.h"

# define BUFF_SIZE 64
# define FD_MAX 4864

typedef struct	s_lbuf
{
	struct s_lbuf	*next;
	struct s_lbuf	*prev;
	char			*str;
	int				fd;
}				t_lbuf;

int		get_next_line(const int fd, char **line);

#endif
