/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha3.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:21:40 by callen            #+#    #+#             */
/*   Updated: 2019/04/05 23:27:05 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SHA3_H
# define FT_SHA3_H

# include <stdlib.h>
# include <ft_ssl.h>

/*
** TODO: sha3_init, sha3_update, sha3_final
** Also do I actually want to implement this
*/
typedef struct s_sha3_ctx	t_sha3_ctx;
struct	s_sha3_ctx
{
	t_u64	a[5][5];
	size_t	len;
	size_t	i;
	size_t	w;
	size_t	r;
};

void	ft_sha3_init(t_sha3_ctx *c);
void	ft_sha3_update(t_sha3_ctx *c, t_u8 *msg, t_u64 len);
void	ft_sha3_final(t_sha3_ctx *c, t_u8 *md);

#endif
