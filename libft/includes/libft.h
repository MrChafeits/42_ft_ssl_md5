/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 16:21:20 by callen            #+#    #+#             */
/*   Updated: 2018/11/13 20:59:16 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <wchar.h>

# define ABS(a)			((a) < 0 ? (-a) : (a))
# define MAX(a, b)		(((b) & ((a - b) >> 31)) | ((a) & (~(a - b) >> 31)))
# define MIN(a, b)		(((a) & ((a - b) >> 31)) | ((b) & (~(a - b) >> 31)))
# define ITOCB(n, b)	((n % b) + (n % b < 10 ? '0' : 'a' - 10))

# define FT_UCHAR_MAX	(unsigned char)(~0)
# define FT_CHAR_MAX	(FT_UCHAR_MAX / 2)
# define FT_CHAR_MIN	(~FT_CHAR_MAX)

# define FT_USHRT_MAX	(unsigned short)(~0)
# define FT_SHRT_MAX	(FT_USHRT_MAX / 2)
# define FT_SHRT_MIN	(~FT_SHRT_MAX)

# define FT_UINT_MAX	(~(0U))
# define FT_INT_MAX		(FT_UINT_MAX / 2)
# define FT_INT_MIN		(~(FT_INT_MAX))

# define FT_ULONG_MAX	(~(0UL))
# define FT_LONG_MAX	(FT_ULONG_MAX / 2)
# define FT_LONG_MIN	(~(FT_LONG_MAX))

# define FT_ULLONG_MAX	(~(0ULL))
# define FT_LLONG_MAX	(FT_ULLONG_MAX / 2)
# define FT_LLONG_MIN	(~(FT_LLONG_MAX))

typedef unsigned char	t_uint8;
typedef unsigned short	t_uint16;
typedef unsigned int	t_uint32;
typedef unsigned long	t_uint64;

typedef struct			s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}						t_list;

/*
** Part 1: Mandatory Functions
*/

void					*ft_memset(void *b, int c, size_t len);
void					ft_bzero(void *s, size_t n);
void					*ft_memccpy(void *dst, const void *src, int c,
									size_t n);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t len);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					*ft_memchr(const void *s, int c, size_t n);
size_t					ft_strlen(const char *s);
char					*ft_strdup(const char *s1);
char					*ft_strcpy(char *dst, const char *src);
char					*ft_strncpy(char *dest, const char *src, size_t len);
char					*ft_strcat(char *s1, const char *s2);
char					*ft_strncat(char *s1, const char *s2, size_t n);
size_t					ft_strlcat(char *dst, const char *src, size_t dstsize);
char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
char					*ft_strstr(const char *h, const char *n);
char					*ft_strnstr(const char *h, const char *n, size_t len);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_atoi(const char *str);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isascii(int c);
int						ft_isprint(int c);
int						ft_toupper(int c);
int						ft_tolower(int c);

/*
** Part 2: Additional Functions
*/

void					*ft_memalloc(size_t size);
void					ft_memdel(void **ap);
char					*ft_strnew(size_t size);
void					ft_strdel(char **as);
void					ft_strclr(char *s);
void					ft_striter(char *s, void (*f)(char *));
void					ft_striteri(char *s, void (*f)(unsigned int, char *));
char					*ft_strmap(char const *s, char (*f)(char));
char					*ft_strmapi(char const *s,
									char (*f)(unsigned int, char));
int						ft_strequ(char const *s1, char const *s2);
int						ft_strnequ(char const *s1, char const *s2, size_t n);
char					*ft_strsub(char const *s, unsigned int start,
									size_t len);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s);
char					**ft_strsplit(char const *s, char c);
char					*ft_itoa(int n);
void					ft_putstr(char const *s);
void					ft_putendl(char const *s);
void					ft_putnbr(int n);
void					ft_putchar(char c);
void					ft_putchar_fd(char c, int fd);
void					ft_putstr_fd(char const *s, int fd);
void					ft_putendl_fd(char const *s, int fd);
void					ft_putnbr_fd(int n, int ft);

/*
** Part 3: Bonus Linked List Functions
*/

t_list					*ft_lstnew(void const *content, size_t content_size);
void					ft_lstdelone(t_list **alst,
									void (*del)(void *, size_t));
void					ft_lstdel(t_list **alst, void(*del)(void *, size_t));
void					ft_lstadd(t_list **alst, t_list *new);
void					ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list					*ft_lstmap(t_list *lstm, t_list *(*f)(t_list *elem));

/*
** Part 4: Bonus Personal Functions
*/

int						ft_isblank(int c);
int						ft_isspace(int c);
int						ft_islower(int c);
int						ft_isupper(int c);
int						ft_nbrlen_base(int64_t n, int64_t b);
void					ft_lstrev(t_list **alst);
void					ft_lstadd_tail(t_list **alst, t_list *new);
int						ft_strchri(const char *s, int c);
void					ft_puterr(const char *s);
void					*ft_realloc(void *p, size_t len, size_t size);
char					*ft_strjoin_free(char *s1, char *s2, int f);
wchar_t					*ft_wstrcpy(wchar_t *dest, const wchar_t *src);
int						get_next_line(const int fd, char **line);
int						ft_printf(char *str, ...);

#endif
