/*
20180104
*/
/*
- based on tweetnacl 20140427 (http://tweetnacl.cr.yp.to/software.html)
- slightly modified
*/

#include "crypto_hash_sha512.h"

static void store64_bigendian(unsigned char *y, unsigned long long x)
{

	long long i;

	for (i = 7; i >= 0; --i) {
		y[i] = x;
		x >>= 8;
	}
}

static unsigned long long load64_bigendian(const unsigned char *x)
{

	unsigned long long y = 0;
	long long i;

	for (i = 0; i < 8; ++i) {
		y = (y << 8) | x[i];
	}
	return y;
}

static unsigned long long R(unsigned long long x, int c)
{
	return (x >> c) | ((x & 0xffffffffffffffffULL) << (64 - c));
}
static unsigned long long Ch(unsigned long long x, unsigned long long y, unsigned long long z)
{
	return (x & y) ^ (~x & z);
}
static unsigned long long Maj(unsigned long long x, unsigned long long y, unsigned long long z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}
static unsigned long long Sigma0(unsigned long long x)
{
	return R(x, 28) ^ R(x, 34) ^ R(x, 39);
}
static unsigned long long Sigma1(unsigned long long x)
{
	return R(x, 14) ^ R(x, 18) ^ R(x, 41);
}
static unsigned long long sigma0(unsigned long long x)
{
	return R(x,  1) ^ R(x,  8) ^ (x >> 7);
}
static unsigned long long sigma1(unsigned long long x)
{
	return R(x, 19) ^ R(x, 61) ^ (x >> 6);
}

static const unsigned long long K[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

static void blocks(unsigned long long *z, const unsigned char *m, unsigned long long n)
{

	unsigned long long b[8], a[8], w[80], t;
	long long i, j;

	for (i = 0; i < 8; ++i) {
		a[i] = z[i];
	}

	while (n >= 128) {
		for (i =  0; i < 16; ++i) {
			w[i] = load64_bigendian(m + 8 * i);
		}
		for (i = 16; i < 80; ++i) {
			w[i] = (sigma1(w[i - 2]) + w[i - 7] + sigma0(w[i - 15]) + w[i - 16]) & 0xffffffffffffffffULL;
		}

		for (i = 0; i < 80; ++i) {
			for (j = 0; j < 8; ++j) {
				b[j] = a[j];
			}
			t = a[7] + Sigma1(a[4]) + Ch(a[4], a[5], a[6]) + K[i] + w[i];
			b[7] = t + Sigma0(a[0]) + Maj(a[0], a[1], a[2]);
			b[3] += t;
			for (j = 0; j < 8; ++j) {
				a[(j + 1) % 8] = b[j] & 0xffffffffffffffffULL;
			}
		}

		for (i = 0; i < 8; ++i) {
			a[i] += z[i];
			a[i] &= 0xffffffffffffffffULL;
			z[i] = a[i];
		}

		m += 128;
		n -= 128;
	}
}

int crypto_hash_sha512_tinyssh(unsigned char *o, const unsigned char *m, unsigned long long n)
{

	long long i;
	unsigned char x[256];
	unsigned long long b = n;
	unsigned long long h[8] = {
		0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
		0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
	};

	blocks(h, m, n);
	m += n;
	n &= 127;
	m -= n;

	for (i = 0; i < sizeof x; ++i) {
		x[i] = 0;
	}
	for (i = 0; i < n; ++i) {
		x[i] = m[i];
	}
	x[n] = 128;

	n = 256 - 128 * (n < 112);
	x[n - 9] = b >> 61;
	store64_bigendian(x + n - 8, b << 3);
	blocks(h, x, n);

	for (i = 0; i < 8; ++i) {
		store64_bigendian(o + 8 * i, h[i]);
	}
	return 0;
}
