/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:29:40 by callen            #+#    #+#             */
/*   Updated: 2019/07/21 00:39:25 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sha3.h"
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

/*
** TODO: all
*/

size_t	sha3_absorb(t_u64 a[5][5], const t_u8 *inp, size_t len, size_t r)
{
	t_u64	*a_flat;
	t_u64	ai;
	size_t	i;
	size_t	w;

	w = r / 8;
	a_flat = (t_u64*)a;
	assert(r <= (25 * sizeof(a[0][0])) && (r % 8) == 0);
	while (len >= r)
	{
		i = 0;
		while (i < w)
		{
			ai = (t_u64)inp[0] | (t_u64)inp[1] << 8 |
				(t_u64)inp[2] << 16 | (t_u64)inp[3] << 24 |
				(t_u64)inp[4] << 32 | (t_u64)inp[5] << 40 |
				(t_u64)inp[6] << 48 | (t_u64)inp[7] << 56;
			inp += 8;
			a_flat[i] ^= ai;
			i++;
		}
		unrolled_keccakf1600(a);
		len -= r;
	}
	return (len);
}

void	sha3_squeeze(t_u64 a[5][5], t_u8 *out, size_t len, size_t r)
{
	t_u64	*a_flat;
	t_u64	ai;
	size_t	i;
	size_t	w;

	w = r / 8;
	a_flat = (t_u64*)a;
	while (len != 0)
	{
		i = 0;
		while (i < w && len)
		{
			ai = a_flat[i];
			if (len < 8)
			{
				i = 0;
				while (i < len)
				{
					*out++ = (t_u8)ai;
					ai >>= 8;
					i++;
				}
				return ;
			}
			out[0] = (t_u8)(ai);
			out[1] = (t_u8)(ai >> 8);
			out[2] = (t_u8)(ai >> 16);
			out[3] = (t_u8)(ai >> 24);
			out[4] = (t_u8)(ai >> 32);
			out[5] = (t_u8)(ai >> 40);
			out[6] = (t_u8)(ai >> 48);
			out[7] = (t_u8)(ai >> 56);
			out += 8;
			len -= 8;
			i++;
		}
		if (len)
			unrolled_keccakf1600(a);
	}
}

void	sha3_sponge(const t_u8 *inp, size_t len, t_u8 *out, size_t d, size_t r)
{
	uint64_t a[5][5];

	memset(a, 0, sizeof(a));
	sha3_absorb(a, inp, len, r);
	sha3_squeeze(a, out, d, r);
}

static int	init(t_sha3_ctx *ctx, size_t bsz, size_t mdsz, t_u8 pad)
{
	if (bsz <= sizeof(ctx->buf))
	{
		memset(ctx->a, 0, sizeof(ctx->a));
		ctx->num = 0;
		ctx->block_size = bsz;
		ctx->md_size = mdsz;
		ctx->pad = pad;
		return (1);
	}
	return (0);
}

void	ft_sha3_init(t_sha3_ctx *ctx)
{
	if (!(init(ctx, (1600 - 256) / 8, (256 / 8), '\x06')))
	{
		fputs("Error in digest init.", stderr);
		exit(1);
	}
}

void	ft_sha3_update(t_sha3_ctx *ctx, t_u8 *msg, t_u64 len)
{
	size_t		bsz;
	const t_u8	*inp = msg;
	size_t		num;
	size_t		rem;

	bsz = ctx->block_size;
	if (len == 0)
		return ;//(1);
	if ((num = ctx->num) != 0)
	{
		rem = bsz - num;
		if (len < rem)
		{
			memcpy(ctx->buf + num, inp, len);
			ctx->num += len;
			return ;//(1);
		}
		memcpy(ctx->buf + num, inp, rem);
		inp += rem;
		len -= rem;
		sha3_absorb(ctx->a, ctx->buf, bsz, bsz);
		ctx->num = 0;
	}
	if (len >= bsz)
		rem = sha3_absorb(ctx->a, inp, len, bsz);
	else
		rem = len;
	if (rem)
	{
		memcpy(ctx->buf, inp + len - rem, rem);
		ctx->num = rem;
	}
}

void	ft_sha3_final(t_sha3_ctx *ctx, t_u8 *md)
{
	size_t	bsz;
	size_t	num;

	bsz = ctx->block_size;
	num = ctx->num;
	memset(ctx->buf + num, 0, bsz - num);
	ctx->buf[num] = ctx->pad;
	ctx->buf[bsz - 1] |= 0x80;
	sha3_absorb(ctx->a, ctx->buf, bsz, bsz);
	sha3_squeeze(ctx->a, md, ctx->md_size, bsz);
}
#define BUFSIZE (1024 * (256 / 8))
int		main(int argc, char *argv[])
{
	size_t				i,j=0;(void)j;
	int					fd;
	static t_u8			buf[BUFSIZE];
	static t_sha3_ctx	ctx;

	if (argc >= 2) {
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			return (1);
	} else {
		fd = STDIN_FILENO;
	}
	ft_sha3_init(&ctx);
	while (1)
	{
		if ((i = read(fd, buf, BUFSIZE)) <= 0) {
			if (++j>2400)
				break ;
		}
		ft_sha3_update(&ctx, buf, i);
	}
	ft_sha3_final(&ctx, buf);
	for (i = 0; i < ctx.md_size; i++)
		printf("%02x", ctx.buf[i]);
	printf(" \"%s\"\n", fd == 0 ? "(stdin)" : argv[1]);
}
