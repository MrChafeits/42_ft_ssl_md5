/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 16:57:29 by callen            #+#    #+#             */
/*   Updated: 2019/03/28 16:57:44 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include "libft.h"
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>

# define MD5_LONG	unsigned int
# define MD5_CBLOCK	64
# define MD5_LBLOCK	(MD5_CBLOCK/4)
# define MD5_DIGEST_LENGTH 16
# define DATA_ORDER_IS_LITTLE_ENDIAN
# define HASH_CBLOCK MD5_CBLOCK
# define HASH_LONG MD5_LONG
# define HL2CA(l,c) (*((c)++)=(t_u8)(((l))&0xff))
# define HL2CB(l,c) (*((c)++)=(t_u8)(((l)>> 8)&0xff))
# define HL2CC(l,c) (*((c)++)=(t_u8)(((l)>>16)&0xff))
# define HL2CD(l,c) (*((c)++)=(t_u8)(((l)>>24)&0xff))
# define FHOST_L2C(l,c) (HL2CA(l,c),HL2CB(l,c),HL2CC(l,c),HL2CD(l,c))

# define HMSA(c,s) (FHOST_L2C((c)->A,(s)))
# define HMSB(c,s) (FHOST_L2C((c)->B,(s)))
# define HMSC(c,s) (FHOST_L2C((c)->C,(s)))
# define HMSD(c,s) (FHOST_L2C((c)->D,(s)))
# define FHASH2STR(c,s) (HMSA(c,s),HMSB(c,s),HMSC(c,s),HMSD(c,s))

/*
** Allowed functions:[open, close, read, write, malloc, free]
*/
typedef unsigned long	t_ul;
typedef uint64_t	t_u64;
typedef uint32_t	t_u32;
typedef uint16_t	t_u16;
typedef uint8_t		t_u8;
typedef struct s_md5_st	t_md5_st;

struct	s_md5_st
{
	MD5_LONG	A;
	MD5_LONG	B;
	MD5_LONG	C;
	MD5_LONG	D;
	MD5_LONG	Nl;
	MD5_LONG	Nh;
	MD5_LONG	data[16];
	int			num;
};

int		ft_md5_openssl(int ac, char **av);
void	ft_md5_block_data_order(t_md5_st *c, const void *data_, int num);
void	ft_md5_block_host_order(t_md5_st *c, const void *data, int num);
int		ft_md5_init(t_md5_st *c);
int		ft_md5_update(t_md5_st *c, const void *data_, t_ul len);
void	ft_md5_transform(t_md5_st *c, const t_u8 *data);
int		ft_md5_final(t_u8 *md, t_md5_st *c);

#endif
