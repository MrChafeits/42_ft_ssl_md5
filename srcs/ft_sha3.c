/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:29:40 by callen            #+#    #+#             */
/*   Updated: 2019/04/05 23:45:04 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_sha3.h>
#include <libft.h>
#include <assert.h>

/*
** TODO: all
*/

#define ROL32(a, o) (((a) << (o)) | ((a) >> ((32 - (o)) & 31)))
static t_u64	rol64(t_u64 val, int offset)
{
	t_u32	hi;
	t_u32	lo;
	t_u32	tmp;

	if (!offset)
		return (val);
	else
	{
		hi = (t_u32)(val >> 32);
		lo = (t_u32)val;
		if (offset & 1)
		{
			tmp = hi;
			offset >>= 1;
			hi = ROL32(lo, offset);
			lo = ROL32(tmp, offset + 1);
		}
		else
		{
			offset >>= 1;
			lo = ROL32(lo, offset);
			hi = ROL32(hi, offset);
		}
	}
	return (((t_u64)hi << 32) | lo);
}

static void	theta(t_u64 a[5][5])
{
	t_u64	c[5];
	t_u64	d[5];
	size_t	y;

	c[0] = a[0][0];
	c[0] = a[0][1];
	c[0] = a[0][2];
	c[0] = a[0][3];
	c[0] = a[0][4];
	y = 0;
	while (++y < 5)
	{
		c[0] ^= a[y][0];
		c[1] ^= a[y][1];
		c[2] ^= a[y][2];
		c[3] ^= a[y][3];
		c[4] ^= a[y][4];
	}
	d[0] = rol64(c[1], 1) ^ c[4];
	d[1] = rol64(c[2], 1) ^ c[0];
	d[2] = rol64(c[3], 1) ^ c[1];
	d[3] = rol64(c[4], 1) ^ c[2];
	d[4] = rol64(c[0], 1) ^ c[3];
	y = -1;
	while (++y < 5)
	{
		a[y][0] ^= d[0];
		a[y][1] ^= d[1];
		a[y][2] ^= d[2];
		a[y][3] ^= d[3];
		a[y][4] ^= d[4];
	}
}

static const t_u8 rhotates[5][5] = {
	{0, 1, 62, 28, 27},
	{36, 44, 6, 55, 20},
	{3, 10, 43, 25, 39},
	{41, 45, 15, 21, 8},
	{18, 2, 61, 56, 14}
};

static void	rho(t_u64 a[5][5])
{
	size_t y;

	y = -1;
	while (++y < 5)
	{
		a[y][0] = rol64(a[y][0], rhotates[y][0]);
		a[y][1] = rol64(a[y][1], rhotates[y][1]);
		a[y][2] = rol64(a[y][2], rhotates[y][2]);
		a[y][3] = rol64(a[y][3], rhotates[y][3]);
		a[y][4] = rol64(a[y][4], rhotates[y][4]);
	}
}

static void	pi(t_u64 a[5][5])
{
	t_u64	t[5][5];

	ft_memcpy(t, a, sizeof(t));
	a[0][0] = t[0][0];
	a[0][1] = t[1][1];
	a[0][2] = t[2][2];
	a[0][3] = t[3][3];
	a[0][4] = t[4][4];
	a[1][0] = t[0][3];
	a[1][1] = t[1][4];
	a[1][2] = t[2][0];
	a[1][3] = t[3][1];
	a[1][4] = t[4][2];
	a[2][0] = t[0][1];
	a[2][1] = t[1][2];
	a[2][2] = t[2][3];
	a[2][3] = t[3][4];
	a[2][4] = t[4][0];
	a[3][0] = t[0][4];
	a[3][1] = t[1][0];
	a[3][2] = t[2][1];
	a[3][3] = t[3][2];
	a[3][4] = t[4][3];
	a[4][0] = t[0][2];
	a[4][1] = t[1][3];
	a[4][2] = t[2][4];
	a[4][3] = t[3][0];
	a[4][4] = t[4][1];
}

static const uint64_t iotas[] = {
	0x0000000000000001ULL,
	0x0000000000008082ULL,
	0x800000000000808aULL,
	0x8000000080008000ULL,
	0x000000000000808bULL,
	0x0000000080000001ULL,
	0x8000000080008081ULL,
	0x8000000000008009ULL,
	0x000000000000008aULL,
	0x0000000000000088ULL,
	0x0000000080008009ULL,
	0x000000008000000aULL,
	0x000000008000808bULL,
	0x800000000000008bULL,
	0x8000000000008089ULL,
	0x8000000000008003ULL,
	0x8000000000008002ULL,
	0x8000000000000080ULL,
	0x000000000000800aULL,
	0x800000008000000aULL,
	0x8000000080008081ULL,
	0x8000000000008080ULL,
	0x0000000080000001ULL,
	0x8000000080008008ULL
};

static void	iota(t_u64 a[5][5], size_t i)
{
	assert(i < (sizeof(iotas) / sizeof(iotas[0])));
	a[0][0] ^= iotas[i];
}
static void	chi(t_u64 a[5][5])
{
	t_u64	c[5];
	size_t	y;

	y = -1;
	while (++y < 5)
	{
		c[0] = a[y][0] ^ (~a[y][1] & a[y][2]);
		c[1] = a[y][1] ^ (~a[y][2] & a[y][3]);
		c[2] = a[y][2] ^ (~a[y][3] & a[y][4]);
		c[3] = a[y][3] ^ (~a[y][4] & a[y][0]);
		c[4] = a[y][4] ^ (~a[y][0] & a[y][1]);
		a[y][0] = c[0];
		a[y][1] = c[1];
		a[y][2] = c[2];
		a[y][3] = c[3];
		a[y][4] = c[4];
	}
}
static void	keccakf1600(t_u64 a[5][5])
{
	size_t i;

	i = 0;
	while (i < 24)
	{
		theta(a);
		rho(a);
		pi(a);
		chi(a);
		iota(a, i);
	}
}

size_t	sha3_absorb(t_u64 a[5][5], const t_u8 *inp, size_t len, size_t r)
{
	t_u64	*a_flat;
	t_u64	ai;
	size_t	i;
	size_t	w;

	w = r / 8;
	a_flat = (t_u64*)a;
	assert(r < (25 * sizeof(a[0][0])) && (r % 8) == 0);
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
		keccakf1600(a);
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
	assert(r < (25 * sizeof(a[0][0])) && (r % 8) == 0);
	while (len)
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
			keccakf1600(a);
	}
}

void	sha3_sponge(const t_u8 *inp, size_t len, t_u8 *out, size_t d, size_t r)
{
	t_u64 a[5][5];

	ft_memset(a, 0, sizeof(a));
	sha3_absorb(a, inp, len, r);
	sha3_squeeze(a, out, d, r);
}

static const t_u8	pingors[24] = {
	10, 7, 11, 17, 18, 3,
	5, 16, 8, 21, 24, 4,
	15, 23, 19, 13, 12, 2,
	20, 14, 22, 9, 6, 1
};
static const t_u8 rhoblo[24] = {
	1, 3, 6, 10, 15, 21,
	28, 36, 45, 55, 2, 14,
	27, 41, 56, 8, 25, 43,
	62, 18, 39, 61, 20, 44
};
#define ROL(x, s) (((x) << s) | ((x) >> (64 - s)))
#define REPEAT6(e) e e e e e e
#define REPEAT24(e) REPEAT6(e e e e)
#define REPEAT5(e) e e e e e
#define FOR5(v, s, e) v = 0; REPEAT5(e; v+= s;)
static inline void	keccakf(void *state)
{
	t_u64	*a;
	t_u64	b[5];
	t_u64	t;
	t_u8	x;
	t_u8	y;

	a = (t_u64*)state;
	ft_memset(b, 0, sizeof(b));
	t = 0;
	for (int i = 0; i < 24; i++) {
		FOR5(x, 1, b[x] = 0;
			FOR5(y, 5, b[x] ^= a[x + y];))
		FOR5(x, 1,
			FOR5(y, 5,
				a[y + x] ^= b[(x + 4) % 5] ^ ROL(b[(x + 1) % 5], 1);))
		t = a[1];
		x = 0;
		REPEAT24(b[0] = a[pingors[x]];
			a[pingors[x]] = ROL(t, rhoblo[x]);
			t = b[0];
			x++;)
		FOR5(y, 5,
			FOR5(x, 1,
				b[x] = a[y + x];)
			FOR5(x, 1,
				a[y + x] = b[x] ^ ((~b[(x + 1) % 5]) & b[(x + 2) % 5]);))
		a[0] ^= iotas[i];
	}
}
void	ft_sha3_init(t_sha3_ctx *c)
{
	c->i = 0;
	c->w = 0;
	c->r = 0;
}

void	ft_sha3_update(t_sha3_ctx *c, t_u8 *msg, t_u64 len)
{
	(void)c;
	(void)msg;
	(void)len;
}

void	ft_sha3_final(t_sha3_ctx *c, t_u8 *md)
{
	(void)c;
	(void)md;
}
