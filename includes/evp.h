#ifndef EVP_H
# define EVP_H

# include <sys/_types/_size_t.h>

# include "stack.h"

# define EVP_MD struct s_env_md
# define EVP_MD_CTX struct s_env_md_ctx
// TODO
# define EVP_PKEY_ASN1_METHOD struct s_evp_pkey_asn1_method
# define EVP_PKEY_CTX struct s_evp_pkey_ctx
// TODO
# define ENGINE struct s_engine

union u_pkey {
	char				*ptr;
	struct rsa_st		*rsa;
	struct dsa_st		*dsa;
	struct dh_st		*dh;
	struct ec_key_st	*ec;
	struct gost_key_st	*gost;
};

struct s_evp_pkey_st {
	int							type;
	int							save_type;
	int							references;
	const EVP_PKEY_ASN1_METHOD	*ameth;
	ENGINE						*engine;
	union u_pkey				pkey;
	int							save_parameters;
	STACK_OF(X509_ATTRIBUTE)	*attributes; /* [ 0 ] */
};

struct s_env_md_ctx {
	const EVP_MD	*digest;
	ENGINE			*engine; /**/
	unsigned long	flags;
	void			*md_data;
	/* */
	/* EVP_PKEY_CTX	*pctx; */
	int				(*update)(EVP_MD_CTX *ctx, const void *data, size_t count);
};

struct s_env_md {
	int				type;
	int				pkey_type;
	int				md_size;
	unsigned long	flags;
	int				(*init)(EVP_MD_CTX *ctx);
	int				(*update)(EVP_MD_CTX *ctx, const void *data, size_t count);
	int				(*final)(EVP_MD_CTX *ctx, unsigned char *md);
	int				(*copy)(EVP_MD_CTX *to, const EVP_MD_CTX *from);
	int				(*cleanup)(EVP_MD_CTX *ctx);
	/* s_evp_sign_method *sign; */
	/* s_evp_verify_method *verify; */
	/* int required_pkey_type[5]; */
	int				block_size;
	int				ctx_size; /* how big does the ctx->md_data need to be */
	/* control function */
	int				(*md_ctrl)(EVP_MD_CTX *ctx, int cmd, int p1, void *p2);
};

#endif
