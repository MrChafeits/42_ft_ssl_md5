/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_common_macros.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:53:58 by callen            #+#    #+#             */
/*   Updated: 2019/04/17 13:54:58 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_COMMON_MACROS_H
# define FT_SSL_COMMON_MACROS_H

# ifndef ROL32
#  define ROL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
# endif

# ifndef ROR32
#  define ROR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
# endif

# ifndef ROL64
#  define ROL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))
# endif

# ifndef ROR64
#  define ROR64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
# endif

#endif
