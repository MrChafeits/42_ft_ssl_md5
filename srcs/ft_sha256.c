/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha256.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:06:09 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:06:10 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_sha256.h>
#include <libft.h>

void	ft_sha256_init(t_sha256 *c)
{
	c->state[0] = 0x6a09e667U;
	c->state[1] = 0xbb67ae85U;
	c->state[2] = 0x3c6ef372U;
	c->state[3] = 0xa54ff53aU;
	c->state[4] = 0x510e527fU;
	c->state[5] = 0x9b05688cU;
	c->state[6] = 0x1f83d9abU;
	c->state[7] = 0x5be0cd19U;
	c->datalen = 0;
	c->bitlen = 0;
	ft_bzero(c->data, 64);
}

void	ft_sha256_update(t_sha256 *c, t_u8 *msg, t_u32 len)
{
	register t_u32 i;

	i = 0;
	while (i < len)
	{
		c->data[c->datalen] = msg[i];
		c->datalen++;
		if (c->datalen == 64)
		{
			ft_sha256_transform(c, c->data);
			c->bitlen += 512;
			c->datalen = 0;
		}
		++i;
	}
}

void	ft_sha256_final(t_sha256 *c, t_u8 *md)
{
	register t_u32 i;

	i = c->datalen;
	if (c->datalen < 56)
	{
		c->data[i++] = 0x80;
		while (i < 56)
			c->data[i++] = 0x00;
	}
	else
	{
		c->data[i++] = 0x80;
		while (i < 64)
			c->data[i++] = 0x00;
		ft_sha256_transform(c, c->data);
		ft_memset(c->data, 0, 56);
	}
	ft_sha256_finish(c, md);
}

void	ft_sha256_finish(t_sha256 *c, t_u8 *md)
{
	register int i;

	c->bitlen += c->datalen * 8;
	c->data[63] = c->bitlen;
	c->data[62] = c->bitlen >> 8;
	c->data[61] = c->bitlen >> 16;
	c->data[60] = c->bitlen >> 24;
	c->data[59] = c->bitlen >> 32;
	c->data[58] = c->bitlen >> 40;
	c->data[57] = c->bitlen >> 48;
	c->data[56] = c->bitlen >> 56;
	ft_sha256_transform(c, c->data);
	i = -1;
	while (++i < 4)
	{
		md[i + 0] = (c->state[0] >> (24 - i * 8)) & 0xFF;
		md[i + 4] = (c->state[1] >> (24 - i * 8)) & 0xFF;
		md[i + 8] = (c->state[2] >> (24 - i * 8)) & 0xFF;
		md[i + 12] = (c->state[3] >> (24 - i * 8)) & 0xFF;
		md[i + 16] = (c->state[4] >> (24 - i * 8)) & 0xFF;
		md[i + 20] = (c->state[5] >> (24 - i * 8)) & 0xFF;
		md[i + 24] = (c->state[6] >> (24 - i * 8)) & 0xFF;
		md[i + 28] = (c->state[7] >> (24 - i * 8)) & 0xFF;
	}
}
