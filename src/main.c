/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 22:11:16 by callen            #+#    #+#             */
/*   Updated: 2019/07/20 23:42:54 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"
#include <assert.h>

void	init_ssl_env(t_ssl_env *h)
{
	if (h->ac >= 1 && *h->av[0] == '-')
		ft_ssl_getopt(h);
	assert(h->digest >= INVAL && h->digest < MAX_MDCMD);
	h->init = g_init[h->digest];
	h->update = g_update[h->digest];
	h->final = g_final[h->digest];
	h->dgst_len = g_dgst_size[h->digest];
	hash_process(h);
}

/*
** {"dgst", "-md5", "-h", 0}
** {"md5", "-h", 0}
** before: av = {"md5", "-h", 0};
** after:  av = {"dgst", "-md5", "-h", 0};
*/

void	hashcmd_to_dgst(t_ssl_env *h)
{
	char	**nv;
	int		i;

	if (!(nv = ft_memalloc(sizeof(*nv) * (h->ac + 1))))
	{
		panic_(-3, "in " __FILE__ ": malloc failure");
		return ;
	}
	nv[h->ac] = 0;
	nv[0] = ft_strdup("dgst");
	nv[1] = ft_strjoin("-", h->av[0]);
	i = 0;
	while (++i < h->ac)
		nv[i + 1] = ft_strdup(h->av[i]);
	ft_free_strtab(&h->av);
	h->av = nv;
	h->to_free = nv;
	h->ac++;
	h->hc2dgst = 1;
}

/*
** mimic openssl and include md5 + sha256 at min, check out openssl on github
*/

static void	(*g_process[])(t_ssl_env*) = {
	std_process,
	init_ssl_env,
	std_process,
	NULL
};

int		get_std_cmd(t_ssl_env *env, const char *cmd)
{
	int		ii;

	ii = MAX_MDCMD;
	while (g_dgst_cmd[--ii] != 0)
		if (ft_strequ(cmd, g_dgst_cmd[ii]))
		{
			env->std_cmd = DGST;
			env->digest = ii;
			env->av++;
			env->ac--;
			return (DGST);
		}
	ii = MAX_STDCMD;
	while (g_std_cmd[--ii] != 0)
		if (ft_strequ(cmd, g_std_cmd[ii]))
		{
			env->std_cmd = ii;
			env->av++;
			env->ac--;
			env->digest = MD5;
			return (ii);
		}
	return (-1);
}

#define EXITCMD (ft_strequ(ln,"q")||ft_strequ("quit",ln)||ft_strequ("exit",ln))

void	shell_prompt(t_ssl_env *env)
{
	char	*ln;

	ln = 0;
	while (1)
	{
		env->shell = 1;
		env->help = 0;
		ft_putstr("ft_ssl> ");
		if ((get_next_line(0, &ln)) > 0 && !EXITCMD)
		{
			env->av = strsplit_str(ln, "\t ");
			env->to_free = env->av;
			env->ac = len_strtab(env->av);
			if (get_std_cmd(env, *env->av) == -1)
				ft_ssl_command_usage(env);
			if (g_process[env->std_cmd] != NULL)
				g_process[env->std_cmd](env);
			ft_free_strtab(&env->to_free);
		}
		else
			break ;
	}
	if (ln != NULL)
		ft_memdel((void**)&ln);
}

int		main(int ac, char **av)
{
	static t_ssl_env	env;

	if (ac == 1)
		shell_prompt(&env);
	if (ac >= 2)
	{
		env.ac = ac - 1;
		env.av = av + 1;
		if (get_std_cmd(&env, *env.av) == -1)
			ft_ssl_command_usage(&env);
		g_process[env.std_cmd](&env);
	}
}
