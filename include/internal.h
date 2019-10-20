/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:16:14 by callen            #+#    #+#             */
/*   Updated: 2019/10/19 21:45:27 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
# include "ft_ssl.h"
# include "ft_md5.h"
# include "ft_sha1.h"
# include "ft_sha256.h"
# include "ft_sha512.h"

typedef union u_ctx	t_ctx;
union	u_ctx {
	t_md5_ctx	md5;
	t_s1ctx		sha1;
	t_sha256	sha2;
	t_sha512	sha512;
};

enum e_cliflags {
	ERROR = -1,
	NONE = 0,
	CLIHELP,
	CLIDGST,
	CLICHECK,
	CLIPRINT,
	CLIQUIET,
	CLIREV,
	CLISTRING
};

static void	(*g_init[])() = {
	[INVAL] = ft_ssl_command_usage,
	[MD5] = ft_md5_init,
	[SHA1] = ft_sha1_init,
	[SHA224] = ft_sha224_init,
	[SHA256] = ft_sha256_init,
	[SHA384] = ft_sha384_init,
	[SHA512] = ft_sha512_init,
	[SHA512224] = ft_sha512224_init,
	[SHA512256] = ft_sha512256_init,
};

static void	(*g_final[])() = {
	[INVAL] = ft_ssl_command_usage,
	[MD5] = ft_md5_final,
	[SHA1] = ft_sha1_final,
	[SHA224] = ft_sha224_final,
	[SHA256] = ft_sha256_final,
	[SHA384] = ft_sha512_final,
	[SHA512] = ft_sha512_final,
	[SHA512224] = ft_sha512_final,
	[SHA512256] = ft_sha512_final,
};

static void	(*g_update[])() = {
	[INVAL] = ft_ssl_command_usage,
	[MD5] = ft_md5_update,
	[SHA1] = ft_sha1_update,
	[SHA224] = ft_sha224_update,
	[SHA256] = ft_sha256_update,
	[SHA384] = ft_sha512_update,
	[SHA512] = ft_sha512_update,
	[SHA512224] = ft_sha512_update,
	[SHA512256] = ft_sha512_update,
};

static int	g_dgst_size[] = {
	[INVAL] = INVAL_DIGEST_SIZE,
	[MD5] = MD5_DIGEST_SIZE,
	[SHA1] = SHA1_DIGEST_SIZE,
	[SHA224] = SHA224_DIGEST_SIZE,
	[SHA256] = SHA256_DIGEST_SIZE,
	[SHA384] = SHA384_DIGEST_SIZE,
	[SHA512] = SHA512_DIGEST_SIZE,
	[SHA512224] = SHA224_DIGEST_SIZE,
	[SHA512256] = SHA256_DIGEST_SIZE,
};

t_i32v	get_command_(t_ssl_env *h, const char *s);
int		len_strtab(char **t);
void	ft_free_strtab(char ***tab);
int		panic_(int fd, char *str);
void	hash_process(t_ssl_env *h);
void	std_process(t_ssl_env *h);
int		cmp_hash_str(t_ssl_env *h, const char *s, t_u8 *md);
int		str_in_tab(const char *s, const char **t);
int		ft_ssl_getopt(t_ssl_env *env);
void	std_dgst_help(t_ssl_env *h);
char	**strsplit_str(const char *s, const char *p);
void	std_help(t_ssl_env *h);
void	std_version(t_ssl_env *h);
void	hash_process(t_ssl_env *h);
void	std_nyimpl(t_ssl_env *h);
void	hash_print(t_ssl_env *h, int fd);
int		hash_digest_files(t_ssl_env *h);
void	hash_string_arg(t_ssl_env *h);

#endif
