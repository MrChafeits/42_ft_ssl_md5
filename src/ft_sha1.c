/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:04:53 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:04:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_ssl.h"
#include "ft_sha1.h"

void		ft_sha1_transform(t_s1ctx *c, t_u8 *d)
{
	t_sql1	s;

	ft_sha1_transform_1(c, d, &s);
	ft_sha1_transform_2(c, &s);
	ft_sha1_transform_3(c, &s);
	c->state[0] += s.a;
	c->state[1] += s.b;
	c->state[2] += s.c;
	c->state[3] += s.d;
	c->state[4] += s.e;
}

void		ft_sha1_init(t_s1ctx *c)
{
	c->datalen = 0;
	c->bitlen = 0;
	c->state[0] = 0x67452301U;
	c->state[1] = 0xefcdab89U;
	c->state[2] = 0x98badcfeU;
	c->state[3] = 0x10325476U;
	c->state[4] = 0xc3d2e1f0U;
	c->k[0] = 0x5a827999U;
	c->k[1] = 0x6ed9eba1U;
	c->k[2] = 0x8f1bbcdcU;
	c->k[3] = 0xca62c1d6U;
}

void		ft_sha1_update(t_s1ctx *c, t_u8 *d, t_u64 l)
{
	t_u64	i;

	i = 0;
	while (i < l)
	{
		c->data[c->datalen] = d[i];
		c->datalen++;
		if (c->datalen == 64)
		{
			ft_sha1_transform(c, c->data);
			c->bitlen += 512;
			c->datalen = 0;
		}
		++i;
	}
}

static void	ft_sha1_almost(t_s1ctx *c)
{
	int i;

	i = c->datalen;
	if (c->datalen < 56)
	{
		c->data[i++] = 0x80;
		while (i < 56)
			c->data[i++] = 0;
	}
	else
	{
		c->data[i++] = 0x80;
		while (i < 64)
			c->data[i++] = 0;
		ft_sha1_transform(c, c->data);
		ft_memset(c->data, 0, 56);
	}
}

void		ft_sha1_final(t_s1ctx *c, t_u8 *md)
{
	int i;

	ft_sha1_almost(c);
	c->bitlen += c->datalen * 8;
	c->data[63] = c->bitlen;
	c->data[62] = c->bitlen >> 8;
	c->data[61] = c->bitlen >> 16;
	c->data[60] = c->bitlen >> 24;
	c->data[59] = c->bitlen >> 32;
	c->data[58] = c->bitlen >> 40;
	c->data[57] = c->bitlen >> 48;
	c->data[56] = c->bitlen >> 56;
	ft_sha1_transform(c, c->data);
	i = -1;
	while (++i < 4)
	{
		md[i + 0] = (c->state[0] >> (24 - i * 8)) & 0xFF;
		md[i + 4] = (c->state[1] >> (24 - i * 8)) & 0xFF;
		md[i + 8] = (c->state[2] >> (24 - i * 8)) & 0xFF;
		md[i + 12] = (c->state[3] >> (24 - i * 8)) & 0xFF;
		md[i + 16] = (c->state[4] >> (24 - i * 8)) & 0xFF;
	}
}
