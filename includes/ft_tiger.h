/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tiger.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:15:36 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:15:53 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TIGER_H
# define FT_TIGER_H

# include <stdlib.h>
# include <ft_ssl.h>

typedef struct s_tiger_ctx	t_tiger_ctx;
typedef struct s_tiger	t_tiger;
struct	s_tiger_ctx
{
	t_u64	hash[3];
	t_u8	message[64];
	t_u64	len;
	int		tiger2;
};
struct	s_tiger
{
	t_u64	a;
	t_u64	A;
	t_u64	b;
	t_u64	B;
	t_u64	c;
	t_u64	C;
	t_u64	x;
	t_u64	x0;
	t_u64	x1;
	t_u64	x2;
	t_u64	x3;
	t_u64	x4;
	t_u64	x5;
	t_u64	x6;
	t_u64	x7;
};

void	ft_tiger_init(t_tiger_ctx *ctx);
void	ft_tiger_update(t_tiger_ctx *ctx, const t_u8* msg, size_t size);
void	ft_tiger_final(t_tiger_ctx *ctx, t_u8 result[24]);

#endif
