/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha224.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:10:33 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:11:22 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SHA224_H
# define FT_SHA224_H

# include <stdlib.h>
# include <ft_ssl.h>

# define ROL(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
# define ROR(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
# define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
# define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
# define EP0(x) (ROR(x, 2) ^ ROR(x, 13) ^ ROR(x, 22))
# define EP1(x) (ROR(x, 6) ^ ROR(x, 11) ^ ROR(x, 25))
# define S0(x) (ROR(x, 7) ^ ROR(x, 18) ^ ((x) >> 3))
# define S1(x) (ROR(x, 17) ^ ROR(x, 19) ^ ((x) >> 10))

static const t_u32	g_k[64] = {
	0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U,
	0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
	0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U,
	0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
	0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU,
	0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
	0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U,
	0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
	0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U,
	0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
	0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U,
	0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
	0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U,
	0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
	0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U,
	0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U,
};

enum { SHA224_DIGEST_SIZE = 224 / 8 };

typedef struct s_sha256	t_sha256;
struct	s_sha256
{
	t_u32	state[8];
	t_u32	datalen;
	size_t	bitlen;
	t_u8	data[64];
};
typedef struct s_sql	t_sql;
struct	s_sql
{
	t_u32	a;
	t_u32	b;
	t_u32	c;
	t_u32	d;
	t_u32	e;
	t_u32	f;
	t_u32	g;
	t_u32	h;
	t_u32	t1;
	t_u32	t2;
	t_u32	m[64];
};

void	ft_sha224_transform(t_sha256 *c, t_u8 *d);
void	ft_sha224_transform_1(t_sql *s, t_u8 *d);
void	ft_sha224_transform_2(t_sql *s);
void	ft_sha224_init(t_sha256 *c);
void	ft_sha224_update(t_sha256 *c, t_u8 *msg, t_u32 len);
void	ft_sha224_finish(t_sha256 *c, t_u8 *md);
void	ft_sha224_final(t_sha256 *c, t_u8 *md);

#endif
