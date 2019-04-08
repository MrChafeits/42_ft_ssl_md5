/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_whirlpool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:16:04 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:16:05 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_WHIRLPOOL_H
# define FT_WHIRLPOOL_H

# include <ft_ssl.h>
# include <stdlib.h>

typedef struct s_whrl	t_whrl_ctx;
typedef union u_h	t_h;
union	u_h
{
	t_u8	c[WHIRL_DIGEST_SIZE];
	double	q[WHIRL_DIGEST_SIZE / sizeof(double)];
};
struct	s_whrl
{
	t_h		h;
	t_u8	data[WHIRL_DIGEST_SIZE];
	t_u32	bitoff;
	size_t	bitlen[(256 / 8) / sizeof(size_t)];
};
void	ft_whrl_update(t_whrl_ctx *c, const void *_inp, size_t bytes);
void	ft_whrl_init(t_whrl_ctx *c);
void	ft_whrl_final(t_whrl_ctx *c, t_u8 *md);


#endif
