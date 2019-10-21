/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha512_transform.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:33:42 by callen            #+#    #+#             */
/*   Updated: 2019/04/03 14:37:49 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_sha512.h>

#define SHUF R1(q->w[ii - 2]) + q->w[ii - 7] + R0(q->w[ii - 15]) + q->w[ii - 16]

void		ft_sha512256_init(t_sha512 *s)
{
	s->len = 0;
	s->h[0] = 0x22312194fc2bf72cULL;
	s->h[1] = 0x9f555fa3c84c64c2ULL;
	s->h[2] = 0x2393b86b6f53b151ULL;
	s->h[3] = 0x963877195940eabdULL;
	s->h[4] = 0x96283ee2a88effe3ULL;
	s->h[5] = 0xbe5e1e2553863992ULL;
	s->h[6] = 0x2b0199fc2c85b8aaULL;
	s->h[7] = 0x0eb72ddc81c52ca2ULL;
}

static void	processblock_1(t_sha512 *s, t_sql512 *q, const t_u8 *buf)
{
	int	ii;

	ii = -1;
	while (++ii < 80)
		if (ii < 16)
		{
			q->w[ii] = (t_u64)buf[8 * ii] << 56;
			q->w[ii] |= (t_u64)buf[8 * ii + 1] << 48;
			q->w[ii] |= (t_u64)buf[8 * ii + 2] << 40;
			q->w[ii] |= (t_u64)buf[8 * ii + 3] << 32;
			q->w[ii] |= (t_u64)buf[8 * ii + 4] << 24;
			q->w[ii] |= (t_u64)buf[8 * ii + 5] << 16;
			q->w[ii] |= (t_u64)buf[8 * ii + 6] << 8;
			q->w[ii] |= buf[8 * ii + 7];
		}
		else if (ii < 80)
			q->w[ii] = SHUF;
	q->a = s->h[0];
	q->b = s->h[1];
	q->c = s->h[2];
	q->d = s->h[3];
	q->e = s->h[4];
	q->f = s->h[5];
	q->g = s->h[6];
	q->h = s->h[7];
}

static void	processblock_2(t_sha512 *s, t_sql512 *q)
{
	int	i;

	i = -1;
	while (++i < 80)
	{
		q->t1 = q->h + S1(q->e) + CH(q->e, q->f, q->g) + g_k[i] + q->w[i];
		q->t2 = S0(q->a) + MAJ(q->a, q->b, q->c);
		q->h = q->g;
		q->g = q->f;
		q->f = q->e;
		q->e = q->d + q->t1;
		q->d = q->c;
		q->c = q->b;
		q->b = q->a;
		q->a = q->t1 + q->t2;
	}
	s->h[0] += q->a;
	s->h[1] += q->b;
	s->h[2] += q->c;
	s->h[3] += q->d;
	s->h[4] += q->e;
	s->h[5] += q->f;
	s->h[6] += q->g;
	s->h[7] += q->h;
}

void		ft_sha512_transform(t_sha512 *s, const t_u8 *buf)
{
	t_sql512	q;

	processblock_1(s, &q, buf);
	processblock_2(s, &q);
}
