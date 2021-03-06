/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha384_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:06:53 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:06:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sha512.h"

void	ft_sha384_init(t_sha512 *s)
{
	s->len = 0;
	s->h[0] = 0xcbbb9d5dc1059ed8ULL;
	s->h[1] = 0x629a292a367cd507ULL;
	s->h[2] = 0x9159015a3070dd17ULL;
	s->h[3] = 0x152fecd8f70e5939ULL;
	s->h[4] = 0x67332667ffc00b31ULL;
	s->h[5] = 0x8eb44a8768581511ULL;
	s->h[6] = 0xdb0c2e0d64f98fa7ULL;
	s->h[7] = 0x47b5481dbefa4fa4ULL;
}
