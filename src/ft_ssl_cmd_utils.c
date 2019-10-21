/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:47:57 by callen            #+#    #+#             */
/*   Updated: 2019/07/20 23:23:29 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"

/*
** Attempt to Mimic OpenSSL dgst command.
*/

void	std_dgst(t_ssl_env *h)
{
	h->init = g_init[h->digest];
	h->update = g_update[h->digest];
	h->final = g_final[h->digest];
	h->dgst_len = g_dgst_size[h->digest];
	hash_process(h);
}

static void	(*g_std[])(t_ssl_env*) = {
	std_nyimpl,
	[DGST] = std_dgst,
	[HELP] = std_help,
	[VER] = std_version,
};

void	std_process(t_ssl_env *h)
{
	if (h->std_cmd)
		g_std[h->std_cmd](h);
	else
		std_nyimpl(h);
}

void	ft_ssl_command_usage(t_ssl_env *h)
{
	fprintf(stderr, "DEBUG: ac(%d) optind(%d) h->digest(%d)\n",
	h->ac, optind, h->digest);
	if (h->ac && h->ac <= 2 && !h->digest)
	{
		ft_dprintf(2, "Invalid command \'%s\'; type \"help\" for a list.\n",
		h->av[!h->shell ? 1 : 0]);
	}
	else if (h->digest)
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[0], h->av[optind]);
		ft_dprintf(2, "%s: Use -h for summary.\n", h->av[0]);
	}
	else if (h->ac >= 2 && optind < h->ac)
	{
		ft_dprintf(2, "%s: Unrecognized flag %s\n", h->av[1], h->av[optind]);
		ft_dprintf(2, "%s: Use -h for summary.\n", h->av[1]);
	}
	!h->shell ? exit(EXIT_FAILURE) : 0;
}

int		get_md_opt(t_ssl_env *env, char *arg)
{
	int		ii;

	ii = 0;
	while (++ii < MAX_MDCMD)
	{
		if (ft_strequ(arg, g_dgst_cmd[ii]))
		{
			env->digest = ii;
			return (1);
		}
	}
	return (INVAL);
}

#define WHY_NORMINETTE env->quiet=1, env->bsd=0
#define YOU_ARE_TEARING_ME_APART env->strarg = env->av[++env->ii];
#define LISA if (env->ac <= env->ii){exit(panic_(-3, "-s missing argument"));}
#define WHYY do {env->string=1; LISA YOU_ARE_TEARING_ME_APART} while (0)

int		ft_ssl_getopt(t_ssl_env *env)
{
	env->ii = -1;
	while (++env->ii < env->ac && *env->av[env->ii] == '-')
	{
		if (ft_strequ(env->av[env->ii], "-c"))
			env->check = 1;
		else if (ft_strequ(env->av[env->ii], "-help"))
			ft_ssl_command_help(env);
		else if (ft_strequ(env->av[env->ii], "-p"))
			env->echo = 1;
		else if (ft_strequ(env->av[env->ii], "-q"))
			WHY_NORMINETTE;
		else if (ft_strequ(env->av[env->ii], "-r"))
			env->bsd = 1;
		else if (ft_strequ(env->av[env->ii], "-s"))
			WHYY;
		else if (get_md_opt(env, env->av[env->ii] + 1))
			;
		else
			ft_ssl_command_usage(env);
	}
	env->ac -= env->ii;
	env->av += env->ii;
	return (env->ii);
}
