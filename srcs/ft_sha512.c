#include <libft.h>
#include <ft_sha512.h>

static const t_u8	fillbuf[128] = {0x80, 0};

void	ft_sha512_init(t_sha512 *c)
{
	c->state[0] = U64HILO(0x6a09e667, 0xf3bcc908);
	c->state[1] = U64HILO(0xbb67ae85, 0x84caa73b);
	c->state[2] = U64HILO(0x3c6ef372, 0xfe94f82b);
	c->state[3] = U64HILO(0xa54ff53a, 0x5f1d36f1);
	c->state[4] = U64HILO(0x510e527f, 0xade682d1);
	c->state[5] = U64HILO(0x9b05688c, 0x2b3e6c1f);
	c->state[6] = U64HILO(0x1f83d9ab, 0xfb41bd6b);
	c->state[7] = U64HILO(0x5be0cd19, 0x137e2179);
	c->total[0] = U64LO(0);
	c->total[1] = c->total[0];
	c->buflen = 0;
}

void	ft_sha384_init(t_sha512 *c)
{
	c->state[0] = U64HILO(0xcbbb9d5d, 0xc1059ed8);
	c->state[1] = U64HILO(0x629a292a, 0x367cd507);
	c->state[2] = U64HILO(0x9159015a, 0x3070dd17);
	c->state[3] = U64HILO(0x152fecd8, 0xf70e5939);
	c->state[4] = U64HILO(0x67332667, 0xffc00b31);
	c->state[5] = U64HILO(0x8eb44a87, 0x68581511);
	c->state[6] = U64HILO(0xdb0c2e0d, 0x64f98fa7);
	c->state[7] = U64HILO(0x47b5481d, 0xbefa4fa4);
	c->total[0] = U64LO(0);
	c->total[1] = c->total[0];
	c->buflen = 0;
}

static void	set_uint64(char *cp, t_u64 v)
{
	ft_memcpy(cp, &v, sizeof v);
}

void	*ft_sha512_read(const t_sha512 *c, void *resbuf)
{
	register int	i;
	char			*r;

	r = resbuf;
	i = -1;
	while (++i < 8)
		set_uint64(r + i * sizeof c->state[0], SWAP(c->state[i]));
	return (resbuf);
}

void	*ft_sha384_read(const t_sha512 *c, void *resbuf)
{
	register int	i;
	char			*r;

	r = resbuf;
	i = -1;
	while (++i < 8)
		set_uint64(r + i * sizeof c->state[0], SWAP(c->state[i]));
	return (resbuf);
}

static void	ft_sha512_conclude(t_sha512 *c)
{
	size_t bytes;
	size_t size;

	bytes = c->buflen;
	size = (bytes < 112) ? 128 / 8 : 128 * 2 / 8;
	c->total[0] = U64PLUS(c->total[0], U64LO(bytes));
	if (U64LT(c->total[0], U64LO(bytes)))
		c->total[1] = U64PLUS(c->total[1], U64LO(1));
	set_uint64((char*)&c->buffer[size - 2],
		SWAP(U64OR(U64SHL(c->total[1], 3), U64SHR(c->total[0], 61))));
	set_uint64((char*)&c->buffer[size - 1],
		SWAP(U64SHL(c->total[0], 3)));
	ft_memcpy(&((char*)c->buffer)[bytes], fillbuf, (size - 2) * 8 - bytes);
	ft_sha512_proc_block(c->buffer, size * 8, c);
}

void	*ft_sha512_finish(t_sha512 *c, void *resbuf)
{
	ft_sha512_conclude(c);
	return (ft_sha512_read(c, resbuf));
}

void	*ft_sha384_finish(t_sha512 *c, void *resbuf)
{
	ft_sha512_conclude(c);
	return (ft_sha384_read(c, resbuf));
}

void	*ft_sha512_buffer(const char *buffer, size_t len, void *resblock)
{
	t_sha512 c;

	ft_sha512_init(&c);
	ft_sha512_proc_bytes(buffer, len, &c);
	return (ft_sha512_finish(&c, resblock));
}

void	*ft_sha384_buffer(const char *buffer, size_t len, void *resblock)
{
	t_sha512 c;

	ft_sha384_init(&c);
	ft_sha512_proc_bytes(buffer, len, &c);
	return (ft_sha384_finish(&c, resblock));
}

void	ft_sha512_proc_bytes(const void *buffer, size_t len, t_sha512 *c)
{
	size_t	left_over;
	size_t	add;

	if (c->buflen != 0)
	{
		left_over = c->buflen;
		add = 256 - left_over > len ? len : 256 - left_over;
		ft_memcpy(&((char*)c->buffer)[left_over], buffer, add);
		c->buflen += add;
		if (c->buflen > 128)
		{
			ft_sha512_proc_block(c->buffer, c->buflen & ~127, c);
			c->buflen &= 127;
			ft_memcpy(c->buffer, &((char*)c->buffer)[(left_over + add) & ~127],
				c->buflen);
		}
		buffer = (const char*)buffer + add;
		len -= add;
	}
	if (len >= 128)
	{
		ft_sha512_proc_block(buffer, len & ~127, c);
		buffer = (const char*)buffer + (len & ~127);
		len &= 127;
	}
	if (len > 0)
	{
		left_over = c->buflen;
		ft_memcpy(&((char*)c->buffer)[left_over], buffer, len);
		left_over += len;
		if (left_over >= 128)
		{
			ft_sha512_proc_block(c->buffer, 128, c);
			left_over -= 128;
			ft_memcpy(c->buffer, &c->buffer[16], left_over);
		}
		c->buflen = left_over;
	}
}

void	ft_sha512_proc_block(const void *buffer, size_t len, t_sha512 *c)
{
	t_u64 const	*words = buffer;
	t_u64 const	*endp = words + len / sizeof(t_u64);
	t_u64		x[16];
	t_u64		a = c->state[0];
	t_u64		b = c->state[1];
	t_u64		c = c->state[2];
	t_u64		d = c->state[3];
	t_u64		e = c->state[4];
	t_u64		f = c->state[5];
	t_u64		g = c->state[6];
	t_u64		h = c->state[7];
	t_u64		lolen = U64SIZE(len);
	int			t;

	c->total[0] = U64PLUS(c->total[0], lolen);
	c->total[1] = U64PLUS(c->total[1],
		U64PLUS(U64SIZE(len >> 31 >> 31 >> 2),
		U64LO(U64LT(c->total[0], lolen))));
	while (words < endp)
	{
		t = -1;
		while (++t < 16)
		{
			x[t] = SWAP(*words);
			words++;
		}
		R(a, b, c, d, e, f, g, h, K( 0), x[ 0]);
		R(h, a, b, c, d, e, f, g, K( 1), x[ 1]);
		R(g, h, a, b, c, d, e, f, K( 2), x[ 2]);
		R(f, g, h, a, b, c, d, e, K( 3), x[ 3]);
		R(e, f, g, h, a, b, c, d, K( 4), x[ 4]);
		R(d, e, f, g, h, a, b, c, K( 5), x[ 5]);
		R(c, d, e, f, g, h, a, b, K( 6), x[ 6]);
		R(b, c, d, e, f, g, h, a, K( 7), x[ 7]);
		R(a, b, c, d, e, f, g, h, K( 8), x[ 8]);
		R(h, a, b, c, d, e, f, g, K( 9), x[ 9]);
		R(g, h, a, b, c, d, e, f, K(10), x[10]);
		R(f, g, h, a, b, c, d, e, K(11), x[11]);
		R(e, f, g, h, a, b, c, d, K(12), x[12]);
		R(d, e, f, g, h, a, b, c, K(13), x[13]);
		R(c, d, e, f, g, h, a, b, K(14), x[14]);
		R(b, c, d, e, f, g, h, a, K(15), x[15]);
		R(a, b, c, d, e, f, g, h, K(16), M(16));
		R(h, a, b, c, d, e, f, g, K(17), M(17));
		R(g, h, a, b, c, d, e, f, K(18), M(18));
		R(f, g, h, a, b, c, d, e, K(19), M(19));
		R(e, f, g, h, a, b, c, d, K(20), M(20));
		R(d, e, f, g, h, a, b, c, K(21), M(21));
		R(c, d, e, f, g, h, a, b, K(22), M(22));
		R(b, c, d, e, f, g, h, a, K(23), M(23));
		R(a, b, c, d, e, f, g, h, K(24), M(24));
		R(h, a, b, c, d, e, f, g, K(25), M(25));
		R(g, h, a, b, c, d, e, f, K(26), M(26));
		R(f, g, h, a, b, c, d, e, K(27), M(27));
		R(e, f, g, h, a, b, c, d, K(28), M(28));
		R(d, e, f, g, h, a, b, c, K(29), M(29));
		R(c, d, e, f, g, h, a, b, K(30), M(30));
		R(b, c, d, e, f, g, h, a, K(31), M(31));
		R(a, b, c, d, e, f, g, h, K(32), M(32));
		R(h, a, b, c, d, e, f, g, K(33), M(33));
		R(g, h, a, b, c, d, e, f, K(34), M(34));
		R(f, g, h, a, b, c, d, e, K(35), M(35));
		R(e, f, g, h, a, b, c, d, K(36), M(36));
		R(d, e, f, g, h, a, b, c, K(37), M(37));
		R(c, d, e, f, g, h, a, b, K(38), M(38));
		R(b, c, d, e, f, g, h, a, K(39), M(39));
		R(a, b, c, d, e, f, g, h, K(40), M(40));
		R(h, a, b, c, d, e, f, g, K(41), M(41));
		R(g, h, a, b, c, d, e, f, K(42), M(42));
		R(f, g, h, a, b, c, d, e, K(43), M(43));
		R(e, f, g, h, a, b, c, d, K(44), M(44));
		R(d, e, f, g, h, a, b, c, K(45), M(45));
		R(c, d, e, f, g, h, a, b, K(46), M(46));
		R(b, c, d, e, f, g, h, a, K(47), M(47));
		R(a, b, c, d, e, f, g, h, K(48), M(48));
		R(h, a, b, c, d, e, f, g, K(49), M(49));
		R(g, h, a, b, c, d, e, f, K(50), M(50));
		R(f, g, h, a, b, c, d, e, K(51), M(51));
		R(e, f, g, h, a, b, c, d, K(52), M(52));
		R(d, e, f, g, h, a, b, c, K(53), M(53));
		R(c, d, e, f, g, h, a, b, K(54), M(54));
		R(b, c, d, e, f, g, h, a, K(55), M(55));
		R(a, b, c, d, e, f, g, h, K(56), M(56));
		R(h, a, b, c, d, e, f, g, K(57), M(57));
		R(g, h, a, b, c, d, e, f, K(58), M(58));
		R(f, g, h, a, b, c, d, e, K(59), M(59));
		R(e, f, g, h, a, b, c, d, K(60), M(60));
		R(d, e, f, g, h, a, b, c, K(61), M(61));
		R(c, d, e, f, g, h, a, b, K(62), M(62));
		R(b, c, d, e, f, g, h, a, K(63), M(63));
		R(a, b, c, d, e, f, g, h, K(64), M(64));
		R(h, a, b, c, d, e, f, g, K(65), M(65));
		R(g, h, a, b, c, d, e, f, K(66), M(66));
		R(f, g, h, a, b, c, d, e, K(67), M(67));
		R(e, f, g, h, a, b, c, d, K(68), M(68));
		R(d, e, f, g, h, a, b, c, K(69), M(69));
		R(c, d, e, f, g, h, a, b, K(70), M(70));
		R(b, c, d, e, f, g, h, a, K(71), M(71));
		R(a, b, c, d, e, f, g, h, K(72), M(72));
		R(h, a, b, c, d, e, f, g, K(73), M(73));
		R(g, h, a, b, c, d, e, f, K(74), M(74));
		R(f, g, h, a, b, c, d, e, K(75), M(75));
		R(e, f, g, h, a, b, c, d, K(76), M(76));
		R(d, e, f, g, h, a, b, c, K(77), M(77));
		R(c, d, e, f, g, h, a, b, K(78), M(78));
		R(b, c, d, e, f, g, h, a, K(79), M(79));
		a = c->state[0] = U64PLUS(c->state[0], a);
		b = c->state[1] = U64PLUS(c->state[1], b);
		c = c->state[2] = U64PLUS(c->state[2], c);
		d = c->state[3] = U64PLUS(c->state[3], d);
		e = c->state[4] = U64PLUS(c->state[4], e);
		f = c->state[5] = U64PLUS(c->state[5], f);
		g = c->state[6] = U64PLUS(c->state[6], g);
		h = c->state[7] = U64PLUS(c->state[7], h);
	}
}
