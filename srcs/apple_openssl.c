#include "ft_ssl.h"

/*
** Apple OpenSSL
** https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md32_common.h.auto.html
** https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5.c.auto.html
** https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5_dgst.c.auto.html
** https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5.h.auto.html
** https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5_locl.h.auto.html
** https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/
*/


#define HOST_c2l(c,l)	(l =(((unsigned int)(*((c)++)))),		\
			l|=(((unsigned int)(*((c)++)))<< 8),		\
			l|=(((unsigned int)(*((c)++)))<<16),		\
			l|=(((unsigned int)(*((c)++)))<<24),		\
			l)
/* static inline void fhost_c2l(const t_u8 **c, t_u64 *l)
{
	*l = (((unsigned int)(*(*c)++)));
	*l |= (((unsigned int)(*((*c)++))) << 8);
	*l |= (((unsigned int)(*((*c)++))) << 16);
	*l |= (((unsigned int)(*((*c)++))) << 24);
} */
#define HOST_p_c2l(c,l,n)	{					\
			switch (n) {					\
			case 0: l =((unsigned int)(*((c)++)));		\
			case 1: l|=((unsigned int)(*((c)++)))<< 8;	\
			case 2: l|=((unsigned int)(*((c)++)))<<16;	\
			case 3: l|=((unsigned int)(*((c)++)))<<24;	\
				} }
/* static inline void fhost_p_c2l(const t_u8 **c, t_u64 *l, int n)
{
	if (n == 0)
		*l = ((unsigned int)(*((*c)++)));
	if (n <= 1)
		*l |= ((unsigned int)(*((*c)++))) << 8;
	if (n <= 2)
		*l |= ((unsigned int)(*((*c)++))) << 16;
	if (n <= 3)
		*l |= ((unsigned int)(*((*c)++))) << 24;
} */
#define HOST_p_c2l_p(c,l,sc,len) {					\
			switch (sc) {					\
			case 0: l =((unsigned int)(*((c)++)));		\
				if (--len == 0) break;			\
			case 1: l|=((unsigned int)(*((c)++)))<< 8;	\
				if (--len == 0) break;			\
			case 2: l|=((unsigned int)(*((c)++)))<<16;	\
				} }
/* static inline void fhost_p_c2l_p(const t_u8 **c, t_u64 *l, int sc, t_ul *len)
{
	if (sc == 0)
	{
		*l = ((unsigned int)(*((*c)++)));
		if (--*len == 0)
			return ;
	}
	if (sc <= 1)
	{
		*l |= ((unsigned int)(*((*c)++))) << 8;
		if (--*len == 0)
			return ;
	}
	if (sc <= 2)
		*l |= ((unsigned int)(*((*c)++))) << 16;
} */
/* NOTE the pointer is not incremented at the end of this */
#define HOST_c2l_p(c,l,n)	{					\
			l=0; (c)+=n;					\
			switch (n) {					\
			case 3: l =((unsigned int)(*(--(c))))<<16;	\
			case 2: l|=((unsigned int)(*(--(c))))<< 8;	\
			case 1: l|=((unsigned int)(*(--(c))));		\
				} }
/* static inline void fhost_c2l_p(const t_u8 **c, t_u64 *l, int n)
{
	*l = 0;
	(*c) += n;
	if (n == 3)
		*l = ((unsigned int)(*(--(*c)))) << 16;
	if (n >= 2)
		*l |= ((unsigned int)(*(--(*c)))) << 8;
	if (n >= 1)
		*l |= ((unsigned int)(*(--(*c))));
} */
#define HOST_l2c(l,c)	(*((c)++)=(unsigned char)(((l))&0xff),	\
			*((c)++)=(unsigned char)(((l)>> 8)&0xff),	\
			*((c)++)=(unsigned char)(((l)>>16)&0xff),	\
			*((c)++)=(unsigned char)(((l)>>24)&0xff),	\
			l)

#define HASH_CTX t_md5_st
#define HASH_UPDATE ft_md5_update
#define HASH_TRANSFORM ft_md5_transform
#define HASH_FINAL ft_md5_final
#define	HASH_MAKE_STRING(c,s)	do {	\
	unsigned int ll;		\
	ll=(c)->A; HOST_l2c(ll,(s));	\
	ll=(c)->B; HOST_l2c(ll,(s));	\
	ll=(c)->C; HOST_l2c(ll,(s));	\
	ll=(c)->D; HOST_l2c(ll,(s));	\
	} while (0)
#define HASH_BLOCK_HOST_ORDER ft_md5_block_host_order
#define HASH_BLOCK_DATA_ORDER ft_md5_block_data_order
#define	F(b,c,d)	((((c) ^ (d)) & (b)) ^ (d))
#define	G(b,c,d)	((((b) ^ (c)) & (d)) ^ (c))
#define	H(b,c,d)	((b) ^ (c) ^ (d))
#define	I(b,c,d)	(((~(d)) | (b)) ^ (c))
#define ROTATE(a,n)	(((a) << (n)) | (((a) & 0xffffffff) >> (32 - (n))))
#define R0(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+F((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R1(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+G((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R2(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+H((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R3(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+I((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };
#define DATA_ORDER_IS_LITTLE_ENDIAN
#if !defined(DATA_ORDER_IS_BIG_ENDIAN) && !defined(DATA_ORDER_IS_LITTLE_ENDIAN)
#error "DATA_ORDER must be defined!"
#endif

#ifndef HASH_CBLOCK
#error "HASH_CBLOCK must be defined!"
#endif
#ifndef HASH_LONG
#error "HASH_LONG must be defined!"
#endif
#ifndef HASH_CTX
#error "HASH_CTX must be defined!"
#endif

#ifndef HASH_UPDATE
#error "HASH_UPDATE must be defined!"
#endif
#ifndef HASH_TRANSFORM
#error "HASH_TRANSFORM must be defined!"
#endif
#ifndef HASH_FINAL
#error "HASH_FINAL must be defined!"
#endif

#ifndef HASH_BLOCK_HOST_ORDER
#error "HASH_BLOCK_HOST_ORDER must be defined!"
#endif

#ifndef HASH_LBLOCK
#define HASH_LBLOCK	(HASH_CBLOCK/4)
#endif
#define L_ENDIAN
#ifndef HASH_LONG_LOG2
#define HASH_LONG_LOG2	2
#endif
#if defined(B_ENDIAN)
#elif defined(L_ENDIAN)
#  if defined(DATA_ORDER_IS_LITTLE_ENDIAN)
#    if !defined(HASH_BLOCK_DATA_ORDER_ALIGNED) && HASH_LONG_LOG2==2
#      define HASH_BLOCK_DATA_ORDER_ALIGNED	HASH_BLOCK_HOST_ORDER
#    endif
#  endif
#endif
#if !defined(HASH_BLOCK_DATA_ORDER_ALIGNED)
#ifndef HASH_BLOCK_DATA_ORDER
#error "HASH_BLOCK_DATA_ORDER must be defined!"
#endif
#endif

int		ft_md5_init(t_md5_st *c)
{
	c->A = 0x67452301U;
	c->B = 0xefcdab89U;
	c->C = 0x98badcfeU;
	c->D = 0x10325476U;
	c->Nl = 0;
	c->Nh = 0;
	c->num = 0;
	return (1);
}

typedef struct s_uvec4	t_uv4;
struct	s_uvec4
{
	unsigned int	a;
	unsigned int	b;
	unsigned int	c;
	unsigned int	d;
};
/* static inline void	ft_md5_round0(t_uv4 *f, const MD5_LONG *X)
{
	R0(f->a,f->b,f->c,f->d,X[0],7,0xd76aa478);
	R0(f->d,f->a,f->b,f->c,X[1],12,0xe8c7b756);
	R0(f->c,f->d,f->a,f->b,X[2],17,0x242070db);
	R0(f->b,f->c,f->d,f->a,X[3],22,0xc1bdceee);
	R0(f->a,f->b,f->c,f->d,X[4],7,0xf57c0faf);
	R0(f->d,f->a,f->b,f->c,X[5],12,0x4787c62a);
	R0(f->c,f->d,f->a,f->b,X[6],17,0xa8304613);
	R0(f->b,f->c,f->d,f->a,X[7],22,0xfd469501);
	R0(f->a,f->b,f->c,f->d,X[8],7,0x698098d8);
	R0(f->d,f->a,f->b,f->c,X[9],12,0x8b44f7af);
	R0(f->c,f->d,f->a,f->b,X[10],17,0xffff5bb1);
	R0(f->b,f->c,f->d,f->a,X[11],22,0x895cd7be);
	R0(f->a,f->b,f->c,f->d,X[12],7,0x6b901122);
	R0(f->d,f->a,f->b,f->c,X[13],12,0xfd987193);
	R0(f->c,f->d,f->a,f->b,X[14],17,0xa679438e);
	R0(f->b,f->c,f->d,f->a,X[15],22,0x49b40821);
}
static inline void	ft_md5_round1(t_uv4 *f, const MD5_LONG *X)
{
	R1(f->a,f->b,f->c,f->d,X[1],5,0xf61e2562);
	R1(f->d,f->a,f->b,f->c,X[6],9,0xc040b340);
	R1(f->c,f->d,f->a,f->b,X[11],14,0x265e5a51);
	R1(f->b,f->c,f->d,f->a,X[0],20,0xe9b6c7aa);
	R1(f->a,f->b,f->c,f->d,X[5],5,0xd62f105d);
	R1(f->d,f->a,f->b,f->c,X[10],9,0x02441453);
	R1(f->c,f->d,f->a,f->b,X[15],14,0xd8a1e681);
	R1(f->b,f->c,f->d,f->a,X[4],20,0xe7d3fbc8);
	R1(f->a,f->b,f->c,f->d,X[9],5,0x21e1cde6);
	R1(f->d,f->a,f->b,f->c,X[14],9,0xc33707d6);
	R1(f->c,f->d,f->a,f->b,X[3],14,0xf4d50d87);
	R1(f->b,f->c,f->d,f->a,X[8],20,0x455a14ed);
	R1(f->a,f->b,f->c,f->d,X[13],5,0xa9e3e905);
	R1(f->d,f->a,f->b,f->c,X[2],9,0xfcefa3f8);
	R1(f->c,f->d,f->a,f->b,X[7],14,0x676f02d9);
	R1(f->b,f->c,f->d,f->a,X[12],20,0x8d2a4c8a);
}
static inline void	ft_md5_round2(t_uv4 *f, const MD5_LONG *X)
{
	R2(f->a,f->b,f->c,f->d,X[5],4,0xfffa3942);
	R2(f->d,f->a,f->b,f->c,X[8],11,0x8771f681);
	R2(f->c,f->d,f->a,f->b,X[11],16,0x6d9d6122);
	R2(f->b,f->c,f->d,f->a,X[14],23,0xfde5380c);
	R2(f->a,f->b,f->c,f->d,X[1],4,0xa4beea44);
	R2(f->d,f->a,f->b,f->c,X[4],11,0x4bdecfa9);
	R2(f->c,f->d,f->a,f->b,X[7],16,0xf6bb4b60);
	R2(f->b,f->c,f->d,f->a,X[10],23,0xbebfbc70);
	R2(f->a,f->b,f->c,f->d,X[13],4,0x289b7ec6);
	R2(f->d,f->a,f->b,f->c,X[0],11,0xeaa127fa);
	R2(f->c,f->d,f->a,f->b,X[3],16,0xd4ef3085);
	R2(f->b,f->c,f->d,f->a,X[6],23,0x04881d05);
	R2(f->a,f->b,f->c,f->d,X[9],4,0xd9d4d039);
	R2(f->d,f->a,f->b,f->c,X[12],11,0xe6db99e5);
	R2(f->c,f->d,f->a,f->b,X[15],16,0x1fa27cf8);
	R2(f->b,f->c,f->d,f->a,X[2],23,0xc4ac5665);
}
static inline void	ft_md5_round3(t_uv4 *f, const MD5_LONG *X)
{
	R3(f->a,f->b,f->c,f->d,X[0],6,0xf4292244);
	R3(f->d,f->a,f->b,f->c,X[7],10,0x432aff97);
	R3(f->c,f->d,f->a,f->b,X[14],15,0xab9423a7);
	R3(f->b,f->c,f->d,f->a,X[5],21,0xfc93a039);
	R3(f->a,f->b,f->c,f->d,X[12],6,0x655b59c3);
	R3(f->d,f->a,f->b,f->c,X[3],10,0x8f0ccc92);
	R3(f->c,f->d,f->a,f->b,X[10],15,0xffeff47d);
	R3(f->b,f->c,f->d,f->a,X[1],21,0x85845dd1);
	R3(f->a,f->b,f->c,f->d,X[8],6,0x6fa87e4f);
	R3(f->d,f->a,f->b,f->c,X[15],10,0xfe2ce6e0);
	R3(f->c,f->d,f->a,f->b,X[6],15,0xa3014314);
	R3(f->b,f->c,f->d,f->a,X[13],21,0x4e0811a1);
	R3(f->a,f->b,f->c,f->d,X[4],6,0xf7537e82);
	R3(f->d,f->a,f->b,f->c,X[11],10,0xbd3af235);
	R3(f->c,f->d,f->a,f->b,X[2],15,0x2ad7d2bb);
	R3(f->b,f->c,f->d,f->a,X[9],21,0xeb86d391);
} */
void	ft_md5_block_host_order(t_md5_st *c, const void *data, int num)
{
	const MD5_LONG	*X;
	t_uv4			f;
	register unsigned int A;
	register unsigned int B;
	register unsigned int C;
	register unsigned int D;

	X = data;
	f.a = A = c->A;
	f.b = B = c->B;
	f.c = C = c->C;
	f.d = D = c->D;
	while (num--)
	{
		/* Round 0 */
		R0(A,B,C,D,X[ 0], 7,0xd76aa478);
		R0(D,A,B,C,X[ 1],12,0xe8c7b756);
		R0(C,D,A,B,X[ 2],17,0x242070db);
		R0(B,C,D,A,X[ 3],22,0xc1bdceee);
		R0(A,B,C,D,X[ 4], 7,0xf57c0faf);
		R0(D,A,B,C,X[ 5],12,0x4787c62a);
		R0(C,D,A,B,X[ 6],17,0xa8304613);
		R0(B,C,D,A,X[ 7],22,0xfd469501);
		R0(A,B,C,D,X[ 8], 7,0x698098d8);
		R0(D,A,B,C,X[ 9],12,0x8b44f7af);
		R0(C,D,A,B,X[10],17,0xffff5bb1);
		R0(B,C,D,A,X[11],22,0x895cd7be);
		R0(A,B,C,D,X[12], 7,0x6b901122);
		R0(D,A,B,C,X[13],12,0xfd987193);
		R0(C,D,A,B,X[14],17,0xa679438e);
		R0(B,C,D,A,X[15],22,0x49b40821);
		/* ft_md5_round0(&f, X); */
		/* Round 1 */
		R1(A,B,C,D,X[ 1], 5,0xf61e2562);
		R1(D,A,B,C,X[ 6], 9,0xc040b340);
		R1(C,D,A,B,X[11],14,0x265e5a51);
		R1(B,C,D,A,X[ 0],20,0xe9b6c7aa);
		R1(A,B,C,D,X[ 5], 5,0xd62f105d);
		R1(D,A,B,C,X[10], 9,0x02441453);
		R1(C,D,A,B,X[15],14,0xd8a1e681);
		R1(B,C,D,A,X[ 4],20,0xe7d3fbc8);
		R1(A,B,C,D,X[ 9], 5,0x21e1cde6);
		R1(D,A,B,C,X[14], 9,0xc33707d6);
		R1(C,D,A,B,X[ 3],14,0xf4d50d87);
		R1(B,C,D,A,X[ 8],20,0x455a14ed);
		R1(A,B,C,D,X[13], 5,0xa9e3e905);
		R1(D,A,B,C,X[ 2], 9,0xfcefa3f8);
		R1(C,D,A,B,X[ 7],14,0x676f02d9);
		R1(B,C,D,A,X[12],20,0x8d2a4c8a);
		/* ft_md5_round1(&f, X); */
		/* Round 2 */
		R2(A,B,C,D,X[ 5], 4,0xfffa3942);
		R2(D,A,B,C,X[ 8],11,0x8771f681);
		R2(C,D,A,B,X[11],16,0x6d9d6122);
		R2(B,C,D,A,X[14],23,0xfde5380c);
		R2(A,B,C,D,X[ 1], 4,0xa4beea44);
		R2(D,A,B,C,X[ 4],11,0x4bdecfa9);
		R2(C,D,A,B,X[ 7],16,0xf6bb4b60);
		R2(B,C,D,A,X[10],23,0xbebfbc70);
		R2(A,B,C,D,X[13], 4,0x289b7ec6);
		R2(D,A,B,C,X[ 0],11,0xeaa127fa);
		R2(C,D,A,B,X[ 3],16,0xd4ef3085);
		R2(B,C,D,A,X[ 6],23,0x04881d05);
		R2(A,B,C,D,X[ 9], 4,0xd9d4d039);
		R2(D,A,B,C,X[12],11,0xe6db99e5);
		R2(C,D,A,B,X[15],16,0x1fa27cf8);
		R2(B,C,D,A,X[ 2],23,0xc4ac5665);
		/* ft_md5_round2(&f, X); */
		/* Round 3 */
		R3(A,B,C,D,X[ 0], 6,0xf4292244);
		R3(D,A,B,C,X[ 7],10,0x432aff97);
		R3(C,D,A,B,X[14],15,0xab9423a7);
		R3(B,C,D,A,X[ 5],21,0xfc93a039);
		R3(A,B,C,D,X[12], 6,0x655b59c3);
		R3(D,A,B,C,X[ 3],10,0x8f0ccc92);
		R3(C,D,A,B,X[10],15,0xffeff47d);
		R3(B,C,D,A,X[ 1],21,0x85845dd1);
		R3(A,B,C,D,X[ 8], 6,0x6fa87e4f);
		R3(D,A,B,C,X[15],10,0xfe2ce6e0);
		R3(C,D,A,B,X[ 6],15,0xa3014314);
		R3(B,C,D,A,X[13],21,0x4e0811a1);
		R3(A,B,C,D,X[ 4], 6,0xf7537e82);
		R3(D,A,B,C,X[11],10,0xbd3af235);
		R3(C,D,A,B,X[ 2],15,0x2ad7d2bb);
		R3(B,C,D,A,X[ 9],21,0xeb86d391);
		/* ft_md5_round3(&f, X); */
		A = c->A += A;
		B = c->B += B;
		B = c->C += B;
		D = c->D += D;
		/* f.a = c->A += f.a;
		f.b = c->B += f.b;
		f.c = c->C += f.c;
		f.d = c->D += f.d; */
		X += HASH_LBLOCK;
	}
}

void	ft_md5_block_data_order(t_md5_st *c, const void *data_, int num) {
	const t_u8 *data = data_;
	register t_u64 A,B,C,D,l;
	MD5_LONG XX[MD5_LBLOCK];
	#define X(i) XX[i]
	A = c->A;
	B = c->B;
	C = c->C;
	D = c->D;
	ft_printf("block_data_order\n");
	while (num--) {
		HOST_c2l(data,l); X( 0)=l;			HOST_c2l(data,l); X( 1)=l;
		/* Round 0 */
		R0(A,B,C,D,X( 0), 7,0xd76aa478L);	HOST_c2l(data,l); X( 2)=l;
		R0(D,A,B,C,X( 1),12,0xe8c7b756L);	HOST_c2l(data,l); X( 3)=l;
		R0(C,D,A,B,X( 2),17,0x242070dbL);	HOST_c2l(data,l); X( 4)=l;
		R0(B,C,D,A,X( 3),22,0xc1bdceeeL);	HOST_c2l(data,l); X( 5)=l;
		R0(A,B,C,D,X( 4), 7,0xf57c0fafL);	HOST_c2l(data,l); X( 6)=l;
		R0(D,A,B,C,X( 5),12,0x4787c62aL);	HOST_c2l(data,l); X( 7)=l;
		R0(C,D,A,B,X( 6),17,0xa8304613L);	HOST_c2l(data,l); X( 8)=l;
		R0(B,C,D,A,X( 7),22,0xfd469501L);	HOST_c2l(data,l); X( 9)=l;
		R0(A,B,C,D,X( 8), 7,0x698098d8L);	HOST_c2l(data,l); X(10)=l;
		R0(D,A,B,C,X( 9),12,0x8b44f7afL);	HOST_c2l(data,l); X(11)=l;
		R0(C,D,A,B,X(10),17,0xffff5bb1L);	HOST_c2l(data,l); X(12)=l;
		R0(B,C,D,A,X(11),22,0x895cd7beL);	HOST_c2l(data,l); X(13)=l;
		R0(A,B,C,D,X(12), 7,0x6b901122L);	HOST_c2l(data,l); X(14)=l;
		R0(D,A,B,C,X(13),12,0xfd987193L);	HOST_c2l(data,l); X(15)=l;
		R0(C,D,A,B,X(14),17,0xa679438eL);
		R0(B,C,D,A,X(15),22,0x49b40821L);
		/* Round 1 */
		R1(A,B,C,D,X( 1), 5,0xf61e2562L);
		R1(D,A,B,C,X( 6), 9,0xc040b340L);
		R1(C,D,A,B,X(11),14,0x265e5a51L);
		R1(B,C,D,A,X( 0),20,0xe9b6c7aaL);
		R1(A,B,C,D,X( 5), 5,0xd62f105dL);
		R1(D,A,B,C,X(10), 9,0x02441453L);
		R1(C,D,A,B,X(15),14,0xd8a1e681L);
		R1(B,C,D,A,X( 4),20,0xe7d3fbc8L);
		R1(A,B,C,D,X( 9), 5,0x21e1cde6L);
		R1(D,A,B,C,X(14), 9,0xc33707d6L);
		R1(C,D,A,B,X( 3),14,0xf4d50d87L);
		R1(B,C,D,A,X( 8),20,0x455a14edL);
		R1(A,B,C,D,X(13), 5,0xa9e3e905L);
		R1(D,A,B,C,X( 2), 9,0xfcefa3f8L);
		R1(C,D,A,B,X( 7),14,0x676f02d9L);
		R1(B,C,D,A,X(12),20,0x8d2a4c8aL);
		/* Round 2 */
		R2(A,B,C,D,X( 5), 4,0xfffa3942L);
		R2(D,A,B,C,X( 8),11,0x8771f681L);
		R2(C,D,A,B,X(11),16,0x6d9d6122L);
		R2(B,C,D,A,X(14),23,0xfde5380cL);
		R2(A,B,C,D,X( 1), 4,0xa4beea44L);
		R2(D,A,B,C,X( 4),11,0x4bdecfa9L);
		R2(C,D,A,B,X( 7),16,0xf6bb4b60L);
		R2(B,C,D,A,X(10),23,0xbebfbc70L);
		R2(A,B,C,D,X(13), 4,0x289b7ec6L);
		R2(D,A,B,C,X( 0),11,0xeaa127faL);
		R2(C,D,A,B,X( 3),16,0xd4ef3085L);
		R2(B,C,D,A,X( 6),23,0x04881d05L);
		R2(A,B,C,D,X( 9), 4,0xd9d4d039L);
		R2(D,A,B,C,X(12),11,0xe6db99e5L);
		R2(C,D,A,B,X(15),16,0x1fa27cf8L);
		R2(B,C,D,A,X( 2),23,0xc4ac5665L);
		/* Round 3 */
		R3(A,B,C,D,X( 0), 6,0xf4292244L);
		R3(D,A,B,C,X( 7),10,0x432aff97L);
		R3(C,D,A,B,X(14),15,0xab9423a7L);
		R3(B,C,D,A,X( 5),21,0xfc93a039L);
		R3(A,B,C,D,X(12), 6,0x655b59c3L);
		R3(D,A,B,C,X( 3),10,0x8f0ccc92L);
		R3(C,D,A,B,X(10),15,0xffeff47dL);
		R3(B,C,D,A,X( 1),21,0x85845dd1L);
		R3(A,B,C,D,X( 8), 6,0x6fa87e4fL);
		R3(D,A,B,C,X(15),10,0xfe2ce6e0L);
		R3(C,D,A,B,X( 6),15,0xa3014314L);
		R3(B,C,D,A,X(13),21,0x4e0811a1L);
		R3(A,B,C,D,X( 4), 6,0xf7537e82L);
		R3(D,A,B,C,X(11),10,0xbd3af235L);
		R3(C,D,A,B,X( 2),15,0x2ad7d2bbL);
		R3(B,C,D,A,X( 9),21,0xeb86d391L);
		A = c->A += A;
		B = c->B += B;
		C = c->C += C;
		D = c->D += D;
	}
}

int		ft_md5_update(t_md5_st *c, const void *data_, t_ul len)
{
	const t_u8 *data = data_;
	register MD5_LONG *p;
	register t_u64 l;
	int sw,sc,ew,ec;

	if (len == 0)
		return (1);
	l = (c->Nl + (len << 3)) & 0xffffffffL;
	if (l < c->Nl)
		c->Nh++;
	c->Nh += (len >> 29);
	c->Nl = l;
	if (c->num != 0)
	{
		p = c->data;
		sw = c->num >> 2;
		sc = c->num & 0x03;
		if ((c->num + len) >= MD5_CBLOCK)
		{
			l = p[sw];
			HOST_p_c2l(data, l, sc);
			p[sw++] = l;
			for (; sw < MD5_LBLOCK; sw++)
			{
				HOST_c2l(data, l);
				p[sw] = l;
			}
			HASH_BLOCK_HOST_ORDER(c, p, 1);
			len -= (MD5_CBLOCK - c->num);
			c->num = 0;
		}
		else
		{
			c->num += len;
			if ((sc + len) < 4) {
				l = p[sw];
				HOST_p_c2l_p(data, l, sc, len);
				p[sw] = l;
			}
			else
			{
				ew = (c->num >> 2);
				ec = (c->num & 0x03);
				if (sc)
					l = p[sw];
				HOST_p_c2l(data, l, sc);
				p[sw++] = l;
				for (; sw < ew; sw++)
				{
					HOST_c2l(data, l);
					p[sw] = l;
				}
				if (ec)
				{
					HOST_c2l_p(data, l, ec);
					p[sw] = l;
				}
			}
		}
		return (1);
	}
	sw = len / MD5_CBLOCK;
	if (sw > 0) {
		if ((((t_ul)data)%4) == 0) {
			HASH_BLOCK_DATA_ORDER_ALIGNED(c, (MD5_LONG*)data, sw);
			sw *= MD5_CBLOCK;
			data += sw;
			len -= sw;
		} else {
			HASH_BLOCK_DATA_ORDER(c, data, sw);
			sw *= MD5_CBLOCK;
			data += sw;
			len -= sw;
		}
	}
	if (len != 0) {
		p = c->data;
		c->num = len;
		ew = len >> 2;
		ec = len & 0x03;
		while (ew) {
			HOST_c2l(data, l);
			*p = l;
			ew--;
			p++;
		}
		HOST_c2l_p(data, l, ec);
		*p = l;
	}
	return (1);
}

void	ft_md5_transform(t_md5_st *c, const t_u8 *data)
{
#if defined(HASH_BLOCK_DATA_ORDER_ALIGNED)
	if ((((t_ul)data) % 4) == 0)
		HASH_BLOCK_DATA_ORDER_ALIGNED(c, (MD5_LONG*)data, 1);
	else
#if !defined(HASH_BLOCK_DATA_ORDER)
	{
		ft_memcpy(c->data, data, MD5_CBLOCK);
		HASH_BLOCK_DATA_ORDER_ALIGNED(c, c->data, 1);
	}
#endif
#endif
#if defined(HASH_BLOCK_DATA_ORDER)
	HASH_BLOCK_DATA_ORDER(c, data, 1);
#endif
}

int		ft_md5_final(t_u8 *md, t_md5_st *c)
{
	register MD5_LONG	*p;
	register t_u64		l;
	register int		i, j;
	static const t_u8		end[4] = {0x80, 0x00, 0x00, 0x00};
	const t_u8			*cp = end;

	p = c->data;
	i = c->num >> 2;
	j = c->num & 0x03;
	l = (j == 0) ? 0 : p[i];
	HOST_p_c2l(cp, l, j);
	p[i++] = l;
	if (i > (MD5_LBLOCK - 2))
	{
		if (i < MD5_LBLOCK)
			p[i] = 0;
		HASH_BLOCK_HOST_ORDER(c, p, 1);
		i = -1;
	}
	while (++i < (MD5_LBLOCK-2))
		p[i] = 0;
	p[MD5_LBLOCK - 2] = c->Nl;
	p[MD5_LBLOCK - 1] = c->Nh;
	HASH_BLOCK_HOST_ORDER(c, p, 1);
	FHASH2STR(c, md);
	c->num = 0;
	return (1);
}
#define BUFSIZE 1024*16
/*
** check if *IN is dir and put error
** FILE *IN(author)(_flags=4, _file=3)
** FILE *IN(src)(_flags=, _file=)
*/
#define CHF(m) (!S_ISREG(m))

static inline void	pt(unsigned char *md)
{
	register int i;

	i = -1;
	while (++i < MD5_DIGEST_LENGTH)
		ft_printf("%02x", md[i]);
	ft_printf("\n");
}

void	do_fd(int fd)
{
	t_md5_st	c;
	t_u8		md[MD5_DIGEST_LENGTH];
	int			i;
	static t_u8	buf[BUFSIZE];

	ft_md5_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break;
		ft_md5_update(&c, buf, (t_u64)i);
	}
	ft_md5_final(&(md[0]), &c);
	pt(md);
}

int		ft_md5_dofiles(int ac, char **av)
{
	int			i;
	int			err;
	int			fd;
	struct stat	st;

	err = 0;
	i = 0;
	while (++i < ac)
	{
		fd = open(av[i], O_RDONLY);
		if (lstat(av[i], &st) || fd < 0 || CHF(st.st_mode))
		{
			if (CHF(st.st_mode))
				errno = EISDIR;
			perror(av[i]);
			err++;
			continue;
		}
		ft_printf("MD5(%s)= ", av[i]);
		do_fd(fd);
		close(fd);
	}
	return (err);
}

int		ft_md5_openssl(int ac, char **av)
{
	if (ac == 1)
		do_fd(STDIN_FILENO);
	else
		exit(ft_md5_dofiles(ac, av));
	return (0);
}
