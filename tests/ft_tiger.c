/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tiger.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 21:16:15 by callen            #+#    #+#             */
/*   Updated: 2019/04/07 21:16:17 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_tiger.h>
#include <libft.h>

/*
** TODO: tiger_init, tiger_update, tiger_final
**       FIX ALL TOIGER IS BROKEN, COMPARE WITH CHECKSUM IN ROOT OF REPO
*/

void	ft_tiger_init(t_tiger_ctx *ctx)
{
	ctx->len = 0;
	ctx->tiger2 = 0;
	ctx->hash[0] = 0x0123456789abcdef;
	ctx->hash[1] = 0xfedcba9876543210;
	ctx->hash[2] = 0xf096a5b4c3b2e187;
}

static void	key_schedule(t_tiger *t)
{
	t->x0 -= t->x7 ^ 0xa5a5a5a5a5a5a5a5;
	t->x1 ^= t->x0;
	t->x2 += t->x1;
	t->x3 -= t->x2 ^ ((~t->x1) << 19);
	t->x4 ^= t->x3;
	t->x5 += t->x4;
	t->x6 -= t->x5 ^ ((~t->x4) >> 23);
	t->x7 ^= t->x6;
	t->x0 += t->x7;
	t->x1 -= t->x0 ^ ((~t->x7) << 19);
	t->x2 ^= t->x1;
	t->x3 += t->x2;
	t->x4 -= t->x3 ^ ((~t->x2) >> 23);
	t->x5 ^= t->x4;
	t->x6 += t->x5;
	t->x7 -= t->x6 ^ 0x0123456789abcdef;
}

#define T1 (g_tiger_sbox)
#define T2 (g_tiger_sbox + 256)
#define T3 (g_tiger_sbox + (256 * 2))
#define T4 (g_tiger_sbox + (256 * 3))
#define RA1(c) (T1[((c) & 0xff)])
#define RA2(c) (T2[((c) >> (2 * 8)) & 0xff])
#define RA3(c) (T3[((c) >> (4 * 8)) & 0xff])
#define RA4(c) (T4[((c) >> (6 * 8)) & 0xff])
#define RB4(c) (T4[((c) >> (1 * 8)) & 0xff])
#define RB3(c) (T3[((c) >> (3 * 8)) & 0xff])
#define RB2(c) (T2[((c) >> (5 * 8)) & 0xff])
#define RB1(c) (T1[((c) >> (7 * 8)) & 0xff])
#define RNDC(c, x) c ^= x;
#define RNDA(a, c) a -= RA1(c) ^ RA2(c) ^ RA3(c) ^ RA4(c);
#define RNDB(b, c) b += RB4(c) ^ RB3(c) ^ RB2(c) ^ RB1(c);
#define RNDM(b, m) b *= m;
#define ROUND(a, b, c, x, m) RNDC(c,x); RNDA(a,c); RNDB(b,c); RNDM(b,m);
#define round(a,b,c,x,mul) \
	c ^= x; \
	a -= T1[(uint8_t)(c)] ^ \
		T2[(uint8_t)((c) >> (2 * 8))] ^ \
		T3[(uint8_t)((c) >> (4 * 8))] ^ \
		T4[(uint8_t)((c) >> (6 * 8))] ; \
	b += T4[(uint8_t)((c) >> (1 * 8))] ^ \
		T3[(uint8_t)((c) >> (3 * 8))] ^ \
		T2[(uint8_t)((c) >> (5 * 8))] ^ \
		T1[(uint8_t)((c) >> (7 * 8))]; \
	b *= mul;
#define IS_ALIGNED_64(p) (0 == (7 & ((const char*)(p) - (const char*)0)))
#define P0(t, m) ROUND(t.a, t.b, t.c, t.x0, m);
#define P1(t, m) ROUND(t.b, t.c, t.a, t.x1, m);
#define P2(t, m) ROUND(t.c, t.a, t.b, t.x2, m);
#define P3(t, m) ROUND(t.a, t.b, t.c, t.x3, m);
#define P4(t, m) ROUND(t.b, t.c, t.a, t.x4, m);
#define P5(t, m) ROUND(t.c, t.a, t.b, t.x5, m);
#define P6(t, m) ROUND(t.a, t.b, t.c, t.x6, m);
#define P7(t, m) ROUND(t.b, t.c, t.a, t.x7, m);
#define PASS(t,m) P0(t,m); P1(t,m); P2(t,m); P3(t,m); P4(t,m); P5(t,m); P6(t,m); P7(t,m);
#define pass(t,mul) \
	round(t.a,t.b,t.c,t.x0,mul) \
	round(t.b,t.c,t.a,t.x1,mul) \
	round(t.c,t.a,t.b,t.x2,mul) \
	round(t.a,t.b,t.c,t.x3,mul) \
	round(t.b,t.c,t.a,t.x4,mul) \
	round(t.c,t.a,t.b,t.x5,mul) \
	round(t.a,t.b,t.c,t.x6,mul) \
	round(t.b,t.c,t.a,t.x7,mul)

static void	ft_tiger_process_block(t_u64 state[3], t_u64 *block)
{
	t_tiger			t;
	register char	i;
	register t_u64	tmp;

	t.x0 = block[0];
	t.x1 = block[1];
	t.x2 = block[2];
	t.x3 = block[3];
	t.x4 = block[4];
	t.x5 = block[5];
	t.x6 = block[6];
	t.x7 = block[7];
	t.a = state[0];
	t.b = state[1];
	t.c = state[2];
	i = -1;
	while (++i < 3)
	{
		if (i)
			key_schedule(&t);
		pass(t, (i == 0 ? 5 : i == 1 ? 7 : 9));//((i * 2) + 5));
		tmp = t.a;
		t.a = t.c;
		t.c = t.b;
		t.b = tmp;
	}
	state[0] = t.a ^ state[0];
	state[1] = t.b - state[1];
	state[2] = t.c + state[2];
}

void	ft_tiger_update(t_tiger_ctx *ctx, const t_u8* msg, size_t size)
{
	size_t	index;
	size_t	left;

	/* fill partial block */
	ctx->len += size;
	index = (size_t)ctx->len & 63;
	if (index)
	{
		left = 64 - index;
		if (size < left)
			return ((void)ft_memcpy(ctx->message + index, msg, size));
		else
		{
			ft_memcpy(ctx->message + index, msg, left);
			ft_tiger_process_block(ctx->hash, (uint64_t*)ctx->message);
			msg += left;
			size -= left;
		}
	}
	while (size >= 64)
	{
		/* the most common case is processing of an already aligned message without copying it */
		if (IS_ALIGNED_64(msg))
			ft_tiger_process_block(ctx->hash, (uint64_t*)msg);
		else
		{
			ft_memcpy(ctx->message, msg, 64);
			ft_tiger_process_block(ctx->hash, (uint64_t*)ctx->message);
		}
		msg += 64;
		size -= 64;
	}
	/* save leftovers */
	if (size)
		ft_memcpy(ctx->message, msg, size);
}

void		ft_tiger_final(t_tiger_ctx *ctx, unsigned char result[24])
{
	t_u32	index;
	t_u64	*msg64;

	/* pad message and run for last block */
	index = (unsigned)ctx->len & 63;
	msg64 = (uint64_t*)ctx->message;
	/* append the byte 0x01 to the message */
	ctx->message[index++] = (ctx->tiger2 ? 0x80 : 0x01);
	/* if no room left in the message to store 64-bit message length */
	if (index > 56)
	{
		/* then fill the rest with zeros and process it */
		while (index < 64)
			ctx->message[index++] = 0;
		ft_tiger_process_block(ctx->hash, msg64);
		index = 0;
	}
	while (index < 56)
		ctx->message[index++] = 0;
	msg64[7] = ctx->len << 3;
	ft_tiger_process_block(ctx->hash, msg64);
	/* save result hash */
	ft_memcpy(result, &ctx->hash, 24);
}

#undef T1
#undef T2
#undef T3
#undef T4
#undef RA1
#undef RA2
#undef RA3
#undef RA4
#undef RB4
#undef RB3
#undef RB2
#undef RB1
#undef RNDC
#undef RNDA
#undef RNDB
#undef RNDM
#undef ROUND
#undef IS_ALIGNED_64
