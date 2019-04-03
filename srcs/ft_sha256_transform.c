/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha256_transform.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:06:18 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:06:20 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_sha256.h>

void	ft_sha256_transform(t_sha256 *c, t_u8 *d)
{
	t_sql s;

	ft_sha256_transform_1(&s, d);
	s.a = c->state[0];
	s.b = c->state[1];
	s.c = c->state[2];
	s.d = c->state[3];
	s.e = c->state[4];
	s.f = c->state[5];
	s.g = c->state[6];
	s.h = c->state[7];
	ft_sha256_transform_2(&s);
	c->state[0] += s.a;
	c->state[1] += s.b;
	c->state[2] += s.c;
	c->state[3] += s.d;
	c->state[4] += s.e;
	c->state[5] += s.f;
	c->state[6] += s.g;
	c->state[7] += s.h;
}

void	ft_sha256_transform_1(t_sql *s, t_u8 *d)
{
	register int i;
	register int j;

	i = -1;
	j = 0;
	while (++i < 16)
	{
		s->m[i] = (d[j] << 24) | (d[j + 1] << 16) |
			(d[j + 2] << 8) | (d[j + 3]);
		j += 4;
	}
	while (i < 64)
	{
		s->m[i] = S1(s->m[i - 2]) + s->m[i - 7] +
			S0(s->m[i - 15]) + s->m[i - 16];
		i++;
	}
}

void	ft_sha256_transform_2(t_sql *s)
{
	register int i;

	i = -1;
	while (++i < 64)
	{
		s->t1 = s->h + EP1(s->e) + CH(s->e, s->f, s->g) + g_k[i] + s->m[i];
		s->t2 = EP0(s->a) + MAJ(s->a, s->b, s->c);
		s->h = s->g;
		s->g = s->f;
		s->f = s->e;
		s->e = s->d + s->t1;
		s->d = s->c;
		s->c = s->b;
		s->b = s->a;
		s->a = s->t1 + s->t2;
	}
}
