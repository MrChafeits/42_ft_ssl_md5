/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha512.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:07:03 by callen            #+#    #+#             */
/*   Updated: 2019/04/03 14:35:42 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_sha512.h>

static void	pad(t_sha512 *s)
{
	unsigned r;

	r = s->len % 128;
	s->buf[r++] = 0x80;
	if (r > 112)
	{
		ft_memset(s->buf + r, 0, 128 - r);
		r = 0;
		ft_sha512_transform(s, s->buf);
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
	ft_sha512_transform(s, s->buf);
}

void		ft_sha512224_init(t_sha512 *s)
{
	s->len = 0;
	s->h[0] = 0x8c3d37c819544da2ULL;
	s->h[1] = 0x73e1996689dcd4d6ULL;
	s->h[2] = 0x1dfab7ae32ff9c82ULL;
	s->h[3] = 0x679dd514582f9fcfULL;
	s->h[4] = 0x0f6d2b697bd44da8ULL;
	s->h[5] = 0x77e36f7304c48942ULL;
	s->h[6] = 0x3f9d85a86a1d36c8ULL;
	s->h[7] = 0x1112e6ad91d692a1ULL;
}

void		ft_sha512_init(t_sha512 *s)
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

void		ft_sha512_final(t_sha512 *s, t_u8 *md)
{
	int i;

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

void		ft_sha512_update(t_sha512 *s, const void *m, t_ul len)
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
		ft_sha512_transform(s, s->buf);
	}
	while (len >= 128)
	{
		ft_sha512_transform(s, p);
		len -= 128;
		p += 128;
	}
	ft_memcpy(s->buf, p, len);
}
