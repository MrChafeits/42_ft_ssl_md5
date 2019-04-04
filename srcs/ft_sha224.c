/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha224.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:05:33 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:05:35 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_sha256.h>

void	ft_sha224_init(t_sha256 *c)
{
	c->state[0] = 0xc1059ed8U;
	c->state[1] = 0x367cd507U;
	c->state[2] = 0x3070dd17U;
	c->state[3] = 0xf70e5939U;
	c->state[4] = 0xffc00b31U;
	c->state[5] = 0x68581511U;
	c->state[6] = 0x64f98fa7U;
	c->state[7] = 0xbefa4fa4U;
	c->datalen = 0;
	c->bitlen = 0;
	ft_bzero(c->data, 64);
}

void	ft_sha224_update(t_sha256 *c, t_u8 *msg, t_u32 len)
{
	register t_u32 i;

	i = -1;
	while (++i < len)
	{
		c->data[c->datalen] = msg[i];
		c->datalen++;
		if (c->datalen == 64)
		{
			ft_sha256_transform(c, c->data);
			c->bitlen += 512;
			c->datalen = 0;
		}
	}
}

void	ft_sha224_finish(t_sha256 *c, t_u8 *md)
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
	}
}

void	ft_sha224_final(t_sha256 *c, t_u8 *md)
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
	ft_sha224_finish(c, md);
}
