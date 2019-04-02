#ifndef FT_SHA512_H
# define FT_SHA512_H

# include <stdlib.h>
# include <ft_ssl.h>

# define U64HILO(H,L) ((t_u64) (((t_u64)(H) << 32) + (L)))
# define U64LO(X) ((t_u64)(X))
# define U64SIZE(X) (U64LO(X))
# define U64LT(X,Y) ((X) < (Y))
# define U64PLUS(X,Y) ((X) + (Y))
# define U64OR(X,Y) ((X) | (Y))
# define U64XOR(X,Y) ((X) ^ (Y))
# define U64AND(X,Y) ((X) & (Y))
# define U64SHL(X,N) ((X) << (N))
# define U64SHR(X,N) ((X) >> (N))
# define U64INIT(H,L) (U64HILO(H,L))
# define SWAP(N) (__builtin_bswap64(N))
# define K(I) (g_k[I])
# define F2(A,B,C) (U64OR(U64AND(A, B), U64AND(C, U64OR(A, B))))
# define F1(E,F,G) (U64XOR(G, U64AND(E, U64XOR(F, G))))
#define S0(X) U64XOR(U64ROL(X, 63), U64XOR(U64ROL(X, 56), U64SHR(X, 7)))
#define S1(X) U64XOR(U64ROL(X, 45), U64XOR(U64ROL(X, 3), U64SHR(X, 6)))
#define SS0(X) U64XOR(U64ROL(X, 36), U64XOR(U64ROL(X, 30), U64ROL(X, 25)))
#define SS1(X) U64XOR(U64ROL(X, 50), U64XOR(U64ROL(X, 46), U64ROL(X, 23)))
#define M(I) (x[(I) & 15] = U64PLUS(x[(I) & 15],\
			U64PLUS(S1(x[((I) - 2) & 15]),\
			U64PLUS(x[((I) - 7) & 15],\
			S0(x[((I) - 15) & 15])))))
#define R(A,B,C,D,E,F,G,H,K,M) \
	do { \
		t_u64 t0 = U64PLUS(SS0(A), F2(A, B, C)); \
		t_u64 t1 = \
			U64PLUS(H, U64PLUS(SS1(E), \
				U64PLUS(F1(E, F, G), U64PLUS(K, M)))); \
		D = U64PLUS(D, t1); \
		H = U64PLUS(t0, t1); \
	} while (0)

static t_u64 const g_k[80] = {
	U64INIT(0x428a2f98, 0xd728ae22), U64INIT(0x71374491, 0x23ef65cd),
	U64INIT(0xb5c0fbcf, 0xec4d3b2f), U64INIT(0xe9b5dba5, 0x8189dbbc),
	U64INIT(0x3956c25b, 0xf348b538), U64INIT(0x59f111f1, 0xb605d019),
	U64INIT(0x923f82a4, 0xaf194f9b), U64INIT(0xab1c5ed5, 0xda6d8118),
	U64INIT(0xd807aa98, 0xa3030242), U64INIT(0x12835b01, 0x45706fbe),
	U64INIT(0x243185be, 0x4ee4b28c), U64INIT(0x550c7dc3, 0xd5ffb4e2),
	U64INIT(0x72be5d74, 0xf27b896f), U64INIT(0x80deb1fe, 0x3b1696b1),
	U64INIT(0x9bdc06a7, 0x25c71235), U64INIT(0xc19bf174, 0xcf692694),
	U64INIT(0xe49b69c1, 0x9ef14ad2), U64INIT(0xefbe4786, 0x384f25e3),
	U64INIT(0x0fc19dc6, 0x8b8cd5b5), U64INIT(0x240ca1cc, 0x77ac9c65),
	U64INIT(0x2de92c6f, 0x592b0275), U64INIT(0x4a7484aa, 0x6ea6e483),
	U64INIT(0x5cb0a9dc, 0xbd41fbd4), U64INIT(0x76f988da, 0x831153b5),
	U64INIT(0x983e5152, 0xee66dfab), U64INIT(0xa831c66d, 0x2db43210),
	U64INIT(0xb00327c8, 0x98fb213f), U64INIT(0xbf597fc7, 0xbeef0ee4),
	U64INIT(0xc6e00bf3, 0x3da88fc2), U64INIT(0xd5a79147, 0x930aa725),
	U64INIT(0x06ca6351, 0xe003826f), U64INIT(0x14292967, 0x0a0e6e70),
	U64INIT(0x27b70a85, 0x46d22ffc), U64INIT(0x2e1b2138, 0x5c26c926),
	U64INIT(0x4d2c6dfc, 0x5ac42aed), U64INIT(0x53380d13, 0x9d95b3df),
	U64INIT(0x650a7354, 0x8baf63de), U64INIT(0x766a0abb, 0x3c77b2a8),
	U64INIT(0x81c2c92e, 0x47edaee6), U64INIT(0x92722c85, 0x1482353b),
	U64INIT(0xa2bfe8a1, 0x4cf10364), U64INIT(0xa81a664b, 0xbc423001),
	U64INIT(0xc24b8b70, 0xd0f89791), U64INIT(0xc76c51a3, 0x0654be30),
	U64INIT(0xd192e819, 0xd6ef5218), U64INIT(0xd6990624, 0x5565a910),
	U64INIT(0xf40e3585, 0x5771202a), U64INIT(0x106aa070, 0x32bbd1b8),
	U64INIT(0x19a4c116, 0xb8d2d0c8), U64INIT(0x1e376c08, 0x5141ab53),
	U64INIT(0x2748774c, 0xdf8eeb99), U64INIT(0x34b0bcb5, 0xe19b48a8),
	U64INIT(0x391c0cb3, 0xc5c95a63), U64INIT(0x4ed8aa4a, 0xe3418acb),
	U64INIT(0x5b9cca4f, 0x7763e373), U64INIT(0x682e6ff3, 0xd6b2b8a3),
	U64INIT(0x748f82ee, 0x5defb2fc), U64INIT(0x78a5636f, 0x43172f60),
	U64INIT(0x84c87814, 0xa1f0ab72), U64INIT(0x8cc70208, 0x1a6439ec),
	U64INIT(0x90befffa, 0x23631e28), U64INIT(0xa4506ceb, 0xde82bde9),
	U64INIT(0xbef9a3f7, 0xb2c67915), U64INIT(0xc67178f2, 0xe372532b),
	U64INIT(0xca273ece, 0xea26619c), U64INIT(0xd186b8c7, 0x21c0c207),
	U64INIT(0xeada7dd6, 0xcde0eb1e), U64INIT(0xf57d4f7f, 0xee6ed178),
	U64INIT(0x06f067aa, 0x72176fba), U64INIT(0x0a637dc5, 0xa2c898a6),
	U64INIT(0x113f9804, 0xbef90dae), U64INIT(0x1b710b35, 0x131c471b),
	U64INIT(0x28db77f5, 0x23047d84), U64INIT(0x32caab7b, 0x40c72493),
	U64INIT(0x3c9ebe0a, 0x15c9bebc), U64INIT(0x431d67c4, 0x9c100d4c),
	U64INIT(0x4cc5d4be, 0xcb3e42b6), U64INIT(0x597f299c, 0xfc657e2a),
	U64INIT(0x5fcb6fab, 0x3ad6faec), U64INIT(0x6c44198c, 0x4a475817),
};

enum { SHA384_DIGEST_SIZE = 384 / 8 };
enum { SHA512_DIGEST_SIZE = 512 / 8 };

typedef struct s_sha512	t_sha512;
struct	s_sha512
{
	t_u64	state[8];
	t_u64	total[2];
	size_t	buflen;
	t_u64	buffer[32];
};

void	ft_sha512_init(t_sha512 *c);
void	ft_sha384_init(t_sha512 *c);

void	ft_sha512_proc_block(const void *buffer, size_t len, t_sha512 *c);
void	ft_sha512_proc_bytes(const void *buffer, size_t len, t_sha512 *c);

void	*ft_sha512_finish(t_sha512 *c, void *resbuf);
void	*ft_sha384_finish(t_sha512 *c, void *resbuf);

void	*ft_sha512_read(const t_sha512 *c, void *resbuf);
void	*ft_sha384_read(const t_sha512 *c, void *resbuf);

void	*ft_sha512_buffer(const char *buffer, size_t len, void *resblock);
void	*ft_sha384_buffer(const char *buffer, size_t len, void *resblock);

#endif
