/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha1.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:10:24 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:10:25 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SHA1_H
# define FT_SHA1_H

# include <stdlib.h>
# include <ft_ssl.h>

typedef struct s_s	t_s1ctx;
struct	s_s
{
	t_u8	data[64];
	t_u32	datalen;
	t_u64	bitlen;
	t_u32	state[5];
	t_u32	k[4];
};

typedef struct s_sql1	t_sql1;
struct	s_sql1
{
	t_u32	a;
	t_u32	b;
	t_u32	c;
	t_u32	d;
	t_u32	e;
	t_u32	t;
	t_u32	m[80];
};

void	ft_sha1_init(t_s1ctx *ctx);
void	ft_sha1_update(t_s1ctx *ctx, t_u8 *data, t_u64 len);
void	ft_sha1_final(t_s1ctx *ctx, t_u8 *hash);
void	ft_sha1_transform_1(t_s1ctx *ctx, t_u8 *data, t_sql1 *s);
void	ft_sha1_transform_2(t_s1ctx *ctx, t_sql1 *s);
void	ft_sha1_transform_3(t_s1ctx *ctx, t_sql1 *s);

#endif
