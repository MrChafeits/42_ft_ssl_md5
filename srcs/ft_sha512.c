#include <libft.h>
#include <ft_sha512.h>

static void	processblock(t_sha512 *s, const t_u8 *buf)
{
	t_sql512		q;
	register int	i;

	i = -1;
	while (++i < 16)
	{
		q.w[i] = (t_u64)buf[8 * i] << 56;
		q.w[i] |= (t_u64)buf[8 * i + 1] << 48;
		q.w[i] |= (t_u64)buf[8 * i + 2] << 40;
		q.w[i] |= (t_u64)buf[8 * i + 3] << 32;
		q.w[i] |= (t_u64)buf[8 * i + 4] << 24;
		q.w[i] |= (t_u64)buf[8 * i + 5] << 16;
		q.w[i] |= (t_u64)buf[8 * i + 6] << 8;
		q.w[i] |= buf[8 * i + 7];
	}
	while (i < 80)
	{
		q.w[i] = R1(q.w[i - 2]) + q.w[i - 7] + R0(q.w[i - 15]) + q.w[i - 16];
		i++;
	}
	q.a = s->h[0];
	q.b = s->h[1];
	q.c = s->h[2];
	q.d = s->h[3];
	q.e = s->h[4];
	q.f = s->h[5];
	q.g = s->h[6];
	q.h = s->h[7];
	i = -1;
	while (++i < 80)
	{
		q.t1 = q.h + S1(q.e) + CH(q.e, q.f, q.g) + g_k[i] + q.w[i];
		q.t2 = S0(q.a) + MAJ(q.a, q.b, q.c);
		q.h = q.g;
		q.g = q.f;
		q.f = q.e;
		q.e = q.d + q.t1;
		q.d = q.c;
		q.c = q.b;
		q.b = q.a;
		q.a = q.t1 + q.t2;
	}
	s->h[0] += q.a;
	s->h[1] += q.b;
	s->h[2] += q.c;
	s->h[3] += q.d;
	s->h[4] += q.e;
	s->h[5] += q.f;
	s->h[6] += q.g;
	s->h[7] += q.h;
}

static void	pad(t_sha512 *s)
{
	unsigned r;

	r = s->len % 128;
	s->buf[r++] = 0x80;
	if (r > 112)
	{
		ft_memset(s->buf + r, 0, 128 - r);
		r = 0;
		processblock(s, s->buf);
	}
	ft_memset(s->buf + r, 0, 120 - r);
	s->len *= 8;
	s->buf[120] = s->len >> 56;
	s->buf[121] = s->len >> 48;
	s->buf[122] = s->len >> 40;
	s->buf[123] = s->len >> 32;
	s->buf[124] = s->len >> 24;
	s->buf[125] = s->len >> 16;
	s->buf[126] = s->len >> 8;
	s->buf[127] = s->len;
	processblock(s, s->buf);
}

void	ft_sha512_init(t_sha512 *s)
{
	s->len = 0;
	s->h[0] = 0x6a09e667f3bcc908ULL;
	s->h[1] = 0xbb67ae8584caa73bULL;
	s->h[2] = 0x3c6ef372fe94f82bULL;
	s->h[3] = 0xa54ff53a5f1d36f1ULL;
	s->h[4] = 0x510e527fade682d1ULL;
	s->h[5] = 0x9b05688c2b3e6c1fULL;
	s->h[6] = 0x1f83d9abfb41bd6bULL;
	s->h[7] = 0x5be0cd19137e2179ULL;
}

void	ft_sha512_final(t_sha512 *s, t_u8 *md)
{
	register int i;

	pad(s);
	i = -1;
	while (++i < 8)
	{
		md[8 * i + 0] = s->h[i] >> 56;
		md[8 * i + 1] = s->h[i] >> 48;
		md[8 * i + 2] = s->h[i] >> 40;
		md[8 * i + 3] = s->h[i] >> 32;
		md[8 * i + 4] = s->h[i] >> 24;
		md[8 * i + 5] = s->h[i] >> 16;
		md[8 * i + 6] = s->h[i] >> 8;
		md[8 * i + 7] = s->h[i];
	}
}

void	ft_sha512_update(t_sha512 *s, const void *m, unsigned long len)
{
	const t_u8	*p = m;
	unsigned	r;

	r = s->len % 128;
	s->len += len;
	if (r)
	{
		if (len < 128 - r)
			return ((void)ft_memcpy(s->buf + r, p, len));
		ft_memcpy(s->buf + r, p, len);
		len -= 128 - r;
		p += 128 - r;
		processblock(s, s->buf);
	}
	while (len >= 128)
	{
		processblock(s, p);
		len -= 128;
		p += 128;
	}
	ft_memcpy(s->buf, p, len);
}
