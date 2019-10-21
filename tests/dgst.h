#ifndef DGST_H
# define DGST_H

# include "stack.h"
# include "evp.h"

typedef char	*FTSSL_STRING;
#define STACK_OF(type) struct stack_st_##type

struct s_dgst_cfg {
	int				argsused;
	int				debug;
	int				do_verify;
	char			*hmac_key;
	char			*keyfile;
	int				keyform;
	const EVP_MD	*m;
	STACK_OF(FTSSL_STRING)	*macopts;
	char			*mac_name;
	const EVP_MD	*md;
};

#endif
