/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha512_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:06:53 by callen            #+#    #+#             */
/*   Updated: 2019/04/18 20:31:44 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sha512.h"

/*
** Initial values for SHA-384 as specified in FIPS-180-4, describing them as
** "... the first 64 bits of the fractional parts of the square roots of the
** ninth through sixteenth prime numbers."
*/

void	ft_sha384_init(t_sha512_ctx *s)
{
	s->len = 0;
	s->h[0] = 0xcbbb9d5dc1059ed8ULL;
	s->h[1] = 0x629a292a367cd507ULL;
	s->h[2] = 0x9159015a3070dd17ULL;
	s->h[3] = 0x152fecd8f70e5939ULL;
	s->h[4] = 0x67332667ffc00b31ULL;
	s->h[5] = 0x8eb44a8768581511ULL;
	s->h[6] = 0xdb0c2e0d64f98fa7ULL;
	s->h[7] = 0x47b5481dbefa4fa4ULL;
}

void	ft_sha512_init(t_sha512_ctx *s)
{
	s->len = 0;
	s->h[0] = 0x6a09e667f3bcc908ULL;
	s->h[1] = 0xbb67ae8584caa73bULL;
	s->h[2] = 0x3c6ef372fe94f82bULL;
	s->h[3] = 0xa54ff53a5f1d36f1ULL;
	s->h[4] = 0x510e527fade682d1ULL;
	s->h[5] = 0x9b05688c2b3e6c1fULL;
	s->h[6] = 0x1f83d9abfb41bd6bULL;
	s->h[7] = 0x5be0cd19137e2179ULL;
}

void	ft_sha512256_init(t_sha512_ctx *s)
{
	s->len = 0;
	s->h[0] = 0x22312194fc2bf72cULL;
	s->h[1] = 0x9f555fa3c84c64c2ULL;
	s->h[2] = 0x2393b86b6f53b151ULL;
	s->h[3] = 0x963877195940eabdULL;
	s->h[4] = 0x96283ee2a88effe3ULL;
	s->h[5] = 0xbe5e1e2553863992ULL;
	s->h[6] = 0x2b0199fc2c85b8aaULL;
	s->h[7] = 0x0eb72ddc81c52ca2ULL;
}

void	ft_sha512224_init(t_sha512_ctx *s)
{
	s->len = 0;
	s->h[0] = 0x8c3d37c819544da2ULL;
	s->h[1] = 0x73e1996689dcd4d6ULL;
	s->h[2] = 0x1dfab7ae32ff9c82ULL;
	s->h[3] = 0x679dd514582f9fcfULL;
	s->h[4] = 0x0f6d2b697bd44da8ULL;
	s->h[5] = 0x77e36f7304c48942ULL;
	s->h[6] = 0x3f9d85a86a1d36c8ULL;
	s->h[7] = 0x1112e6ad91d692a1ULL;
}
