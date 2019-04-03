/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 00:03:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 21:29:35 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <sys/stat.h>
# include <stdint.h>
# include <fcntl.h>
# include <errno.h>

/*
** Allowed functions:[open, close, read, write, malloc, free]
** md5 && sha256:
**    -p echo stdin to stdout and append hash to stdout
**    -q quiet mode
**    -r reverse output
**    -s hash args as strings
**    if none then openssl cli-interface
*/

typedef unsigned long	t_ul;
typedef uint64_t	t_u64;
typedef uint32_t	t_u32;
typedef uint16_t	t_u16;
typedef uint8_t	t_u8;
typedef enum e_dgsts	t_dgsts;

enum	e_dgsts
{
	INVAL,
	MD5,
	SHA1,
	SHA224,
	SHA256,
	SHA384,
	SHA512,
	TIGER,
	WHIRL
};

int		panic(int fd, char *str);
void	ft_ssl_usage(int ac, char **av);
void	ft_md5_process(int ac, char **av);
void	ft_sha1_process(int ac, char **av);
void	ft_sha224_process(int ac, char **av);
void	ft_sha256_process(int ac, char **av);
void	ft_sha384_process(int ac, char **av);
void	ft_sha512_process(int ac, char **av);

#endif
