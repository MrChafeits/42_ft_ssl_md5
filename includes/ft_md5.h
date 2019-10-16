/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_md5.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:10:16 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:10:17 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MD5_H
# define FT_MD5_H

# include "ft_ssl.h"
# include <stdlib.h>

typedef struct s_md5_ctx	t_md5_ctx;
struct	s_md5_ctx
{
	t_u32	count[2];
	t_u32	state[4];
	t_u8	buffer[64];
};

void	ft_md5_init(t_md5_ctx *c);
void	ft_md5_update(t_md5_ctx *a, t_u8 *b, t_u32 c);
void	ft_md5_final(t_md5_ctx *context, t_u8 digest[16]);
void	ft_md5_transform(t_u32 state[4], t_u8 block[64]);
void	ft_md5_encode(t_u8 *o, t_u32 *i, t_u32 len);
void	ft_md5_decode(t_u32 *o, t_u8 *input, t_u32 len);

#endif
