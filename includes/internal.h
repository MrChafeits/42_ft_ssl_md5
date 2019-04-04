/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:16:14 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:16:16 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H
# include <ft_ssl.h>
# include <ft_md5.h>
# include <ft_sha1.h>
# include <ft_sha256.h>
# include <ft_tiger.h>
# include <ft_whirlpool.h>
# include <ft_sha512.h>

static void	(*g_init[])() = {
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
	[MD5] = MD5_DIGEST_SIZE,
	[SHA1] = SHA1_DIGEST_SIZE,
	[SHA224] = SHA224_DIGEST_SIZE,
	[SHA256] = SHA256_DIGEST_SIZE,
	[SHA384] = SHA384_DIGEST_SIZE,
	[SHA512] = SHA512_DIGEST_SIZE,
	[SHA512224] = SHA224_DIGEST_SIZE,
	[SHA512256] = SHA256_DIGEST_SIZE,
	[TIGER128] = TIGER128_DIGEST_SIZE,
	[TIGER160] = TIGER160_DIGEST_SIZE,
	[TIGER192] = TIGER192_DIGEST_SIZE,
	[WHIRL] = WHIRL_DIGEST_SIZE,
};
int		get_command_(const char *s);
int		len_strtab(char **t);
void	ft_free_strtab(char ***tab);
int		panic_(int fd, char *str);
void	hash_process(t_hash *h);

#endif
