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
			ft_sha224_transform(c, c->data);
			c->bitlen += 512;
			c->datalen = 0;
		}
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
		ft_sha224_transform(c, c->data);
		ft_memset(c->data, 0, 56);
	}
	ft_sha224_finish(c, md);
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
	ft_sha224_transform(c, c->data);
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

/*
static const t_u8	fillbuf[64] = {0x80, 0};
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
	c->total[0] = 0;
	c->total[1] = 0;
	c->buflen = 0;
}

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
	c->total[0] = 0;
	c->total[1] = 0;
	c->buflen = 0;
}

static void	set_uint32(char *cp, t_u32 v)
{
	ft_memcpy(cp, &v, sizeof v);
}

void	*ft_sha256_read(const t_sha256 *c, void *resbuf)
{
	register int	i;
	char			*r;

	r = resbuf;
	i = -1;
	while (++i < 8)
		set_uint32(r + i * sizeof c->state[0], __builtin_bswap32(c->state[i]));
	return (resbuf);
}

void	*ft_sha224_read(const t_sha256 *c, void *resbuf)
{
	register int	i;
	char			*r;

	r = resbuf;
	i = -1;
	while (++i < 7)
		set_uint32(r + i * sizeof c->state[0], __builtin_bswap32(c->state[i]));
	return (resbuf);
}

static void	ft_sha256_final(t_sha256 *c)
{
	size_t	bytes;
	size_t	size;

	bytes = c->buflen;
	size = (bytes < 56) ? 64 / 4 : 64 * 2 / 4;
	c->total[0] += bytes;
	if (c->total[0] < bytes)
		++c->total[1];
	set_uint32((char*)&c->buffer[size - 2], __builtin_bswap32((c->total[1] << 3) | (c->total[0] >> 29)));
	set_uint32((char*)&c->buffer[size - 1], __builtin_bswap32(c->total[0] << 3));
	ft_memcpy(&((char*)c->buffer)[bytes], fillbuf, (size - 2) * 4 - bytes);
	ft_sha256_proc_block(c->buffer, size * 4, c);
}

void	*ft_sha256_finish(t_sha256 *c, void *resbuf)
{
	ft_sha256_final(c);
	return (ft_sha256_read(c, resbuf));
}

void	*ft_sha224_finish(t_sha256 *c, void *resbuf)
{
	ft_sha256_final(c);
	return (ft_sha224_read(c, resbuf));
}

void	*ft_sha256_buffer(const char *buffer, size_t len, void *resblock)
{
	t_sha256 c;

	ft_sha256_init(&c);
	ft_sha256_proc_bytes(buffer, len, &c);
	return (ft_sha256_finish(&c, resblock));
}

void	*ft_sha224_buffer(const char *buffer, size_t len, void *resblock)
{
	t_sha256 c;

	ft_sha224_init(&c);
	ft_sha256_proc_bytes(buffer, len, &c);
	return (ft_sha224_finish(&c, resblock));
}

void	ft_sha256_proc_bytes(const void *buffer, size_t len, t_sha256 *c)
{
	size_t	left_over;
	size_t	add;

	if (c->buflen != 0)
	{
		left_over = c->buflen;
		add = 128 - left_over > len ? len : 128 - left_over;
		ft_memcpy(&((char*)c->buffer)[left_over], buffer, add);
		c->buflen += add;
		if (c->buflen > 64)
		{
			ft_sha256_proc_block(c->buffer, c->buflen & ~63, c);
			c->buflen &= 63;
			ft_memcpy(c->buffer, &((char*)c->buffer)[(left_over + add) & ~63], c->buflen);
		}
		buffer = (const char*)buffer + add;
		len -= add;
	}
	if (len >= 64)
	{
		ft_sha256_proc_block(buffer, len & ~63, c);
		buffer = (const char*)buffer + (len & ~63);
		len &= 63;
	}
	if (len > 0)
	{
		left_over = c->buflen;
		ft_memcpy(&((char*)c->buffer)[left_over], buffer, len);
		left_over += len;
		if (left_over >= 64)
		{
			ft_sha256_proc_block(c->buffer, 64, c);
			left_over -= 64;
			ft_memcpy(c->buffer, &c->buffer[16], left_over);
		}
		c->buflen = left_over;
	}
}

#define K(I) (g_sha256_const[I])
static const t_u32	g_sha256_const[64] = {
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

#define F2(A,B,C) ((A & B) | (C & (A | B)))
#define F1(E,F,G) (G ^ (E & (F ^ G)))

void	ft_sha256_proc_block(const void *buffer, size_t len, t_sha256 *c)
{
	const t_u32	*words = buffer;
	const t_u32	*endp = words + (len / sizeof(t_u32));
	t_sha		s;

	s.nwords = len / sizeof(t_u32);
	s.a = c->state[0];
	s.b = c->state[1];
	s.c = c->state[2];
	s.d = c->state[3];
	s.e = c->state[4];
	s.f = c->state[5];
	s.g = c->state[6];
	s.h = c->state[7];
	s.lolen = len;
	c->total[0] += s.lolen;
	c->total[1] += (len >> 31 >> 1) + (c->total[0] < )
}
*/
