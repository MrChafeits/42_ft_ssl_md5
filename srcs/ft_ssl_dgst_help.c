/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_dgst_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:36:20 by callen            #+#    #+#             */
/*   Updated: 2019/04/08 14:36:21 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal.h>
#include <libft.h>

#ifdef ECHOFMT
# undef ECHOFMT
#endif
#ifdef QUIETFMT
# undef QUIETFMT
#endif
#ifdef QSTR2
# undef QSTR2
#endif
#ifdef QUIETSTR
# undef QUIETSTR
#endif
#define ECHOFMT "-p", "Echo stdin to stdout and append checksum to stdout"
#define QSTR2 "  Overrides the -r option."
#define QUIETSTR ("Quiet mode - only the checksum is printed out.")
#define QUIETFMT "-q", QUIETSTR, QSTR2

void	std_dgst_help(t_hash *h)
{
	ft_dprintf(2, "Usage: %s [options] [file...]\n", h->av[1]);
	ft_dprintf(2, "  file... files to digest (default is stdin)\n");
	ft_dprintf(2, " %-11s%s\n", "-*", "Any supported digest");
	ft_dprintf(2, " %-11s%s\n", "-h", "Display this summary");
	ft_dprintf(2, " %-11s%s\n", "-c", "Verify checksum from file");
	ft_dprintf(2, " %-11s%s\n", ECHOFMT);
	ft_dprintf(2, " %-11s%s%s\n", QUIETFMT);
	ft_dprintf(2, " %-11s%s\n", "-r", "Use coreutils output format");
	ft_dprintf(2, " %-11s%s\n", "-s string", "Digest string");
	!h->shell ? exit(0) : 0;
}

#undef ECHOFMT
#undef QUIETFMT
#undef QSTR2
#undef QUIETSTR
