#include <libft.h>
#include <stdlib.h>
#include <internal.h>

#ifdef STOB
# undef STOB
#endif
#ifdef ISXDIG
# undef ISXDIG
#endif
#ifdef CTOI
# undef CTOI
#endif
#define ISXDIG(c) ((c) >= 'a' && (c) <= 'f')
#define CTOI(c) ((c) - (ISXDIG(c) ? '0' : 'a' - 10))
#define STOB(s) ((CTOI(*(s)) * 16) | (CTOI((s)[1])))

int	cmp_hash_str(t_hash *h, const char *s, t_u8 *md)
{
	register int		i;
	register t_u8		c;
	const char			*p = s;
	const char *const	b = s;

	i = -1;
	while (++i < h->dgst_len)
	{
		c = (t_u8)STOB(p);
		if (c != md[i])
			return (0);
		p += 2;
	}
	if (b != s)
		panic_(-1, "SHITS FUCKED, B AIN\'T S");
	return (1);
}

#undef CTOI
#undef STOB
#undef ISXDIG
