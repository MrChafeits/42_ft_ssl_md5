#include <libft.h>
#include <stdlib.h>
#include <ft_ssl.h>
#include <ft_sha256.h>
#include <unistd.h>
#include <fcntl.h>

#define DGSTLEN (SHA224_DIGEST_SIZE)
#define BUFSIZE (1024 * DGSTLEN)

static inline void	ft_sha224_print(t_u8 *md)
{
	register int i;

	i = -1;
	while (++i < DGSTLEN)
		ft_printf("%02x", md[i]);
	ft_printf("\n");
}

static void			ft_dofd(int fd)
{
	t_sha256	c;
	t_u8		md[DGSTLEN];
	int			i;
	static t_u8	buf[BUFSIZE];

	ft_sha224_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break ;
		ft_sha224_update(&c, (t_u8*)(&buf), i);
	}
	ft_sha224_final(&c, md);
	ft_sha224_print(md);
}

static int			ft_sha224_fileargs(int ac, char **av)
{
	int			i;
	int			err;
	int			fd;
	struct stat	st;

	i = 0;
	err = 0;
	while (++i < ac)
	{
		fd = open(av[i], O_RDONLY);
		if (lstat(av[i], &st) || fd < 0 || !S_ISREG(st.st_mode))
		{
			if (!S_ISREG(st.st_mode))
				errno = EISDIR;
			perror(av[i]);
			err++;
			continue ;
		}
		ft_printf("SHA224(%s)= ", av[i]);
		ft_dofd(fd);
		close(fd);
	}
	return (err);
}

void				ft_sha224_process(int ac, char **av)
{
	if (ac == 2)
		ft_dofd(STDIN_FILENO);
	else
		exit(ft_sha224_fileargs(--ac, ++av));
}

#undef DGSTLEN
#undef BUFSIZE
