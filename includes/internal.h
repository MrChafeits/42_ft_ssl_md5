#ifndef INTERNAL_H
# define INTERNAL_H

int		get_command(const char *s);
int		len_strtab(const char **t);
void	ft_free_strtab(char ***tab);
int		panic_(int fd, char *str);

#endif
