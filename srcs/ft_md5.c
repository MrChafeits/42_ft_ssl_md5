/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_md5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:04:31 by callen            #+#    #+#             */
/*   Updated: 2019/04/06 21:50:48 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_md5.h>
#include <libft.h>
#include <stdlib.h>

static t_u8 g_padding[64] = {0x80, 0x00};

void	ft_md5_update(t_md5_ctx *context, t_u8 *input, t_u32 inputlen)
{
	register t_u32 i;
	register t_u32 index;
	register t_u32 partlen;

	index = (context->count[0] >> 3) & 0x3F;
	partlen = 64 - index;
	context->count[0] += inputlen << 3;
	if (context->count[0] < (inputlen << 3))
		context->count[1]++;
	context->count[1] += inputlen >> 29;
	if (inputlen >= partlen)
	{
		ft_memcpy(&context->buffer[index], input, partlen);
		ft_md5_transform(context->state, context->buffer);
		i = partlen;
		while (i + 64 <= inputlen)
		{
			ft_md5_transform(context->state, &input[i]);
			i += 64;
		}
		index = 0;
	}
	else
		i = 0;
	ft_memcpy(&context->buffer[index], &input[i], inputlen - i);
}

void	ft_md5_final(t_md5_ctx *context, t_u8 digest[16])
{
	register t_u32	index;
	register t_u32	padlen;
	unsigned char	bits[8];

	index = (context->count[0] >> 3) & 0x3F;
	padlen = (index < 56) ? (56 - index) : (120 - index);
	ft_md5_encode(bits, context->count, 8);
	ft_md5_update(context, g_padding, padlen);
	ft_md5_update(context, bits, 8);
	ft_md5_encode(digest, context->state, 16);
}

void	ft_md5_encode(t_u8 *output, t_u32 *input, t_u32 len)
{
	register t_u32 i;
	register t_u32 j;

	i = 0;
	j = 0;
	while (j < len)
	{
		output[j] = input[i] & 0xFF;
		output[j + 1] = (input[i] >> 8) & 0xFF;
		output[j + 2] = (input[i] >> 16) & 0xFF;
		output[j + 3] = (input[i] >> 24) & 0xFF;
		i++;
		j += 4;
	}
}

void	ft_md5_decode(t_u32 *output, t_u8 *input, t_u32 len)
{
	register t_u32	i;
	register t_u32	j;

	i = 0;
	j = 0;
	while (j < len)
	{
		output[i] = (input[j]) |
			(input[j + 1] << 8) |
			(input[j + 2] << 16) |
			(input[j + 3] << 24);
		i++;
		j += 4;
	}
}

/*
** Macros for all four rounds of MD5, adapted from the md5-x86_64.pl asm script
*/
#ifdef F
# undef F
#endif
#ifdef G
# undef G
#endif
#ifdef H
# undef H
#endif
#ifdef I
# undef I
#endif
#ifdef ROL32
# undef ROL32
#endif
#ifdef FF
# undef FF
#endif
#ifdef GG
# undef GG
#endif
#ifdef HH
# undef HH
#endif
#ifdef II
# undef II
#endif
#ifdef F00
# undef F00
#endif
#ifdef F01
# undef F01
#endif
#ifdef F02
# undef F02
#endif
#ifdef F03
# undef F03
#endif
#ifdef F04
# undef F04
#endif
#ifdef F05
# undef F05
#endif
#ifdef F06
# undef F06
#endif
#ifdef F07
# undef F07
#endif
#ifdef F08
# undef F08
#endif
#ifdef F09
# undef F09
#endif
#ifdef F10
# undef F10
#endif
#ifdef F11
# undef F11
#endif
#ifdef F12
# undef F12
#endif
#ifdef F13
# undef F13
#endif
#ifdef F14
# undef F14
#endif
#ifdef F15
# undef F15
#endif
#ifdef G00
# undef G00
#endif
#ifdef G01
# undef G01
#endif
#ifdef G02
# undef G02
#endif
#ifdef G03
# undef G03
#endif
#ifdef G04
# undef G04
#endif
#ifdef G05
# undef G05
#endif
#ifdef G06
# undef G06
#endif
#ifdef G07
# undef G07
#endif
#ifdef G08
# undef G08
#endif
#ifdef G09
# undef G09
#endif
#ifdef G10
# undef G10
#endif
#ifdef G11
# undef G11
#endif
#ifdef G12
# undef G12
#endif
#ifdef G13
# undef G13
#endif
#ifdef G14
# undef G14
#endif
#ifdef G15
# undef G15
#endif
#ifdef H00
# undef H00
#endif
#ifdef H01
# undef H01
#endif
#ifdef H02
# undef H02
#endif
#ifdef H03
# undef H03
#endif
#ifdef H04
# undef H04
#endif
#ifdef H05
# undef H05
#endif
#ifdef H06
# undef H06
#endif
#ifdef H07
# undef H07
#endif
#ifdef H08
# undef H08
#endif
#ifdef H09
# undef H09
#endif
#ifdef H10
# undef H10
#endif
#ifdef H11
# undef H11
#endif
#ifdef H12
# undef H12
#endif
#ifdef H13
# undef H13
#endif
#ifdef H14
# undef H14
#endif
#ifdef H15
# undef H15
#endif
#ifdef I00
# undef I00
#endif
#ifdef I01
# undef I01
#endif
#ifdef I02
# undef I02
#endif
#ifdef I03
# undef I03
#endif
#ifdef I04
# undef I04
#endif
#ifdef I05
# undef I05
#endif
#ifdef I06
# undef I06
#endif
#ifdef I07
# undef I07
#endif
#ifdef I08
# undef I08
#endif
#ifdef I09
# undef I09
#endif
#ifdef I10
# undef I10
#endif
#ifdef I11
# undef I11
#endif
#ifdef I12
# undef I12
#endif
#ifdef I13
# undef I13
#endif
#ifdef I14
# undef I14
#endif
#ifdef I15
# undef I15
#endif
#ifdef MD5
# undef MD5
#endif
#define F(x,y,z) (((x) & (y)) | (~(x) & (z)))
#define G(x,y,z) (((x) & (z)) | ((y) & ~(z)))
#define H(x,y,z) ((x) ^ (y) ^ (z))
#define I(x,y,z) ((y) ^ ((x) | ~(z)))
#define ROL32(x,n) (((x) << (n)) | ((x) >> (32 - (n))))
#define FF(a,b,c,d,x,s,ac){a += F(b,c,d) + x + ac;a = ROL32(a,s);a += b;}
#define GG(a,b,c,d,x,s,ac){a += G(b,c,d) + x + ac;a = ROL32(a,s);a += b;}
#define HH(a,b,c,d,x,s,ac){a += H(b,c,d) + x + ac;a = ROL32(a,s);a += b;}
#define II(a,b,c,d,x,s,ac){a += I(b,c,d) + x + ac;a = ROL32(a,s);a += b;}
#define F00      FF(a, b, c, d, x[ 0],  7, 0xd76aa478);
#define F01 F00; FF(d, a, b, c, x[ 1], 12, 0xe8c7b756);
#define F02 F01; FF(c, d, a, b, x[ 2], 17, 0x242070db);
#define F03 F02; FF(b, c, d, a, x[ 3], 22, 0xc1bdceee);
#define F04 F03; FF(a, b, c, d, x[ 4],  7, 0xf57c0faf);
#define F05 F04; FF(d, a, b, c, x[ 5], 12, 0x4787c62a);
#define F06 F05; FF(c, d, a, b, x[ 6], 17, 0xa8304613);
#define F07 F06; FF(b, c, d, a, x[ 7], 22, 0xfd469501);
#define F08 F07; FF(a, b, c, d, x[ 8],  7, 0x698098d8);
#define F09 F08; FF(d, a, b, c, x[ 9], 12, 0x8b44f7af);
#define F10 F09; FF(c, d, a, b, x[10], 17, 0xffff5bb1);
#define F11 F10; FF(b, c, d, a, x[11], 22, 0x895cd7be);
#define F12 F11; FF(a, b, c, d, x[12],  7, 0x6b901122);
#define F13 F12; FF(d, a, b, c, x[13], 12, 0xfd987193);
#define F14 F13; FF(c, d, a, b, x[14], 17, 0xa679438e);
#define F15 F14; FF(b, c, d, a, x[15], 22, 0x49b40821);
#define G00 F15; GG(a, b, c, d, x[ 1],  5, 0xf61e2562);
#define G01 G00; GG(d, a, b, c, x[ 6],  9, 0xc040b340);
#define G02 G01; GG(c, d, a, b, x[11], 14, 0x265e5a51);
#define G03 G02; GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
#define G04 G03; GG(a, b, c, d, x[ 5],  5, 0xd62f105d);
#define G05 G04; GG(d, a, b, c, x[10],  9, 0x02441453);
#define G06 G05; GG(c, d, a, b, x[15], 14, 0xd8a1e681);
#define G07 G06; GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
#define G08 G07; GG(a, b, c, d, x[ 9],  5, 0x21e1cde6);
#define G09 G08; GG(d, a, b, c, x[14],  9, 0xc33707d6);
#define G10 G09; GG(c, d, a, b, x[ 3], 14, 0xf4d50d87);
#define G11 G10; GG(b, c, d, a, x[ 8], 20, 0x455a14ed);
#define G12 G11; GG(a, b, c, d, x[13],  5, 0xa9e3e905);
#define G13 G12; GG(d, a, b, c, x[ 2],  9, 0xfcefa3f8);
#define G14 G13; GG(c, d, a, b, x[ 7], 14, 0x676f02d9);
#define G15 G14; GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);
#define H00 G15; HH(a, b, c, d, x[ 5],  4, 0xfffa3942);
#define H01 H00; HH(d, a, b, c, x[ 8], 11, 0x8771f681);
#define H02 H01; HH(c, d, a, b, x[11], 16, 0x6d9d6122);
#define H03 H02; HH(b, c, d, a, x[14], 23, 0xfde5380c);
#define H04 H03; HH(a, b, c, d, x[ 1],  4, 0xa4beea44);
#define H05 H04; HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
#define H06 H05; HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60);
#define H07 H06; HH(b, c, d, a, x[10], 23, 0xbebfbc70);
#define H08 H07; HH(a, b, c, d, x[13],  4, 0x289b7ec6);
#define H09 H08; HH(d, a, b, c, x[ 0], 11, 0xeaa127fa);
#define H10 H09; HH(c, d, a, b, x[ 3], 16, 0xd4ef3085);
#define H11 H10; HH(b, c, d, a, x[ 6], 23, 0x04881d05);
#define H12 H11; HH(a, b, c, d, x[ 9],  4, 0xd9d4d039);
#define H13 H12; HH(d, a, b, c, x[12], 11, 0xe6db99e5);
#define H14 H13; HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
#define H15 H14; HH(b, c, d, a, x[ 2], 23, 0xc4ac5665);
#define I00 H15; II(a, b, c, d, x[ 0],  6, 0xf4292244);
#define I01 I00; II(d, a, b, c, x[ 7], 10, 0x432aff97);
#define I02 I01; II(c, d, a, b, x[14], 15, 0xab9423a7);
#define I03 I02; II(b, c, d, a, x[ 5], 21, 0xfc93a039);
#define I04 I03; II(a, b, c, d, x[12],  6, 0x655b59c3);
#define I05 I04; II(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
#define I06 I05; II(c, d, a, b, x[10], 15, 0xffeff47d);
#define I07 I06; II(b, c, d, a, x[ 1], 21, 0x85845dd1);
#define I08 I07; II(a, b, c, d, x[ 8],  6, 0x6fa87e4f);
#define I09 I08; II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
#define I10 I09; II(c, d, a, b, x[ 6], 15, 0xa3014314);
#define I11 I10; II(b, c, d, a, x[13], 21, 0x4e0811a1);
#define I12 I11; II(a, b, c, d, x[ 4],  6, 0xf7537e82);
#define I13 I12; II(d, a, b, c, x[11], 10, 0xbd3af235);
#define I14 I13; II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);
#define I15 I14; II(b, c, d, a, x[ 9], 21, 0xeb86d391);
#define MD5 I15;

void	ft_md5_transform(t_u32 state[4], t_u8 block[64])
{
	register t_u32	a;
	register t_u32	b;
	register t_u32	c;
	register t_u32	d;
	t_u32			x[64];

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	ft_md5_decode(x, block, 64);
	MD5;
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}
#undef F
#undef G
#undef H
#undef I
#undef ROL32
#undef FF
#undef GG
#undef HH
#undef II
#undef F00
#undef F01
#undef F02
#undef F03
#undef F04
#undef F05
#undef F06
#undef F07
#undef F08
#undef F09
#undef F10
#undef F11
#undef F12
#undef F13
#undef F14
#undef F15
#undef G00
#undef G01
#undef G02
#undef G03
#undef G04
#undef G05
#undef G06
#undef G07
#undef G08
#undef G09
#undef G10
#undef G11
#undef G12
#undef G13
#undef G14
#undef G15
#undef H00
#undef H01
#undef H02
#undef H03
#undef H04
#undef H05
#undef H06
#undef H07
#undef H08
#undef H09
#undef H10
#undef H11
#undef H12
#undef H13
#undef H14
#undef H15
#undef I00
#undef I01
#undef I02
#undef I03
#undef I04
#undef I05
#undef I06
#undef I07
#undef I08
#undef I09
#undef I10
#undef I11
#undef I12
#undef I13
#undef I14
#undef I15
#undef MD5
