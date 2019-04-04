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
**    -c verify checksum input
**    if none then openssl cli-interface
*/

typedef unsigned long	t_ul;
typedef uint64_t	t_u64;
typedef uint32_t	t_u32;
typedef uint16_t	t_u16;
typedef uint8_t	t_u8;
typedef struct s_hash	t_hash;
typedef enum e_dgsts	t_dgsts;

struct	s_hash
{
	void	(*f)(t_hash*);
	void	(*init)();
	void	(*update)();
	void	(*final)();
	void	(*print)(t_hash*);
	void	*ctx;
	char	*path;
	char	**av;
	int		dgst_len;
	int		ac;
	int		id;
	int		bsd;
	int		echo;
	int		quiet;
	int		string;
	int		check;
};

enum
{
	MD5_DIGEST_SIZE = (128 / 8),
	TIGER128_DIGEST_SIZE = (128 / 8),
	TIGER160_DIGEST_SIZE = (160 / 8),
	TIGER192_DIGEST_SIZE = (192 / 8),
	SHA1_DIGEST_SIZE = (160 / 8),
	SHA224_DIGEST_SIZE = (224 / 8),
	SHA256_DIGEST_SIZE = (256 / 8),
	SHA384_DIGEST_SIZE = (384 / 8),
	SHA512_DIGEST_SIZE = (512 / 8),
	WHIRL_DIGEST_SIZE = (512 / 8)
};
enum	e_dgsts
{
	INVAL,
	MD5,
	SHA1,
	SHA224,
	SHA256,
	SHA384,
	SHA512,
	SHA512224,
	SHA512256,
	TIGER128,
	TIGER160,
	TIGER192,
	WHIRL
};

static char	*g_s[] = {};

static char	*g_h[] = {
	"md5",
	"sha1",
	"sha224",
	"sha256",
	"sha384",
	"sha512",
	"sha512224",
	"sha512256",
	"tiger128",
	"tiger160",
	"tiger192",
	"whirlpool",
};

static char	*g_c[] = {
	"des",
	"des-cbc",
	"des-ecb",
};

int		panic(int fd, char *str);
void	ft_ssl_help(t_hash *h);
void	ft_md5_process(t_hash *h);
void	ft_sha1_process(t_hash *h);
void	ft_sha224_process(t_hash *h);
void	ft_sha256_process(t_hash *h);
void	ft_sha384_process(t_hash *h);
void	ft_sha512_process(t_hash *h);
void	ft_sha512224_process(t_hash *h);
void	ft_sha512256_process(t_hash *h);

#endif
