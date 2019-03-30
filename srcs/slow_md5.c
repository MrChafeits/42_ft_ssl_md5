/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slow_md5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:10:52 by callen            #+#    #+#             */
/*   Updated: 2019/03/29 22:10:53 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	ft_md5(const uint8_t *in, size_t ilen, uint8_t *dgst)
{
	uint32_t	h[4];
	uint8_t		*msg;
	uint32_t	w[16];
	uint32_t	a, b, c, d, i, f, g, tmp;
	size_t		nlen, offs;

	h[0] = 0x67452301;
	h[1] = 0xefcdab89;
	h[2] = 0x98badcfe;
	h[3] = 0x10325476;
	nlen = ilen;
	while (++nlen % 64 != 56)
		;
	msg = (uint8_t*)malloc(nlen + 8);
	ft_memcpy(msg, in, ilen);
	msg[ilen] = 0x80;
	offs = ilen;
	while (++offs < nlen)
		;
	ft_tobytes(ilen * 8, msg + nlen);
	ft_tobytes(ilen >> 29, msg + nlen + 4);
	offs = 0;
	while (offs < nlen)
	{
		for (i = 0; i < 16; i++) {
			w[i] = ft_toint(msg + offs + i * 4);
		}
		a = h[0];
		b = h[1];
		c = h[2];
		d = h[3];
		for (i = 0; i < 64; i++)
		{
			if (i < 16)
			{
				f = (b & c) | ((~b) & d);
				g = i;
			}
			else if (i < 32)
			{
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i < 48)
			{
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else
			{
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}
			tmp = d;
			d = c;
			c = b;
			b = b + RL((a + f + k[i] + w[g]), r[i]);
			a = tmp;
			offs += 64;
		}
		h[0] += a;
		h[1] += b;
		h[2] += c;
		h[3] += d;
	}
	free(msg);
	ft_tobytes(h[0], dgst);
	ft_tobytes(h[1], dgst + 4);
	ft_tobytes(h[2], dgst + 8);
	ft_tobytes(h[3], dgst + 12);
}

void	ft_slow_md5(char const *msg, size_t len)
{
	size_t	i;
	t_u8		res[16];

	for (i = 0; i < 100000; i++)
		ft_md5((uint8_t*)msg, len, res);
	for (i = 0; i < 16; i++)
		ft_printf("%2.2x", res[i]);
	ft_putchar('\n');
}
