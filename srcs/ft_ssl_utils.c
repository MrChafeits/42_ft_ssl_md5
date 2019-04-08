/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:07:28 by callen            #+#    #+#             */
/*   Updated: 2019/04/05 22:44:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal.h>
#include <libft.h>

int		panic_(int fd, char *str)
{
	ft_dprintf(2, "%s: %s: %s\n", "ft_ssl", str, strerror(errno));
	if (fd != -1)
		close(fd);
	return (-1);
}

t_i32v	get_command_(t_hash *h, const char *s)
{
	static char	**hecc[] = {(char**)&g_s, (char**)&g_h, (char**)&g_c, 0};
	t_i32v		i;

	if (ft_strequ("help", s))
		ft_ssl_help(h);
	if (!str_in_strtab(s, (const char**)&g_h))
		ft_ssl_command_usage(h);
	i.y = -1;
	while (hecc[++i.y])
	{
		i.x = -1;
		while (hecc[i.y][++i.x])
			if (ft_strequ(hecc[i.y][i.x], s))
				break ;
		if (!hecc[i.y][i.x])
			continue ;
		else
		{
			i.x++;
			break ;
		}
	}
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
	register int i;

	i = 0;
	while ((*tab)[i])
	{
		free((*tab)[i]);
		i++;
	}
	free((*tab));
}

#define ECHOFMT "-p", "Echo stdin to stdout and append checksum to stdout"
#define QSTR2 "  Overrides the -r option."
#define QUIETSTR ("Quiet mode - only the checksum is printed out.")
#define QUIETFMT "-q", QUIETSTR, QSTR2

void	ft_ssl_command_help(t_hash *h)
{
	if (h->id.y == 0)
	{
		ft_dprintf(2, "Usage: %s [options]\n", h->av[1]);
		ft_dprintf(2, " %-20s%s\n", "-h", "Display this summary");
	}
	else if (h->id.y == 1)
	{
		ft_dprintf(2, "Usage: %s [options] [file...]\n", h->av[1]);
		ft_dprintf(2, "  file... files to digest (default is stdin)\n");
		ft_dprintf(2, " %-11s%s\n", "-h", "Display this summary");
		ft_dprintf(2, " %-11s%s\n", "-c", "Verify checksum from file");
		ft_dprintf(2, " %-11s%s\n", ECHOFMT);
		ft_dprintf(2, " %-11s%s%s\n", QUIETFMT);
		ft_dprintf(2, " %-11s%s\n", "-r", "Use BSD output format");
		ft_dprintf(2, " %-11s%s\n", "-s string", "Digest string");
	}
	else if (h->id.y == 2)
	{
		ft_dprintf(2, "Usage: %s [options]\n", h->av[1]);
		ft_dprintf(2, " %-20s%s\n", "-h", "Display this summary");
	}
	exit(EXIT_SUCCESS);
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
