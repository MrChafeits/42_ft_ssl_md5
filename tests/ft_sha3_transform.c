/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha3_transform.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 20:15:04 by callen            #+#    #+#             */
/*   Updated: 2019/04/07 20:16:37 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_sha3.h>
#include <libft.h>
#include <assert.h>

/*
** TODO: all
*/

#ifdef ROL32
# undef ROL32
# define ROL32(a, o) (((a) << (o)) | ((a) >> ((32 - (o)) & 31)))
#else
# define ROL32(a, o) (((a) << (o)) | ((a) >> ((32 - (o)) & 31)))
#endif

static const t_u8	g_rhotates[5][5] = {
	{0, 1, 62, 28, 27},
	{36, 44, 6, 55, 20},
	{3, 10, 43, 25, 39},
	{41, 45, 15, 21, 8},
	{18, 2, 61, 56, 14}
};

static const uint64_t	g_iotas[] = {
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

static t_u64	rol64(t_u64 val, int offset)
{
	if (!offset)
		return (val);
	else
		return ((val << offset) | (val >> (64 - offset)));
}

#ifdef TH00
# undef TH00
#endif
#ifdef TH01
# undef TH01
#endif
#ifdef TH02
# undef TH02
#endif
#ifdef TH03
# undef TH03
#endif
#ifdef TH04
# undef TH04
#endif
#ifdef TH05
# undef TH05
#endif
#ifdef TH06
# undef TH06
#endif
#ifdef TH07
# undef TH07
#endif
#ifdef TH08
# undef TH08
#endif
#ifdef TH09
# undef TH09
#endif
#ifdef TH10
# undef TH10
#endif
#ifdef TH11
# undef TH11
#endif
#ifdef TH12
# undef TH12
#endif
#ifdef TH13
# undef TH13
#endif
#ifdef TH14
# undef TH14
#endif
#ifdef TH15
# undef TH15
#endif
#ifdef TH16
# undef TH16
#endif
#ifdef TH17
# undef TH17
#endif
#ifdef TH18
# undef TH18
#endif
#ifdef TH19
# undef TH19
#endif
#ifdef TH20
# undef TH20
#endif
#ifdef TH21
# undef TH21
#endif
#ifdef TH22
# undef TH22
#endif
#ifdef TH23
# undef TH23
#endif
#ifdef TH24
# undef TH24
#endif
#ifdef TH25
# undef TH25
#endif
#ifdef TH26
# undef TH26
#endif
#ifdef TH27
# undef TH27
#endif
#ifdef TH28
# undef TH28
#endif
#ifdef TH29
# undef TH29
#endif
#ifdef TH30
# undef TH30
#endif
#ifdef TH31
# undef TH31
#endif
#ifdef TH32
# undef TH32
#endif
#ifdef TH33
# undef TH33
#endif
#ifdef TH34
# undef TH34
#endif
#ifdef TH35
# undef TH35
#endif
#ifdef TH36
# undef TH36
#endif
#ifdef TH37
# undef TH37
#endif
#ifdef TH38
# undef TH38
#endif
#ifdef TH39
# undef TH39
#endif
#ifdef TH40
# undef TH40
#endif
#ifdef TH41
# undef TH41
#endif
#ifdef TH42
# undef TH42
#endif
#ifdef TH43
# undef TH43
#endif
#ifdef TH44
# undef TH44
#endif
#ifdef TH45
# undef TH45
#endif
#ifdef TH46
# undef TH46
#endif
#ifdef TH47
# undef TH47
#endif
#ifdef TH48
# undef TH48
#endif
#ifdef TH49
# undef TH49
#endif
#ifdef TH50
# undef TH50
#endif
#ifdef TH51
# undef TH51
#endif
#ifdef TH52
# undef TH52
#endif
#ifdef TH53
# undef TH53
#endif
#ifdef TH54
# undef TH54
#endif
#ifdef TH55
# undef TH55
#endif
#ifdef TH56
# undef TH56
#endif
#ifdef TH57
# undef TH57
#endif
#ifdef TH58
# undef TH58
#endif
#ifdef TH59
# undef TH59
#endif
#ifdef THIC
# undef THIC
#endif
#define TH00 c[0] = a[0][0] ^ a[1][0] ^ a[2][0] ^ a[3][0] ^ a[4][0];
#define TH01 TH00; c[1] = a[0][1] ^ a[1][1] ^ a[2][1] ^ a[3][1] ^ a[4][1];
#define TH02 TH01; c[2] = a[0][2] ^ a[1][2] ^ a[2][2] ^ a[3][2] ^ a[4][2];
#define TH03 TH02; c[3] = a[0][3] ^ a[1][3] ^ a[2][3] ^ a[3][3] ^ a[4][3];
#define TH04 TH03; c[4] = a[0][4] ^ a[1][4] ^ a[2][4] ^ a[3][4] ^ a[4][4];
#define TH05 TH04; d[0] = rol64(c[1], 1) ^ c[4];
#define TH06 TH05; d[1] = rol64(c[2], 1) ^ c[0];
#define TH07 TH06; d[2] = rol64(c[3], 1) ^ c[1];
#define TH08 TH07; d[3] = rol64(c[4], 1) ^ c[2];
#define TH09 TH08; d[4] = rol64(c[0], 1) ^ c[3];
#define TH10 TH09; c[0] =       a[0][0] ^ d[0];
#define TH11 TH10; c[1] = rol64(a[1][1] ^ d[1], g_rhotates[1][1]);
#define TH12 TH11; c[2] = rol64(a[2][2] ^ d[2], g_rhotates[2][2]);
#define TH13 TH12; c[3] = rol64(a[3][3] ^ d[3], g_rhotates[3][3]);
#define TH14 TH13; c[4] = rol64(a[4][4] ^ d[4], g_rhotates[4][4]);
#define TH15 TH14; r[0][0] = c[0] ^ ( c[1] | c[2]) ^ g_iotas[i];
#define TH16 TH15; r[0][1] = c[1] ^ (~c[2] | c[3]);
#define TH17 TH16; r[0][2] = c[2] ^ ( c[3] & c[4]);
#define TH18 TH17; r[0][3] = c[3] ^ ( c[4] | c[0]);
#define TH19 TH18; r[0][4] = c[4] ^ ( c[0] & c[1]);
#define TH20 TH19; c[0] = rol64(a[0][3] ^ d[3], g_rhotates[0][3]);
#define TH21 TH20; c[1] = rol64(a[1][4] ^ d[4], g_rhotates[1][4]);
#define TH22 TH21; c[2] = rol64(a[2][0] ^ d[0], g_rhotates[2][0]);
#define TH23 TH22; c[3] = rol64(a[3][1] ^ d[1], g_rhotates[3][1]);
#define TH24 TH23; c[4] = rol64(a[4][2] ^ d[2], g_rhotates[4][2]);
#define TH25 TH24; r[1][0] = c[0] ^ (c[1] |  c[2]);
#define TH26 TH25; r[1][1] = c[1] ^ (c[2] &  c[3]);
#define TH27 TH26; r[1][2] = c[2] ^ (c[3] | ~c[4]);
#define TH28 TH27; r[1][3] = c[3] ^ (c[4] |  c[0]);
#define TH29 TH28; r[1][4] = c[4] ^ (c[0] &  c[1]);
#define TH30 TH29; c[0] = rol64(a[0][1] ^ d[1], g_rhotates[0][1]);
#define TH31 TH30; c[1] = rol64(a[1][2] ^ d[2], g_rhotates[1][2]);
#define TH32 TH31; c[2] = rol64(a[2][3] ^ d[3], g_rhotates[2][3]);
#define TH33 TH32; c[3] = rol64(a[3][4] ^ d[4], g_rhotates[3][4]);
#define TH34 TH33; c[4] = rol64(a[4][0] ^ d[0], g_rhotates[4][0]);
#define TH35 TH34; r[2][0] =  c[0] ^ ( c[1] | c[2]);
#define TH36 TH35; r[2][1] =  c[1] ^ ( c[2] & c[3]);
#define TH37 TH36; r[2][2] =  c[2] ^ (~c[3] & c[4]);
#define TH38 TH37; r[2][3] = ~c[3] ^ ( c[4] | c[0]);
#define TH39 TH38; r[2][4] =  c[4] ^ ( c[0] & c[1]);
#define TH40 TH39; c[0] = rol64(a[0][4] ^ d[4], g_rhotates[0][4]);
#define TH41 TH40; c[1] = rol64(a[1][0] ^ d[0], g_rhotates[1][0]);
#define TH42 TH41; c[2] = rol64(a[2][1] ^ d[1], g_rhotates[2][1]);
#define TH43 TH42; c[3] = rol64(a[3][2] ^ d[2], g_rhotates[3][2]);
#define TH44 TH43; c[4] = rol64(a[4][3] ^ d[3], g_rhotates[4][3]);
#define TH45 TH44; r[3][0] =  c[0] ^ ( c[1] & c[2]);
#define TH46 TH45; r[3][1] =  c[1] ^ ( c[2] | c[3]);
#define TH47 TH46; r[3][2] =  c[2] ^ (~c[3] | c[4]);
#define TH48 TH47; r[3][3] = ~c[3] ^ ( c[4] & c[0]);
#define TH49 TH48; r[3][4] =  c[4] ^ ( c[0] | c[1]);
#define TH50 TH49; c[0] = rol64(a[0][2] ^ d[2], g_rhotates[0][2]);
#define TH51 TH50; c[1] = rol64(a[1][3] ^ d[3], g_rhotates[1][3]);
#define TH52 TH51; c[2] = rol64(a[2][4] ^ d[4], g_rhotates[2][4]);
#define TH53 TH52; c[3] = rol64(a[3][0] ^ d[0], g_rhotates[3][0]);
#define TH54 TH53; c[4] = rol64(a[4][1] ^ d[1], g_rhotates[4][1]);
#define TH55 TH54; r[4][0] =  c[0] ^ (~c[1] & c[2]);
#define TH56 TH55; r[4][1] = ~c[1] ^ ( c[2] | c[3]);
#define TH57 TH56; r[4][2] =  c[2] ^ ( c[3] & c[4]);
#define TH58 TH57; r[4][3] =  c[3] ^ ( c[4] | c[0]);
#define TH59 TH58; r[4][4] =  c[4] ^ ( c[0] & c[1]);
#define THIC TH59;

static void		unrolled_round(uint64_t r[5][5], uint64_t a[5][5], size_t i)
{
	uint64_t	c[5];
	uint64_t	d[5];

	assert(i < (sizeof(g_iotas) / sizeof(g_iotas[0])));
	THIC;
}

void			unrolled_keccakf1600(uint64_t a[5][5])
{
	uint64_t	t[5][5];
	size_t		i;

	a[0][1] = ~a[0][1];
	a[0][2] = ~a[0][2];
	a[1][3] = ~a[1][3];
	a[2][2] = ~a[2][2];
	a[3][2] = ~a[3][2];
	a[4][0] = ~a[4][0];
	i = 0;
	while (i < 24)
	{
		unrolled_round(t, a, i);
		unrolled_round(a, t, i + 1);
		i += 2;
	}
	a[0][1] = ~a[0][1];
	a[0][2] = ~a[0][2];
	a[1][3] = ~a[1][3];
	a[2][2] = ~a[2][2];
	a[3][2] = ~a[3][2];
	a[4][0] = ~a[4][0];
}

#undef ROL32
#undef TH00
#undef TH01
#undef TH02
#undef TH03
#undef TH04
#undef TH05
#undef TH06
#undef TH07
#undef TH08
#undef TH09
#undef TH10
#undef TH11
#undef TH12
#undef TH13
#undef TH14
#undef TH15
#undef TH16
#undef TH17
#undef TH18
#undef TH19
#undef TH20
#undef TH21
#undef TH22
#undef TH23
#undef TH24
#undef TH25
#undef TH26
#undef TH27
#undef TH28
#undef TH29
#undef TH30
#undef TH31
#undef TH32
#undef TH33
#undef TH34
#undef TH35
#undef TH36
#undef TH37
#undef TH38
#undef TH39
#undef TH40
#undef TH41
#undef TH42
#undef TH43
#undef TH44
#undef TH45
#undef TH46
#undef TH47
#undef TH48
#undef TH49
#undef TH50
#undef TH51
#undef TH52
#undef TH53
#undef TH54
#undef TH55
#undef TH56
#undef TH57
#undef TH58
#undef TH59
#undef THIC
