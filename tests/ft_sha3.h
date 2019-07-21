/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha3.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:21:40 by callen            #+#    #+#             */
/*   Updated: 2019/07/21 00:10:26 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SHA3_H
# define FT_SHA3_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
/* # include <ft_ssl.h> */

/*
** TODO: sha3_init, sha3_update, sha3_final
** Also do I actually want to implement this
*/

typedef unsigned char t_u8;
typedef unsigned long long t_u64;

typedef struct s_sha3_ctx	t_sha3_ctx;
struct	s_sha3_ctx
{
	t_u64	a[5][5];
	size_t	block_size;
	size_t	md_size;
	size_t	num;
	t_u8	buf[1600 / 8 - 32];
	t_u8	pad;//bleb
	t_u8	*inp;
	t_u8	*out;
	size_t	len;
	size_t	i;
	size_t	w;
	size_t	r;
};

void	unrolled_keccakf1600(uint64_t a[5][5]);
void	ft_sha3_init(t_sha3_ctx *c);
void	ft_sha3_update(t_sha3_ctx *c, t_u8 *msg, t_u64 len);
void	ft_sha3_final(t_sha3_ctx *c, t_u8 *md);

#endif
