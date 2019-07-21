/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha256.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:13:52 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:13:55 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SHA256_H
# define FT_SHA256_H

# include <stdlib.h>
# include "ft_ssl.h"

typedef struct s_sha256	t_sha256;
struct	s_sha256
{
	t_u32	state[8];
	t_u32	datalen;
	size_t	bitlen;
	t_u8	data[64];
};
typedef struct s_sql256	t_sql256;
struct	s_sql256
{
	t_u32	a;
	t_u32	b;
	t_u32	c;
	t_u32	d;
	t_u32	e;
	t_u32	f;
	t_u32	g;
	t_u32	h;
	t_u32	t1;
	t_u32	t2;
	t_u32	m[64];
};

void	ft_sha224_init(t_sha256 *c);
void	ft_sha224_update(t_sha256 *c, t_u8 *msg, t_u32 len);
void	ft_sha224_finish(t_sha256 *c, t_u8 *md);
void	ft_sha224_final(t_sha256 *c, t_u8 *md);

void	ft_sha256_transform(t_sha256 *c, t_u8 *d);
void	ft_sha256_transform_1(t_sql256 *s, t_u8 *d);
void	ft_sha256_transform_2(t_sql256 *s);
void	ft_sha256_init(t_sha256 *c);
void	ft_sha256_update(t_sha256 *c, t_u8 *msg, t_u32 len);
void	ft_sha256_finish(t_sha256 *c, t_u8 *md);
void	ft_sha256_final(t_sha256 *c, t_u8 *md);

#endif
