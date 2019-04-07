/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_md5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:04:31 by callen            #+#    #+#             */
/*   Updated: 2019/04/06 21:50:48 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_md5.h>
#include <libft.h>
#include <stdlib.h>

static t_u8 g_padding[64] = {0x80, 0x00};

void	ft_md5_update(t_md5_ctx *context, t_u8 *input, t_u32 inputlen)
{
	register t_u32 i;
	register t_u32 index;
	register t_u32 partlen;

	index = (context->count[0] >> 3) & 0x3F;
	partlen = 64 - index;
	context->count[0] += inputlen << 3;
	if (context->count[0] < (inputlen << 3))
		context->count[1]++;
	context->count[1] += inputlen >> 29;
	if (inputlen >= partlen)
	{
		ft_memcpy(&context->buffer[index], input, partlen);
		ft_md5_transform(context->state, context->buffer);
		i = partlen;
		while (i + 64 <= inputlen)
		{
			ft_md5_transform(context->state, &input[i]);
			i += 64;
		}
		index = 0;
	}
	else
		i = 0;
	ft_memcpy(&context->buffer[index], &input[i], inputlen - i);
}

void	ft_md5_final(t_md5_ctx *context, t_u8 digest[16])
{
	register t_u32	index;
	register t_u32	padlen;
	unsigned char	bits[8];

	index = (context->count[0] >> 3) & 0x3F;
	padlen = (index < 56) ? (56 - index) : (120 - index);
	ft_md5_encode(bits, context->count, 8);
	ft_md5_update(context, g_padding, padlen);
	ft_md5_update(context, bits, 8);
	ft_md5_encode(digest, context->state, 16);
}

void	ft_md5_encode(t_u8 *output, t_u32 *input, t_u32 len)
{
	register t_u32 i;
	register t_u32 j;

	i = 0;
	j = 0;
	while (j < len)
	{
		output[j] = input[i] & 0xFF;
		output[j + 1] = (input[i] >> 8) & 0xFF;
		output[j + 2] = (input[i] >> 16) & 0xFF;
		output[j + 3] = (input[i] >> 24) & 0xFF;
		i++;
		j += 4;
	}
}

void	ft_md5_decode(t_u32 *output, t_u8 *input, t_u32 len)
{
	register t_u32	i;
	register t_u32	j;

	i = 0;
	j = 0;
	while (j < len)
	{
		/* ft_printf("i(%d) j(%d)\n",i,j); */
		output[i] = (input[j]) |
			(input[j + 1] << 8) |
			(input[j + 2] << 16) |
			(input[j + 3] << 24);
		i++;
		j += 4;
	}
}

void	ft_md5_transform(t_u32 state[4], t_u8 block[64])
{
	register t_u32	a;
	register t_u32	b;
	register t_u32	c;
	register t_u32	d;
	t_u32			x[64];

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	ft_md5_decode(x, block, 64);
	FUN;
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}
