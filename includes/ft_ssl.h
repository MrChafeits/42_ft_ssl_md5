/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 00:03:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/01 11:46:39 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <sys/stat.h>
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
typedef enum e_functype	t_functype;
typedef enum e_dgsts	t_dgsts;
typedef struct s_function	t_function;
typedef struct s_opt	t_opt;

enum	e_dgsts
{
	INVAL,
	MD5,
	SHA1,
	SHA224,
	SHA256,
	SHA384,
	SHA512
};
struct	s_opt
{
	const char	*name;
	int			retval;
	int			valtype;
	const char	*helpstr;
};
enum	e_functype
{
	FT_none,
	FT_general,
	FT_md,
	FT_cipher,
	FT_pkey,
	FT_md_alg,
	FT_cipher_alg
};
struct	s_function
{
	t_functype	type;
	const char	*name;
	int			(*func)(int argc, char *argv[]);
	const t_opt	*help;
};

int		panic(int fd, char *str);
void	ft_md5_process(int ac, char **av);
void	ft_sha1_process(int ac, char **av);
void	ft_sha224_process(int ac, char **av);
void	ft_sha256_process(int ac, char **av);
void	ft_sha384_process(int ac, char **av);
void	ft_sha512_process(int ac, char **av);

#endif
