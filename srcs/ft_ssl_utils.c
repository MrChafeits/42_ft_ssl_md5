/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:07:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/12 18:18:51 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"

int		panic_(int fd, char *str)
{
	if (fd == -2 && !errno)
		ft_dprintf(2, "ft_ssl: %s: Not yet implemented\n", str);
	else if (fd == -3)
		ft_dprintf(2, "ft_ssl: Error: %s\n", str);
	else
		ft_dprintf(2, "ft_ssl: %s: %s\n", str, strerror(errno));
	if (fd > 2)
		close(fd);
	return (1);
}

/*
** If s is not std_cmd, check hash_cmd
** md5 is same command as dgst
*/

t_i32v	get_command_(t_hash *h, const char *s)
{
	static char	**cmds[] = {(char**)&g_s, (char**)&g_h, (char**)&g_c};
	t_i32v		i;

	i.y = -1;
	while (++i.y < 3)
	{
		i.x = -1;
		while (cmds[i.y][++i.x])
			if (ft_strequ(cmds[i.y][i.x], s))
				break ;
		if (!cmds[i.y][i.x])
			continue ;
		else
			return ((t_i32v){i.x + (i.y == 1), i.y});
	}
	if (!cmds[i.y] || !cmds[i.y][i.x])
		ft_ssl_command_usage(h);
	return (i);
}

int		len_strtab(char **t)
{
	int i;

	i = 0;
	while (t[i])
		++i;
	return (i);
}

void	ft_free_strtab(char ***tab)
{
	int i;

	if (tab && *tab && **tab)
	{
		i = 0;
		while ((*tab)[i])
		{
			ft_memdel((void**)&(*tab)[i]);
			i++;
		}
		ft_memdel((void**)&(*tab));
	}
}

#define ECHOFMT "-p", "Echo stdin to stdout and append checksum to stdout"
#define QSTR2 "  Overrides the -r option."
#define QUIETSTR ("Quiet mode - only the checksum is printed out.")
#define QUIETFMT "-q", QUIETSTR, QSTR2

void	ft_ssl_command_help(t_hash *h)
{
	if (h->id.y == 2 || (!h->id.y && h->id.x))
	{
		ft_dprintf(2, "Usage: %s [options]\n", h->av[1]);
		ft_dprintf(2, " %-11s%s\n", "-h", "Display this summary");
	}
	else if (h->id.y == 1 || (!h->id.y && !h->id.x))
		std_dgst_help(h);
	if (h->shell == 0)
		exit(0);
}

/*
**Usage: gost [options] [file...]
**  file... files to digest (default is stdin)
** -help               Display this summary
** -c                  Print the digest with separating colons
** -r                  Print the digest in coreutils format
** -out outfile        Output to filename rather than stdout
** -passin val         Input file pass phrase source
** -sign val           Sign digest using private key
** -verify val         Verify a signature using public key
** -prverify val       Verify a signature using private key
** -signature infile   File with signature to verify
** -keyform format     Key file format (PEM or ENGINE)
** -hex                Print as hex dump
** -binary             Print in binary form
** -d                  Print debug info
** -debug              Print debug info
** -fips-fingerprint   Compute HMAC with the key used in OpenSSL-FIPS fprint
** -hmac val           Create hashed MAC with key
** -mac val            Create MAC (not necessarily HMAC)
** -sigopt val         Signature parameter in n:v form
** -macopt val         MAC algorithm parameters in n:v form or key
** -*                  Any supported digest
** -rand val           Load the file(s) into the random number generator
** -writerand outfile  Write random data to the specified file
** -engine val         Use engine e, possibly a hardware device
** -engine_impl        Also use engine given by -engine for digest operations
*/
