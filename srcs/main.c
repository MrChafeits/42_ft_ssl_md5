#include "ft_ssl.h"
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define RL(x,c) (((x) << (c)) | ((x) >> (32 - (c))))
typedef uint64_t	u64;
typedef uint32_t	u32;
typedef uint16_t	u16;
typedef uint8_t		u8;

/*
** Constant integer part of the sines of integers in radians * 2^32
*/

static const u32 k[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const u32 r[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/*static const u8	PADDING[64] =
{
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};*/

typedef struct	s_md5
{
	u32	buf[4];
	u32	bits[2];
	u8	in[64];
}				t_md5;

enum	e_md5const
{
	MD5M_A=0x67452301UL,
	MD5M_B=0xefcdab89UL,
	MD5M_C=0x98badcfeUL,
	MD5M_D=0x10325476UL,
	MD5S00=7,
	MD5S01=12,
	MD5S02=17,
	MD5S03=22,
	MD5S10=5,
	MD5S11=9,
	MD5S12=14,
	MD5S13=20,
	MD5S20=4,
	MD5S21=11,
	MD5S22=16,
	MD5S23=23,
	MD5S30=6,
	MD5S31=10,
	MD5S32=15,
	MD5S33=21,
	MD5C00=0xd76aa478L,
	MD5C01=0xe8c7b756L,
	MD5C02=0x242070dbL,
	MD5C03=0xc1bdceeeL,
	MD5C04=0xf57c0fafL,
	MD5C05=0x4787c62aL,
	MD5C06=0xa8304613L,
	MD5C07=0xfd469501L,
	MD5C08=0x698098d8L,
	MD5C09=0x8b44f7afL,
	MD5C0a=0xffff5bb1L,
	MD5C0b=0x895cd7beL,
	MD5C0c=0x6b901122L,
	MD5C0d=0xfd987193L,
	MD5C0e=0xa679438eL,
	MD5C0f=0x49b40821L,
	MD5C10=0xf61e2562L,
	MD5C11=0xc040b340L,
	MD5C12=0x265e5a51L,
	MD5C13=0xe9b6c7aaL,
	MD5C14=0xd62f105dL,
	MD5C15=0x02441453L,
	MD5C16=0xd8a1e681L,
	MD5C17=0xe7d3fbc8L,
	MD5C18=0x21e1cde6L,
	MD5C19=0xc33707d6L,
	MD5C1a=0xf4d50d87L,
	MD5C1b=0x455a14edL,
	MD5C1c=0xa9e3e905L,
	MD5C1d=0xfcefa3f8L,
	MD5C1e=0x676f02d9L,
	MD5C1f=0x8d2a4c8aL,
	MD5C20=0xfffa3942L,
	MD5C21=0x8771f681L,
	MD5C22=0x6d9d6122L,
	MD5C23=0xfde5380cL,
	MD5C24=0xa4beea44L,
	MD5C25=0x4bdecfa9L,
	MD5C26=0xf6bb4b60L,
	MD5C27=0xbebfbc70L,
	MD5C28=0x289b7ec6L,
	MD5C29=0xeaa127faL,
	MD5C2a=0xd4ef3085L,
	MD5C2b=0x04881d05L,
	MD5C2c=0xd9d4d039L,
	MD5C2d=0xe6db99e5L,
	MD5C2e=0x1fa27cf8L,
	MD5C2f=0xc4ac5665L,
	MD5C30=0xf4292244L,
	MD5C31=0x432aff97L,
	MD5C32=0xab9423a7L,
	MD5C33=0xfc93a039L,
	MD5C34=0x655b59c3L,
	MD5C35=0x8f0ccc92L,
	MD5C36=0xffeff47dL,
	MD5C37=0x85845dd1L,
	MD5C38=0x6fa87e4fL,
	MD5C39=0xfe2ce6e0L,
	MD5C3a=0xa3014314L,
	MD5C3b=0x4e0811a1L,
	MD5C3c=0xf7537e82L,
	MD5C3d=0xbd3af235L,
	MD5C3e=0x2ad7d2bbL,
	MD5C3f=0xeb86d391L
};
#define ROTL(a,n) ((a << n) | (a >> (32 - n)))
u32	rotl32(const u32 a, const u32 n)
{
	return ((a << n) | (a >> (32 - n)));
}
#define ADD3(a,b,c) ((a) + (b) + (c))
u32	hc_add3(const u32 a, const u32 b, const u32 c)
{
	return (a + b + c);
}

#define MD5_F(x,y,z)	((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G(x,y,z)	((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H(x,y,z)	((x) ^ (y) ^ (z))
#define MD5_I(x,y,z)	((y) ^ ((x) | ~(z)))
#define MD5_FF(x,y,z)	(MD5_F((x), (y), (z)))
#define MD5_GG(x,y,z)	(MD5_G((x), (y), (z)))
#define MD54(a,b) ((a) += (b))
#define MD53(a,b,s) ((a) = rotl32(a, s); md54(a, b))
#define MD52(f,a,b,c,d,x,s) ((a) = hc_add3(a, x, f(b, c, d)); md53(a, b, s))
#define MD51(f,a,b,c,d,x,K,s) ((a) += K; md52(f, a, b, c, d, x, s))
#define MD5(f,a,b,c,d,x,K,s) {a+=K;a=hc_add3(a,x,f(b,c,d));a=rotl32(a,s);a+=b;}

static inline void	md5_round4(u32 w[4][4], u32 d[4], u32 m[4])
{
	MD5(MD5_I, d[0], d[1], d[2], d[3], w[0][0], MD5C30, MD5S30);
	MD5(MD5_I, d[3], d[0], d[1], d[2], w[1][3], MD5C31, MD5S31);
	MD5(MD5_I, d[2], d[3], d[0], d[1], w[3][2], MD5C32, MD5S32);
	MD5(MD5_I, d[1], d[2], d[3], d[0], w[1][1], MD5C33, MD5S33);
	MD5(MD5_I, d[0], d[1], d[2], d[3], w[3][0], MD5C34, MD5S30);
	MD5(MD5_I, d[3], d[0], d[1], d[2], w[0][3], MD5C35, MD5S31);
	MD5(MD5_I, d[2], d[3], d[0], d[1], w[2][2], MD5C36, MD5S32);
	MD5(MD5_I, d[1], d[2], d[3], d[0], w[0][1], MD5C37, MD5S33);
	MD5(MD5_I, d[0], d[1], d[2], d[3], w[2][0], MD5C38, MD5S30);
	MD5(MD5_I, d[3], d[0], d[1], d[2], w[3][3], MD5C39, MD5S31);
	MD5(MD5_I, d[2], d[3], d[0], d[1], w[1][2], MD5C3a, MD5S32);
	MD5(MD5_I, d[1], d[2], d[3], d[0], w[3][1], MD5C3b, MD5S33);
	MD5(MD5_I, d[0], d[1], d[2], d[3], w[1][0], MD5C3c, MD5S30);
	MD5(MD5_I, d[3], d[0], d[1], d[2], w[2][3], MD5C3d, MD5S31);
	MD5(MD5_I, d[2], d[3], d[0], d[1], w[0][2], MD5C3e, MD5S32);
	MD5(MD5_I, d[1], d[2], d[3], d[0], w[2][1], MD5C3f, MD5S33);
	m[0] += d[0];
	m[1] += d[1];
	m[2] += d[2];
	m[3] += d[3];
}

static inline void	md5_round3(u32 w[4][4], u32 d[4], u32 m[4])
{
	MD5(MD5_H, d[0], d[1], d[2], d[3], w[1][1], MD5C20, MD5S20);
	MD5(MD5_H, d[3], d[0], d[1], d[2], w[2][0], MD5C21, MD5S21);
	MD5(MD5_H, d[2], d[3], d[0], d[1], w[2][3], MD5C22, MD5S22);
	MD5(MD5_H, d[1], d[2], d[3], d[0], w[3][2], MD5C23, MD5S23);
	MD5(MD5_H, d[0], d[1], d[2], d[3], w[0][1], MD5C24, MD5S20);
	MD5(MD5_H, d[3], d[0], d[1], d[2], w[1][0], MD5C25, MD5S21);
	MD5(MD5_H, d[2], d[3], d[0], d[1], w[1][3], MD5C26, MD5S22);
	MD5(MD5_H, d[1], d[2], d[3], d[0], w[2][2], MD5C27, MD5S23);
	MD5(MD5_H, d[0], d[1], d[2], d[3], w[3][1], MD5C28, MD5S20);
	MD5(MD5_H, d[3], d[0], d[1], d[2], w[0][0], MD5C29, MD5S21);
	MD5(MD5_H, d[2], d[3], d[0], d[1], w[0][3], MD5C2a, MD5S22);
	MD5(MD5_H, d[1], d[2], d[3], d[0], w[1][2], MD5C2b, MD5S23);
	MD5(MD5_H, d[0], d[1], d[2], d[3], w[2][1], MD5C2c, MD5S20);
	MD5(MD5_H, d[3], d[0], d[1], d[2], w[3][0], MD5C2d, MD5S21);
	MD5(MD5_H, d[2], d[3], d[0], d[1], w[3][3], MD5C2e, MD5S22);
	MD5(MD5_H, d[1], d[2], d[3], d[0], w[0][2], MD5C2f, MD5S23);
	md5_round4(w, d, m);
}

static inline void	md5_round2(u32 w[4][4], u32 d[4], u32 m[4])
{
	MD5(MD5_GG, d[0], d[1], d[2], d[3], w[0][1], MD5C10, MD5S10);
	MD5(MD5_GG, d[3], d[0], d[1], d[2], w[1][2], MD5C11, MD5S11);
	MD5(MD5_GG, d[2], d[3], d[0], d[1], w[2][3], MD5C12, MD5S12);
	MD5(MD5_GG, d[1], d[2], d[3], d[0], w[0][0], MD5C13, MD5S13);
	MD5(MD5_GG, d[0], d[1], d[2], d[3], w[1][1], MD5C14, MD5S10);
	MD5(MD5_GG, d[3], d[0], d[1], d[2], w[2][2], MD5C15, MD5S11);
	MD5(MD5_GG, d[2], d[3], d[0], d[1], w[3][3], MD5C16, MD5S12);
	MD5(MD5_GG, d[1], d[2], d[3], d[0], w[1][0], MD5C17, MD5S13);
	MD5(MD5_GG, d[0], d[1], d[2], d[3], w[2][1], MD5C18, MD5S10);
	MD5(MD5_GG, d[3], d[0], d[1], d[2], w[3][2], MD5C19, MD5S11);
	MD5(MD5_GG, d[2], d[3], d[0], d[1], w[0][3], MD5C1a, MD5S12);
	MD5(MD5_GG, d[1], d[2], d[3], d[0], w[2][0], MD5C1b, MD5S13);
	MD5(MD5_GG, d[0], d[1], d[2], d[3], w[3][1], MD5C1c, MD5S10);
	MD5(MD5_GG, d[3], d[0], d[1], d[2], w[0][2], MD5C1d, MD5S11);
	MD5(MD5_GG, d[2], d[3], d[0], d[1], w[1][3], MD5C1e, MD5S12);
	MD5(MD5_GG, d[1], d[2], d[3], d[0], w[3][0], MD5C1f, MD5S13);
	md5_round3(w, d, m);
}

static inline void	md5_round1(u32 w[4][4], u32 d[4], u32 m[4])
{
	MD5(MD5_FF, d[0], d[1], d[2], d[3], w[0][0], MD5C00, MD5S00);
	MD5(MD5_FF, d[3], d[0], d[1], d[2], w[0][1], MD5C01, MD5S01);
	MD5(MD5_FF, d[2], d[3], d[0], d[1], w[0][2], MD5C02, MD5S02);
	MD5(MD5_FF, d[1], d[2], d[3], d[0], w[0][3], MD5C03, MD5S03);
	MD5(MD5_FF, d[0], d[1], d[2], d[3], w[1][0], MD5C04, MD5S00);
	MD5(MD5_FF, d[3], d[0], d[1], d[2], w[1][1], MD5C05, MD5S01);
	MD5(MD5_FF, d[2], d[3], d[0], d[1], w[1][2], MD5C06, MD5S02);
	MD5(MD5_FF, d[1], d[2], d[3], d[0], w[1][3], MD5C07, MD5S03);
	MD5(MD5_FF, d[0], d[1], d[2], d[3], w[2][0], MD5C08, MD5S00);
	MD5(MD5_FF, d[3], d[0], d[1], d[2], w[2][1], MD5C09, MD5S01);
	MD5(MD5_FF, d[2], d[3], d[0], d[1], w[2][2], MD5C0a, MD5S02);
	MD5(MD5_FF, d[1], d[2], d[3], d[0], w[2][3], MD5C0b, MD5S03);
	MD5(MD5_FF, d[0], d[1], d[2], d[3], w[3][0], MD5C0c, MD5S00);
	MD5(MD5_FF, d[3], d[0], d[1], d[2], w[3][1], MD5C0d, MD5S01);
	MD5(MD5_FF, d[2], d[3], d[0], d[1], w[3][2], MD5C0e, MD5S02);
	MD5(MD5_FF, d[1], d[2], d[3], d[0], w[3][3], MD5C0f, MD5S03);
	md5_round2(w, d, m);
}

void	ft_md5_64(const u32 block[16], u32 digest[4])
{
	u32	w[4][4];
	u32	m[4];
	int	i;

	i = -1;
	while (++i < 4)
	{
		w[0][i] = block[i];
		w[1][i] = block[i + 4];
		w[2][i] = block[i + 8];
		w[3][i] = block[i + 12];
	}
	md5_round1(w, digest, m);
	digest[0] = m[0];
	digest[1] = m[1];
	digest[2] = m[2];
	digest[3] = m[3];
}

#define HOST_c2l(c,l)	(l =(((unsigned int)(*((c)++)))),		\
			l|=(((unsigned int)(*((c)++)))<< 8),		\
			l|=(((unsigned int)(*((c)++)))<<16),		\
			l|=(((unsigned int)(*((c)++)))<<24),		\
			l)
// static inline host_c2l(const uint8_t *c, uint64_t l)
// {
// 	l = (((unsigned int)(*(c)++)));
// 	l |= (((unsigned int)(*((c)++)))<< 8);
// 	l |= (((unsigned int)(*((c)++)))<<16);
// 	l |= (((unsigned int)(*((c)++)))<<24);
// 	l;
// }
#define HOST_p_c2l(c,l,n)	{					\
			switch (n) {					\
			case 0: l =((unsigned int)(*((c)++)));		\
			case 1: l|=((unsigned int)(*((c)++)))<< 8;	\
			case 2: l|=((unsigned int)(*((c)++)))<<16;	\
			case 3: l|=((unsigned int)(*((c)++)))<<24;	\
				} }
#define HOST_p_c2l_p(c,l,sc,len) {					\
			switch (sc) {					\
			case 0: l =((unsigned int)(*((c)++)));		\
				if (--len == 0) break;			\
			case 1: l|=((unsigned int)(*((c)++)))<< 8;	\
				if (--len == 0) break;			\
			case 2: l|=((unsigned int)(*((c)++)))<<16;	\
				} }
/* NOTE the pointer is not incremented at the end of this */
#define HOST_c2l_p(c,l,n)	{					\
			l=0; (c)+=n;					\
			switch (n) {					\
			case 3: l =((unsigned int)(*(--(c))))<<16;	\
			case 2: l|=((unsigned int)(*(--(c))))<< 8;	\
			case 1: l|=((unsigned int)(*(--(c))));		\
				} }
#define HOST_l2c(l,c)	(*((c)++)=(unsigned char)(((l))&0xff),	\
			*((c)++)=(unsigned char)(((l)>> 8)&0xff),	\
			*((c)++)=(unsigned char)(((l)>>16)&0xff),	\
			*((c)++)=(unsigned char)(((l)>>24)&0xff),	\
			l)
#define MD5_LONG	unsigned int
#define MD5_CBLOCK	64
#define MD5_LBLOCK	(MD5_CBLOCK/4)
#define MD5_DIGEST_LENGTH 16
#define DATA_ORDER_IS_LITTLE_ENDIAN
#define HASH_CBLOCK MD5_CBLOCK
#define HASH_LONG MD5_LONG
typedef struct	s_md5_st
{
	MD5_LONG	A;
	MD5_LONG	B;
	MD5_LONG	C;
	MD5_LONG	D;
	MD5_LONG	Nl;
	MD5_LONG	Nh;
	MD5_LONG	data[16];
	int			num;
}				t_md5_st;

void	ft_md5_block_data_order(t_md5_st *c, const void *data_, int num);
void	ft_md5_block_host_order(t_md5_st *c, const void *data, int num);
int		ft_md5_init(t_md5_st *c);
int		ft_md5_update(t_md5_st *c, const void *data_, unsigned long len);
void	ft_md5_transform(t_md5_st *c, const u8 *data);
int		ft_md5_final(u8 *md, t_md5_st *c);
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
#define ROTATE(a,n)	(((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))
#define R0(a,b,c,d,k,s,t) {a+=((k)+(t)+F((b),(c),(d)));a=ROTATE(a,s);a+=b;};

#define R1(a,b,c,d,k,s,t) {a+=((k)+(t)+G((b),(c),(d)));a=ROTATE(a,s);a+=b;};

#define R2(a,b,c,d,k,s,t) {a+=((k)+(t)+H((b),(c),(d)));a=ROTATE(a,s);a+=b;};

#define R3(a,b,c,d,k,s,t) {a+=((k)+(t)+I((b),(c),(d)));a=ROTATE(a,s);a+=b;};
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

void	ft_md5_block_host_order(t_md5_st *c, const void *data, int num)
{
	const MD5_LONG *X;
	register unsigned int A;
	register unsigned int B;
	register unsigned int C;
	register unsigned int D;

	X = data;
	A = c->A;
	B = c->B;
	C = c->C;
	D = c->D;
	while (num--)
	{
		/* Round 0 */
		R0(A,B,C,D,X[ 0], 7,0xd76aa478);//printf("A(%08x) B(%08x) C(%08x) D(%08x)\n", A, B, C, D);
		R0(D,A,B,C,X[ 1],12,0xe8c7b756);
		R0(C,D,A,B,X[ 2],17,0x242070db);
		R0(B,C,D,A,X[ 3],22,0xc1bdceee);
		R0(A,B,C,D,X[ 4], 7,0xf57c0faf);//printf("A(%08x) B(%08x) C(%08x) D(%08x)\n", A, B, C, D);
		R0(D,A,B,C,X[ 5],12,0x4787c62a);
		R0(C,D,A,B,X[ 6],17,0xa8304613);
		R0(B,C,D,A,X[ 7],22,0xfd469501);
		R0(A,B,C,D,X[ 8], 7,0x698098d8);//printf("A(%08x) B(%08x) C(%08x) D(%08x)\n", A, B, C, D);
		R0(D,A,B,C,X[ 9],12,0x8b44f7af);
		R0(C,D,A,B,X[10],17,0xffff5bb1);
		R0(B,C,D,A,X[11],22,0x895cd7be);
		R0(A,B,C,D,X[12], 7,0x6b901122);//printf("A(%08x) B(%08x) C(%08x) D(%08x)\n", A, B, C, D);
		R0(D,A,B,C,X[13],12,0xfd987193);
		R0(C,D,A,B,X[14],17,0xa679438e);
		R0(B,C,D,A,X[15],22,0x49b40821);
		/* Round 1 */
		R1(A,B,C,D,X[ 1], 5,0xf61e2562);//printf("A(%08x) B(%08x) C(%08x) D(%08x)\n", A, B, C, D);
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
		A = c->A += A;
		B = c->B += B;
		C = c->C += C;
		D = c->D += D;
		X+=HASH_LBLOCK;
		//printf("END\nA(%08x) B(%08x) C(%08x) D(%08x)\n", A, B, C, D);
	}
}

void	ft_md5_block_data_order(t_md5_st *c, const void *data_, int num) {
	const u8 *data = data_;
	register u64 A,B,C,D,l;
	MD5_LONG XX[MD5_LBLOCK];
	#define X(i) XX[i]
	A = c->A;
	B = c->B;
	C = c->C;
	D = c->D;
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

int		ft_md5_update(t_md5_st *c, const void *data_, unsigned long len)
{
	const u8 *data = data_;
	register MD5_LONG *p;
	register u64 l;
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
		if ((((unsigned long)data)%4) == 0) {
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

void	ft_md5_transform(t_md5_st *c, const u8 *data)
{
#if defined(HASH_BLOCK_DATA_ORDER_ALIGNED)
	if ((((unsigned long)data)%4) == 0)
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

int		ft_md5_final(u8 *md, t_md5_st *c)
{
	register MD5_LONG	*p;
	register u64		l;
	register int		i, j;
	static const u8		end[4] = {0x80, 0x00, 0x00, 0x00};
	const u8			*cp = end;

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
	HASH_MAKE_STRING(c, md);
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
	u8			md[MD5_DIGEST_LENGTH];
	int			i;
	static u8	buf[BUFSIZE];

	ft_md5_init(&c);
	while (1)
	{
		i = read(fd, buf, BUFSIZE);
		if (i <= 0)
			break;
		ft_md5_update(&c, buf, (u64)i);
	}
	ft_md5_final(&(md[0]), &c);
	// for (i = 0; i < 8; i++)
	// 	ft_printf("W[%2d] = %08x\tW[%2d] = %08x\n", i, c.data[i], i+8, c.data[i+8]);
	pt(md);
}

int		ft_md5_openssl(int ac, char **av)
{
	int			i;
	int			err;
	int			fd;
	struct stat	st;

	i = 0;
	err = 0;
	if (ac == 1)
		do_fd(STDIN_FILENO);
	else
	{
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
			printf("MD5(%s)= ", av[i]);
			do_fd(fd);
			close(fd);
		}
	}
	exit(err);
}

/*
 * https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md32_common.h.auto.html
 * https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5.c.auto.html
 * https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5_dgst.c.auto.html
 * https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5.h.auto.html
 * https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/md5_locl.h.auto.html
 * https://opensource.apple.com/source/OpenSSL/OpenSSL-46.11/openssl/crypto/md5/
 */

void	ft_md5_nolimit(u32 digest[4], const u32 *plain, const u32 plain_len)
{
	u32 a = MD5M_A;
	u32 b = MD5M_B;
	u32 c = MD5M_C;
	u32 d = MD5M_D;
	digest[0] = a;
	digest[1] = b;
	digest[2] = c;
	digest[3] = d;
	u32 r_a = digest[0];
	u32 r_b = digest[1];
	u32 r_c = digest[2];
	u32 r_d = digest[3];
	u32 block[16];
	int block_total_len = 16 * 8;
	char *block_ptr = (char *)block;
	char *plain_ptr = (char *)plain;
	int remaining_len = (int)plain_len;
	u32 loop = 1;

	while (loop)
	{
		loop = (remaining_len > 55);
		int cur_len = MIN(block_total_len, remaining_len);
		int copy_len = MAX(cur_len, 0);
		ft_bzero(block_ptr, block_total_len);
		ft_memcpy(block_ptr, plain_ptr, (size_t)copy_len);
		if (cur_len >= 0)
			if (copy_len < block_total_len)
				block_ptr[copy_len] = (char)0x80;
		if (!loop)
			block[14] = plain_len * 8;
		u32 w0[4];
		u32 w1[4];
		u32 w2[4];
		u32 w3[4];
		w0[0] = block[ 0];
		w0[1] = block[ 1];
		w0[2] = block[ 2];
		w0[3] = block[ 3];
		w1[0] = block[ 4];
		w1[1] = block[ 5];
		w1[2] = block[ 6];
		w1[3] = block[ 7];
		w2[0] = block[ 8];
		w2[1] = block[ 9];
		w2[2] = block[10];
		w2[3] = block[11];
		w3[0] = block[12];
		w3[1] = block[13];
		w3[2] = block[14];
		w3[3] = block[15];

		MD5(MD5_FF, a, b, c, d, w0[0], MD5C00, MD5S00);
		MD5(MD5_FF, d, a, b, c, w0[1], MD5C01, MD5S01);
		MD5(MD5_FF, c, d, a, b, w0[2], MD5C02, MD5S02);
		MD5(MD5_FF, b, c, d, a, w0[3], MD5C03, MD5S03);
		MD5(MD5_FF, a, b, c, d, w1[0], MD5C04, MD5S00);
		MD5(MD5_FF, d, a, b, c, w1[1], MD5C05, MD5S01);
		MD5(MD5_FF, c, d, a, b, w1[2], MD5C06, MD5S02);
		MD5(MD5_FF, b, c, d, a, w1[3], MD5C07, MD5S03);
		MD5(MD5_FF, a, b, c, d, w2[0], MD5C08, MD5S00);
		MD5(MD5_FF, d, a, b, c, w2[1], MD5C09, MD5S01);
		MD5(MD5_FF, c, d, a, b, w2[2], MD5C0a, MD5S02);
		MD5(MD5_FF, b, c, d, a, w2[3], MD5C0b, MD5S03);
		MD5(MD5_FF, a, b, c, d, w3[0], MD5C0c, MD5S00);
		MD5(MD5_FF, d, a, b, c, w3[1], MD5C0d, MD5S01);
		MD5(MD5_FF, c, d, a, b, w3[2], MD5C0e, MD5S02);
		MD5(MD5_FF, b, c, d, a, w3[3], MD5C0f, MD5S03);

		MD5(MD5_GG, a, b, c, d, w0[1], MD5C10, MD5S10);
		MD5(MD5_GG, d, a, b, c, w1[2], MD5C11, MD5S11);
		MD5(MD5_GG, c, d, a, b, w2[3], MD5C12, MD5S12);
		MD5(MD5_GG, b, c, d, a, w0[0], MD5C13, MD5S13);
		MD5(MD5_GG, a, b, c, d, w1[1], MD5C14, MD5S10);
		MD5(MD5_GG, d, a, b, c, w2[2], MD5C15, MD5S11);
		MD5(MD5_GG, c, d, a, b, w3[3], MD5C16, MD5S12);
		MD5(MD5_GG, b, c, d, a, w1[0], MD5C17, MD5S13);
		MD5(MD5_GG, a, b, c, d, w2[1], MD5C18, MD5S10);
		MD5(MD5_GG, d, a, b, c, w3[2], MD5C19, MD5S11);
		MD5(MD5_GG, c, d, a, b, w0[3], MD5C1a, MD5S12);
		MD5(MD5_GG, b, c, d, a, w2[0], MD5C1b, MD5S13);
		MD5(MD5_GG, a, b, c, d, w3[1], MD5C1c, MD5S10);
		MD5(MD5_GG, d, a, b, c, w0[2], MD5C1d, MD5S11);
		MD5(MD5_GG, c, d, a, b, w1[3], MD5C1e, MD5S12);
		MD5(MD5_GG, b, c, d, a, w3[0], MD5C1f, MD5S13);

		MD5(MD5_H , a, b, c, d, w1[1], MD5C20, MD5S20);
		MD5(MD5_H , d, a, b, c, w2[0], MD5C21, MD5S21);
		MD5(MD5_H , c, d, a, b, w2[3], MD5C22, MD5S22);
		MD5(MD5_H , b, c, d, a, w3[2], MD5C23, MD5S23);
		MD5(MD5_H , a, b, c, d, w0[1], MD5C24, MD5S20);
		MD5(MD5_H , d, a, b, c, w1[0], MD5C25, MD5S21);
		MD5(MD5_H , c, d, a, b, w1[3], MD5C26, MD5S22);
		MD5(MD5_H , b, c, d, a, w2[2], MD5C27, MD5S23);
		MD5(MD5_H , a, b, c, d, w3[1], MD5C28, MD5S20);
		MD5(MD5_H , d, a, b, c, w0[0], MD5C29, MD5S21);
		MD5(MD5_H , c, d, a, b, w0[3], MD5C2a, MD5S22);
		MD5(MD5_H , b, c, d, a, w1[2], MD5C2b, MD5S23);
		MD5(MD5_H , a, b, c, d, w2[1], MD5C2c, MD5S20);
		MD5(MD5_H , d, a, b, c, w3[0], MD5C2d, MD5S21);
		MD5(MD5_H , c, d, a, b, w3[3], MD5C2e, MD5S22);
		MD5(MD5_H , b, c, d, a, w0[2], MD5C2f, MD5S23);

		MD5(MD5_I , a, b, c, d, w0[0], MD5C30, MD5S30);
		MD5(MD5_I , d, a, b, c, w1[3], MD5C31, MD5S31);
		MD5(MD5_I , c, d, a, b, w3[2], MD5C32, MD5S32);
		MD5(MD5_I , b, c, d, a, w1[1], MD5C33, MD5S33);
		MD5(MD5_I , a, b, c, d, w3[0], MD5C34, MD5S30);
		MD5(MD5_I , d, a, b, c, w0[3], MD5C35, MD5S31);
		MD5(MD5_I , c, d, a, b, w2[2], MD5C36, MD5S32);
		MD5(MD5_I , b, c, d, a, w0[1], MD5C37, MD5S33);
		MD5(MD5_I , a, b, c, d, w2[0], MD5C38, MD5S30);
		MD5(MD5_I , d, a, b, c, w3[3], MD5C39, MD5S31);
		MD5(MD5_I , c, d, a, b, w1[2], MD5C3a, MD5S32);
		MD5(MD5_I , b, c, d, a, w3[1], MD5C3b, MD5S33);
		MD5(MD5_I , a, b, c, d, w1[0], MD5C3c, MD5S30);
		MD5(MD5_I , d, a, b, c, w2[3], MD5C3d, MD5S31);
		MD5(MD5_I , c, d, a, b, w0[2], MD5C3e, MD5S32);
		MD5(MD5_I , b, c, d, a, w2[1], MD5C3f, MD5S33);
		remaining_len -= block_total_len;
		if (remaining_len > 0) plain_ptr += 64;
		a += r_a;
		b += r_b;
		c += r_c;
		d += r_d;
		digest[0] = a;
		digest[1] = b;
		digest[2] = c;
		digest[3] = d;
		r_a = digest[0];
		r_b = digest[1];
		r_c = digest[2];
		r_d = digest[3];
		if (remaining_len < 0)
			break;
	}
	return;
}

void	ft_tobytes(uint32_t v, uint8_t *b)
{
	b[0] = (uint8_t)v;
	b[1] = (uint8_t)(v >> 8);
	b[2] = (uint8_t)(v >> 16);
	b[3] = (uint8_t)(v >> 24);
}

uint32_t	ft_toint(const uint8_t *b)
{
	return ((uint32_t)b[0]
			| ((uint32_t)b[1] << 8)
			| ((uint32_t)b[2] << 16)
			| ((uint32_t)b[3] << 24));
}
int flippo = 0;
void	ft_md5(const uint8_t *in, size_t ilen, uint8_t *dgst)
{
	uint32_t	h[4];
	uint8_t		*msg;
	uint32_t	w[16];
	uint32_t	a, b, c, d, i, f, g, tmp;
	size_t		nlen, offs;

	h[0] = 0x67452301;
	h[1] = 0xefcdab89;
	h[2] = 0x98badcfe;
	h[3] = 0x10325476;
	nlen = ilen;
	while (++nlen % 64 != 56)
		;
	msg = (uint8_t*)malloc(nlen + 8);
	ft_memcpy(msg, in, ilen);
	msg[ilen] = 0x80;
	offs = ilen;
	while (++offs < nlen)
		;
	ft_tobytes(ilen * 8, msg + nlen);
	ft_tobytes(ilen >> 29, msg + nlen + 4);
	offs = 0;
	while (offs < nlen)
	{
		for (i = 0; i < 16; i++) {
			w[i] = ft_toint(msg + offs + i * 4);
			// ft_printf("W[%2d] = %08x\n", i, w[i]);
		}
		a = h[0];
		b = h[1];
		c = h[2];
		d = h[3];
		for (i = 0; i < 64; i++)
		{
			if (i < 16)
			{
				f = (b & c) | ((~b) & d);
				g = i;
			}
			else if (i < 32)
			{
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i < 48)
			{
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else
			{
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}
			tmp = d;
			d = c;
			c = b;
			b = b + RL((a + f + k[i] + w[g]), r[i]);
			a = tmp;
			offs += 64;
		}
		h[0] += a;
		h[1] += b;
		h[2] += c;
		h[3] += d;
	}
	free(msg);
	ft_tobytes(h[0], dgst);
	ft_tobytes(h[1], dgst + 4);
	ft_tobytes(h[2], dgst + 8);
	ft_tobytes(h[3], dgst + 12);
	if (flippo == 99999) {
		for (i = 0; i < 4; i++) {
			ft_printf("W[%2d - %2d] = %08x %08x %08x %08x\n",i*4,i*4+4, w[i*4],w[i*4+1],w[i*4+2],w[i*4+3]);
		}
	}
}

//ft_toint(const uint8_t b[4]) -> uint32_t ret; for fast need u32 block[16];
void	ft_fast_md5(char *msg, size_t len)
{
	u32 block[16] = {0};
	u32 digest[4] = {0};
	size_t i;
	size_t j;
	size_t o;
	size_t m;

	if (len <= 64)
	{
		if ((o = len % 4))
		{
			len -= o;
			o += len;
		}
		for (i = 0; i <= len; i += 4)//asdf ghjk l
			block[i / 4] = ft_toint((u8*)msg + i);
		ft_md5_nolimit(digest, block, i);
		/* ft_md5_64(block, digest); */
		ft_printf("MD5(%s)= ", msg);
		for(i = 0; i < 4; i++)
			ft_printf("%8.8x", digest[i]);
		ft_printf("\n");
		return;
	}
	else
	{
		if ((o = len % 64))
			len -= o;
		m = len / 64;
		for (i = 0; i <= m; i++)
		{
			for (j = 0; j < 64; j += 4)
				block[j / 4] = ft_toint((u8*)msg + j);msg += j;
			if (o)
			{
				m = o % 4;
				for (i = 0; i <= m; i++)
					block[i] = ft_toint((u8*)msg + (i * 4));msg += (i*4);
			}
			ft_md5_64(block, digest);
			for (i = 0; i < 4; i++)
				ft_printf("%8.8x", block[i]);
		}
	}
}

void	ft_slow_md5(char const *msg, size_t len)
{
	size_t	i;
	u8		res[16];

	for (i = 0; i < 100000; i++, flippo++)
		ft_md5((uint8_t*)msg, len, res);
	for (i = 0; i < 16; i++)
		ft_printf("%2.2x", res[i]);
	ft_putchar('\n');
}

void	usage(char *bin, char *cmd, int opt)
{
	if (opt)
	{
		ft_printf("usage: %s command [command opts] [command args]\n", bin);
	}
	else
	{
		ft_printf("%s: Error: \'%s\' is an invalid command.\n\n", bin, cmd);
		ft_printf("Standard commands:\n\n");
		ft_printf("Message Digest commands:\nopenssl\nfast\nslow\n\n");
		ft_printf("Cipher commands:\n\n");
	}
}

int		main(int ac, char **av)
{//mimic openssl and include md5 + sha256 at min, check out openssl github
	char	*msg;
	size_t	len;

	if (ac == 1)
		usage(av[0], av[1], 0);
	if (ac >= 2)
	{
		if (!ft_strcmp("openssl", av[1]))
			ft_md5_openssl(--ac, ++av);
		else if (ac == 3)
		{
			msg = av[2];
			len = ft_strlen(msg);
			if (!ft_strcmp("fast", av[1]))
				ft_fast_md5(msg, len);
			else if (!ft_strcmp("slow", av[1]))
				ft_slow_md5(msg, len);
		}
		else
			usage(av[0], av[1], 1);
	}
	exit(0);
}
