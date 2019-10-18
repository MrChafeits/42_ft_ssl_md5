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

void	ft_ssl_command_usage(t_ssl_env *h)
{
	fprintf(stderr, "DEBUG: ac(%d) optind(%d) h->digest(%d)\n", h->ac, optind, h->digest);
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
	while (g_dgst_cmd[ii] != 0)
	{
		if (ft_strequ(arg, g_dgst_cmd[ii]))
		{
			env->digest = ii;
			return (1);
		}
	}
	return (INVAL);
}

int		ft_ssl_getopt(t_ssl_env *env)
{
	int ii;

	ii = 1;
	while (ii < env->ac)
	{
		if (ft_strequ(env->av[ii], "-c"))
			env->check = 1;
		else if (ft_strequ(env->av[ii], "-help"))
			ft_ssl_command_help(env);
		else if (ft_strequ(env->av[ii], "-p"))
			env->echo = 1;
		else if (ft_strequ(env->av[ii], "-q"))
		{
			env->quiet = 1;
			env->bsd = 0;
		}
		else if (ft_strequ(env->av[ii], "-r"))
			env->bsd = 1;
		else if (ft_strequ(env->av[ii], "-s"))
		{
			env->string = 1;
			if (env->ac <= ii)
				exit(panic_(-3, "-s missing argument"));
			env->strarg = env->av[++ii];
		}
		else if (get_md_opt(env, env->av[ii]))
			continue ;
		else
			ft_ssl_command_usage(env);
		ii++;
	}
	return (ii);
}

void	init_ssl_env(t_ssl_env *h)
{
	// int		c;

	if (h->id.y != 1)
		h->id = get_command_(h, h->av[h->shell]);
	if (h->ac >= 2)
		ft_ssl_getopt(h);
	assert(h->id.x >= 0 && h->id.x < 9);
	h->init = g_init[h->id.x];
	h->update = g_update[h->id.x];
	h->final = g_final[h->id.x];
	h->dgst_len = g_dgst_size[h->id.x];
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
/* BEGIN STRSPLIT */
#define BITOP_A(a,b) ((a)[(size_t)(b) / (8 * sizeof(*(a)))])
#define BITOP_B(a,b) ((size_t)1 << ((size_t)(b) % (8 * sizeof(*(a)))))
#define BITOP_AND(a,b) (BITOP_A(a,b) & BITOP_B(a,b))
#define BITOP_OREQ(a,b) (BITOP_A(a,b) |= BITOP_B(a,b))

char	*ft_strchrnul(const char *s, int c)
{
	c = (unsigned char)c;
	if (!c)
		return ((char*)s + ft_strlen(s));
	while (*s && *(unsigned char*)s != c)
		s++;
	return ((char*)s);
}

size_t	ft_strspn(const char *s, const char *c)
{
	const char		*a = s;
	size_t			byteset[32 / sizeof(size_t)];

	if (!c[0])
		return (0);
	if (!c[1])
	{
		while (*s == *c)
			s++;
		return (s - a);
	}
	ft_memset(byteset, 0, sizeof(byteset));
	while (*c && (BITOP_OREQ(byteset, *(unsigned char*)c)))
		c++;
	while (*s && (BITOP_AND(byteset, *(unsigned char*)s)))
		s++;
	return (s - a);
}

size_t	ft_strcspn(const char *s, const char *c)
{
	const char		*a = s;
	size_t			byteset[32 / sizeof(size_t)];

	if (!c[0] || !c[1])
		return (ft_strchrnul(s, *c) - a);
	ft_memset(byteset, 0, sizeof(byteset));
	while (*c && BITOP_OREQ(byteset, *(unsigned char*)c))
		c++;
	while (*s && !BITOP_AND(byteset, *(unsigned char*)s))
		s++;
	return (s - a);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s2;

	if ((s2 = malloc(n + 1)) == NULL)
		return (NULL);
	ft_memcpy(s2, s1, n);
	s2[n] = '\0';
	return (s2);
}

int	wordcount(const char *str, const char *sep)
{
	int total;
	int ii;

	total = 0;
	ii = 0;
	while (str[ii] != '\0')
	{
		ii += ft_strspn(str + ii, sep);
		if (str[ii] != '\0')
			total++;
		ii += ft_strcspn(str + ii, sep);
	}
	return (total);
}

char **strsplit_str(const char *str, const char *sep)
{
	char	**ret;
	int		len;
	int		words;
	int		ii;
	int		jj;

	words = wordcount(str, sep);
	ret = malloc((words + 1) * sizeof(*ret));
	if (ret == NULL)
		return (NULL);
	ii = 0;
	jj = 0;
	while (str[ii] != '\0')
	{
		ii += ft_strspn(str + ii, sep);
		if ((len = ft_strcspn(str + ii, sep)) != 0)
		{
			ret[jj++] = ft_strndup(str + ii, len);
		}
		ii += len;
	}
	ret[jj] = 0;
	return (ret);
} /* END STRSPLIT */

int		get_std_cmd(t_ssl_env *env, const char *cmd) {
	int		ii;

	ii = MAX_MDCMD;
	while (g_dgst_cmd[--ii] != 0)
		if (ft_strequ(cmd, g_dgst_cmd[ii])) {
			env->std_cmd = DGST;
			env->md_cmd = ii + 1;
			return (DGST);
		}
	ii = MAX_STDCMD;
	while (g_std_cmd[++ii] != 0)
		if (ft_strequ(cmd, g_std_cmd[ii])) {
			env->std_cmd = ii;
			return (ii);
		}
	return (-1);
}

void	shell_prompt(t_ssl_env *env)
{
	ssize_t	linelen;
	size_t	lcap=0;
	char	*ln=0;

	while (1)
	{
		env->shell = 1;
		env->help = 0;
		ft_putstr("ft_ssl> ");
		/* if ((get_next_line(0, &ln)) > 0) */
		if ((linelen = getline(&ln, &lcap, stdin)) > 0)
		{
			if (linelen > 0 && ln[linelen - 1] == '\n')
				ln[linelen - 1] = 0;
			env->av = strsplit_str(ln, "\t ");
			env->ac = len_strtab(env->av);
			if (get_std_cmd(env, *env->av) == -1)
				ft_ssl_command_usage(env);
			env->id = get_command_(env, env->av[0]);
			// if (env->id.y == 1)
			// 	hashcmd_to_dgst(h);
			if (g_process[env->id.y] != NULL)
				g_process[env->id.y](env);
			ft_free_strtab(&env->av);
		}
		else
			break ;
	}
	ln ? ft_memdel((void**)&ln) : 0;
}

int		main(int ac, char **av)
{
	static t_ssl_env	env;

	if (ac == 1)
		shell_prompt(&env);
	if (ac >= 2)
	{
		env.ac = ac;
		env.av = av;
		env.id = get_command_(&env, env.av[1]);
		g_process[env.id.y](&env);
	}
	return (0);
}
