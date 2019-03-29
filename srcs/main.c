#include "ft_ssl.h"

#define RL(x,c) (((x) << (c)) | ((x) >> (32 - (c))))

/*
** Constant integer part of the sines of integers in radians * 2^32
*/

static const t_u32 k[64] =
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

static const t_u32 r[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/*static const t_u8	PADDING[64] =
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
	t_u32	buf[4];
	t_u32	bits[2];
	t_u8	in[64];
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
t_u32	rotl32(const t_u32 a, const t_u32 n)
{
	return ((a << n) | (a >> (32 - n)));
}
#define ADD3(a,b,c) ((a) + (b) + (c))
t_u32	hc_add3(const t_u32 a, const t_u32 b, const t_u32 c)
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

static inline void	md5_round4(t_u32 w[4][4], t_u32 d[4], t_u32 m[4])
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

static inline void	md5_round3(t_u32 w[4][4], t_u32 d[4], t_u32 m[4])
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

static inline void	md5_round2(t_u32 w[4][4], t_u32 d[4], t_u32 m[4])
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

static inline void	md5_round1(t_u32 w[4][4], t_u32 d[4], t_u32 m[4])
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

void	ft_md5_64(const t_u32 block[16], t_u32 digest[4])
{
	t_u32	w[4][4];
	t_u32	m[4];
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

void	ft_md5_nolimit(t_u32 digest[4], const t_u32 *plain, const t_u32 plain_len)
{
	t_u32 a = MD5M_A;
	t_u32 b = MD5M_B;
	t_u32 c = MD5M_C;
	t_u32 d = MD5M_D;
	digest[0] = a;
	digest[1] = b;
	digest[2] = c;
	digest[3] = d;
	t_u32 r_a = digest[0];
	t_u32 r_b = digest[1];
	t_u32 r_c = digest[2];
	t_u32 r_d = digest[3];
	t_u32 block[16];
	int block_total_len = 16 * 8;
	char *block_ptr = (char *)block;
	char *plain_ptr = (char *)plain;
	int remaining_len = (int)plain_len;
	t_u32 loop = 1;

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
		t_u32 w0[4];
		t_u32 w1[4];
		t_u32 w2[4];
		t_u32 w3[4];
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
}

//ft_toint(const uint8_t b[4]) -> uint32_t ret; for fast need t_u32 block[16];
void	ft_fast_md5(char *msg, size_t len)
{
	t_u32 block[16] = {0};
	t_u32 digest[4] = {0};
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
			block[i / 4] = ft_toint((t_u8*)msg + i);
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
				block[j / 4] = ft_toint((t_u8*)msg + j);msg += j;
			if (o)
			{
				m = o % 4;
				for (i = 0; i <= m; i++)
					block[i] = ft_toint((t_u8*)msg + (i * 4));msg += (i*4);
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
	t_u8		res[16];

	for (i = 0; i < 100000; i++)
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

/*
** mimic openssl and include md5 + sha256 at min, check out openssl github
** awindham github awin ft_ssl_md5
*/

int		main(int ac, char **av)
{
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
