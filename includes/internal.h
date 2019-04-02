#ifndef INTERNAL_H
# define INTERNAL_H

int		get_command_(const char *s);
int		len_strtab(char **t);
void	ft_free_strtab(char ***tab);
int		panic_(int fd, char *str);

#endif
