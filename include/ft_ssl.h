/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 00:03:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/05 23:05:27 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <sys/stat.h>
# include <string.h>
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
**    if none then openssl-esque cli-interface
*/

# ifdef FT_SSL_VERSION
#  undef FT_SSL_VERSION
# endif
# define FT_SSL_VERSION "0.0042"

typedef unsigned long	t_ul;
typedef uint64_t	t_u64;
typedef uint32_t	t_u32;
typedef uint16_t	t_u16;
typedef uint8_t	t_u8;
typedef struct s_i32v	t_i32v;
typedef struct s_hash	t_ssl_env;
typedef enum e_dgsts	t_dgsts;

struct	s_i32v {
	int	x;
	int	y;
};

/*
** Absolutely abominable struct ahead
*/

#define DGSTLEN (512 / 8)
#define BUFSIZE (1024 * DGSTLEN)

struct	s_hash {
	void		(*f)(t_ssl_env*);
	void		(*init)();
	void		(*update)();
	void		(*final)();
	void		(*print)(t_ssl_env*);
	void		*ctx;
	char		*path;
	char		*cpth;
	char		**av;
	char		**to_free;
	char		*strarg;
	char		**t;
	t_u8		md[DGSTLEN];
	t_u8		buf[BUFSIZE];
	int			*i;
	int			ii;
	int			chk1;
	int			chk2;
	int			std_cmd;
	int			md_cmd;
	struct stat	st;
	int			cfd;
	int			err;
	int			dgst_len;
	int			ac;
	int			c;
	t_i32v		id;
	int			hc2dgst;
	int			digest;
	int			bsd;
	int			echo;
	int			quiet;
	int			string;
	int			check;
	int			shell;
	int			help;
};

enum {
	INVAL_DIGEST_SIZE = (0),
	MD5_DIGEST_SIZE = (128 / 8),
	SHA1_DIGEST_SIZE = (160 / 8),
	SHA224_DIGEST_SIZE = (224 / 8),
	SHA256_DIGEST_SIZE = (256 / 8),
	SHA384_DIGEST_SIZE = (384 / 8),
	SHA512_DIGEST_SIZE = (512 / 8),
};

enum	e_stdcmd {
	INVAL = 0,
	DGST,
	HELP,
	VER,
	MAX_STDCMD
};

static char	*g_std_cmd[] = {
	NULL,
	[DGST] = "dgst",
	[HELP] = "help",
	[VER] = "version",
};

enum	e_dgsts {
	INVAL_MD,
	MD5,
	SHA1,
	SHA224,
	SHA256,
	SHA384,
	SHA512,
	SHA512224,
	SHA512256,
	MAX_MDCMD
};

static char	*g_dgst_cmd[] = {
	NULL,
	"md5",
	"sha1",
	"sha224",
	"sha256",
	"sha384",
	"sha512",
	"sha512-224",
	"sha512-256",
};

static char	*g_enc_cmd[] = {
	NULL
};

void	ft_ssl_command_usage(t_ssl_env *h);
void	ft_ssl_command_help(t_ssl_env *h);

#endif
