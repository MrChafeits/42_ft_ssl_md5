/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 16:48:17 by callen            #+#    #+#             */
/*   Updated: 2018/11/15 17:20:24 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Allowed libc functions: read() malloc() free()
** -Write a function that returns a line read from a file descriptor.
** -What we call a “line” is a succession of characters that end with ’\n’
**  (ascii code 0x0a) or with End Of File.
** -The first parameter is the file descriptor that will be used to read.
** -The second parameter is the address of a pointer to a character that will
**  be used to save the line read from the file descriptor.
** -Return 1 for successful read, 0 if EOF has been reached, -1 for errors
** -Exclude \n from line
** ft_bzero ft_strchr ft_strjoin ft_strdup ft_strnew
*/

void	lbuf_add(t_lbuf *alst, t_lbuf *new)
{
	if (alst && new)
	{
		alst->next = new;
		new->prev = alst;
	}
}

t_lbuf	*lbuf_new(int fd)
{
	t_lbuf	*new;

	if (!(new = (t_lbuf*)malloc(sizeof(t_lbuf))))
		return (NULL);
	new->next = NULL;
	new->str = NULL;
	new->fd = fd;
	return (new);
}

int		gnl_find(t_lbuf *buf, char **line)
{
	char	*cur;

	cur = ft_strchr(buf->str, '\n');
	if (cur)
	{
		*cur = '\0';
		*line = ft_strdup(buf->str);
		buf->str = ft_strdup(cur + 1);
		return (1);
	}
	else if (ft_strlen(buf->str) > 0)
	{
		*line = ft_strdup(buf->str);
		*(buf)->str = '\0';
		return (1);
	}
	return (0);
}

t_lbuf	*gnl_read(int fd, t_lbuf *buf)
{
	while (buf && (int)buf->fd < fd)
	{
		if (!buf->next)
			lbuf_add(buf, lbuf_new(fd));
		buf = buf->next;
		if (buf->fd == fd)
			return (buf);
	}
	while (buf && (int)buf->fd > fd)
	{
		buf = buf->prev;
		if (buf->fd == fd)
			return (buf);
	}
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	static t_lbuf	*buf;
	char			tmp[BUFF_SIZE + 1];
	int				bep;

	if (line == NULL || fd < 0 || fd >= FD_MAX || BUFF_SIZE <= 0)
		return (-1);
	if (buf && (int)buf->fd != fd && fd)
		buf = gnl_read(fd, buf);
	if (!buf)
		buf = lbuf_new(fd);
	*line = 0;
	while ((bep = read(fd, tmp, BUFF_SIZE)) > 0)
	{
		tmp[bep] = '\0';
		buf->str = ft_strjoin_free(buf->str, tmp, 1);
		ft_bzero(tmp, BUFF_SIZE + 1);
	}
	if (bep < 0)
		return (-1);
	return (gnl_find(buf, line));
}
