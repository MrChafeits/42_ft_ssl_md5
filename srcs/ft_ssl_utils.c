/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:07:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:07:28 by callen           ###   ########.fr       */
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

int		get_command_(const char *s)
{
	int		ret;

	if (ft_strequ("md5", s))
		ret = MD5;
	else if (ft_strequ("sha1", s))
		ret = SHA1;
	else if (ft_strequ("sha224", s))
		ret = SHA224;
	else if (ft_strequ("sha256", s))
		ret = SHA256;
	else if (ft_strequ("sha384", s))
		ret = SHA384;
	else if (ft_strequ("sha512", s))
		ret = SHA512;
	else
		ret = INVAL;
	return (ret);
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
