#include "wifi.h"

unsigned int rol(unsigned int v, unsigned char i);
void movsb(const unsigned char *s, unsigned char *d, unsigned int l);
void movsd(const unsigned int *s, unsigned int *d, unsigned int l);
void xors(unsigned int *s, unsigned int val, unsigned int len);
void xors_(unsigned int *s, const unsigned int *v, unsigned int len);
void initW0(unsigned int *W);
void initW1(const unsigned char *t, unsigned int *W);
void initW2(const unsigned char *s, unsigned int len, unsigned int tlen, unsigned int *W);
void initW3(const unsigned char *s, unsigned int *W);
void sha1_sub(unsigned int *H, const unsigned int *W);
void reverseH(unsigned char *o);

unsigned int rol(unsigned int v, unsigned char i)
{
	unsigned int _ret=0;
	__asm__ __volatile__(
		"rol %%cl,%%eax"
		:"=a"(_ret)
		:"a"(v),"c"(i)
	);
}

void movsb(const unsigned char *s, unsigned char *d, unsigned int l)
{
	__asm__ __volatile__(
		"cld\n"
		"rep movsb"
		:
		:"S"(s),"D"(d),"c"(l)
	);
}

void movsd(const unsigned int *s, unsigned int *d, unsigned int l)
{
	__asm__ __volatile__(
		"cld\n"
		"rep movsl"
		:
		:"S"(s),"D"(d),"c"(l)
	);
}

void xors(unsigned int *s, unsigned int val, unsigned int len)
{
	__asm__ __volatile__(
		"movl %2,%%ecx\n"
		"1:\n"
		"xor %%eax,-4(%0,%%rcx,4)\n"
		"loop 1b"
		:
		:"D"(s),"a"(val),"m"(len)
		:"%ecx"
	);
}

void xors_(unsigned int *s, const unsigned int *v, unsigned int len)
{
	__asm__ __volatile__(
		"movl %2,%%ecx\n"
		"1:\n"
		"movl -4(%1,%%rcx,4),%%eax\n"
		"xor %%eax,-4(%0,%%rcx,4)\n"
		"loop 1b"
		:
		:"D"(s),"S"(v),"m"(len)
		:"%eax"
	);
}

void initW0(unsigned int *W)
{
	__asm__ __volatile__(
		"add $64,%0\n"
		"movq $64,%%rcx\n"
		"1:\n"
		"movl -12(%0),%%eax\n"
		"xor -32(%0),%%eax\n"
		"xor -56(%0),%%eax\n"
		"xor -64(%0),%%eax\n"
		"rol $1,%%eax\n"
		"movl %%eax,(%0)\n"
		"add $4,%0\n"
		"loop 1b"
		:
		:"D"(W)
		:"%eax","%rcx"
	);
}

void initW1(const unsigned char *t, unsigned int *W)
{
	__asm__ __volatile__(
		"movq $16,%%rcx\n"
		"1:\n"
		"movb (%0),%%ah\n"
		"movb 1(%0),%%al\n"
		"shl $16,%%eax\n"
		"movb 2(%0),%%ah\n"
		"movb 3(%0),%%al\n"
		"movl %%eax,(%1)\n"
		"add $4,%0\n"
		"add $4,%1\n"
		"loop 1b"
		:
		:"S"(t),"D"(W)
		:"%eax","%rcx"
	);
	initW0(W);
}

void initW2(const unsigned char *s, unsigned int len, unsigned int tlen, unsigned int *W)
{
	__asm__ __volatile__(
		"movq $16,%%rcx\n"
		"xor %%eax,%%eax\n"
		"push %1\n"
		"cld\n"
		"rep stosl\n"
		"xor %%edx,%%edx\n"
		"pop %1\n"
		"1:\n"
		"movl %%edx,%%ecx\n"
		"not %%ecx\n"
		"and $3,%%ecx\n"
		"shl $3,%%ecx\n"
		"xor %%eax,%%eax\n"
		"movb (%0,%%rdx),%%al\n"
		"shl %%cl,%%eax\n"
		"movl %%edx,%%ecx\n"
		"shr $2,%%ecx\n"
		"xor %%eax,(%1,%%rcx,4)\n"
		"inc %%edx\n"
		"cmp %2,%%edx\n"
		"jb 1b\n"
		"movl %%edx,%%ecx\n"
		"not %%ecx\n"
		"and $3,%%ecx\n"
		"shl $3,%%ecx\n"
		"movl $0x80,%%eax\n"
		"shl %%cl,%%eax\n"
		"movl %%edx,%%ecx\n"
		"shr $2,%%ecx\n"
		"xor %%eax,(%1,%%rcx,4)\n"
		"movl %3,%%eax\n"
		"shl $3,%%eax\n"
		"movl %%eax,60(%1)"
		:
		:"S"(s),"D"(W),"m"(len),"m"(tlen)
		:"%eax","%rcx","%rdx"
	);
	initW0(W);
}

void initW3(const unsigned char *s, unsigned int *W)
{
	__asm__ __volatile__(
		"movq $5,%%rcx\n"
		"1:\n"
		"movb (%0),%%ah\n"
		"movb 1(%0),%%al\n"
		"shl $16,%%eax\n"
		"movb 2(%0),%%ah\n"
		"movb 3(%0),%%al\n"
		"movl %%eax,(%1)\n"
		"add $4,%0\n"
		"add $4,%1\n"
		"loop 1b\n"
		"movl $0x80000000,(%1)\n"
		"xor %%eax,%%eax\n"
		"add $4,%1\n"
		"movq $9,%%rcx\n"
		"cld\n"
		"rep stosl\n"
		"movl $672,(%1)"
		:
		:"S"(s),"D"(W)
		:"%eax","%rcx"
	);
	initW0(W);
}

void sha1_sub(unsigned int *H, const unsigned int *W)
{
	unsigned int HT[5],i,t;
	movsd(H, HT, 5);
	for (i = 0; i < 20; ++i)
	{
		t=rol(HT[0], 5) + ( ( HT[1] & HT[2] ) | ( ~HT[1] & HT[3] ) ) + HT[4] + W[i] + 0x5A827999;
		HT[4] = HT[3]; HT[3] = HT[2]; HT[2] = rol(HT[1], 30); HT[1] = HT[0]; HT[0] = t;
	}
	for (i = 20; i < 40; ++i)
	{
		t = rol(HT[0], 5) + ( HT[1] ^ HT[2] ^ HT[3] ) + HT[4] + W[i] + 0x6ED9EBA1;
		HT[4] = HT[3]; HT[3] = HT[2]; HT[2] = rol(HT[1], 30); HT[1] = HT[0]; HT[0] = t;
	}
	for (i = 40; i < 60; ++i)
	{
		t = rol(HT[0], 5) + ( ( HT[1] & HT[2] ) | ( HT[1] & HT[3] ) | ( HT[2] & HT[3] ) ) + HT[4] + W[i] + 0x8F1BBCDC;
		HT[4] = HT[3]; HT[3] = HT[2]; HT[2] = rol(HT[1], 30); HT[1] = HT[0]; HT[0] = t;
	}
	for (i = 60; i < 80; ++i)
	{
		t = rol(HT[0], 5) + ( HT[1] ^ HT[2] ^ HT[3] ) + HT[4] + W[i] + 0xCA62C1D6;
		HT[4] = HT[3]; HT[3] = HT[2]; HT[2] = rol(HT[1], 30); HT[1] = HT[0]; HT[0] = t;
	}
	for (i = 0; i < 5; ++i)
		H[i] += HT[i];
}

void reverseH(unsigned char *o)
{
	__asm__ __volatile__(
		"movq $5,%%rcx\n"
		"1:\n"
		"roll $16,-4(%0,%%rcx,4)\n"
		"rolw $8,-4(%0,%%rcx,4)\n"
		"rolw $8,-2(%0,%%rcx,4)\n"
		"loop 1b"
		:
		:"b"(o)
		:"%rcx"
	);
}

void wifi(unsigned char *pass, unsigned int plen, unsigned char *ssid, unsigned int slen, unsigned char *O)
{
	unsigned char u[20],uu[20];
	unsigned char k0[64] = { 0 }, k1[64],kk[20];
	unsigned int H[] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0},W1[80],i,W2[80],W3[80],HT[5];

	// k0=PADDING(pass)
	movsb(pass, k0, plen);
	// k1=k0
	movsd((unsigned int*)k0, (unsigned int*)k1, 16);
	// k0^=ipad(0x36)
	xors((unsigned int*)k0, 0x36363636, 16);
	// k1^=opad(0x5c)
	xors((unsigned int*)k1, 0x5c5c5c5c, 16);


	*(unsigned int*)( ssid + slen ) = 0x01000000;

	movsd(H, (unsigned int*)kk, 5);
	initW1(k0, W1);
	sha1_sub((unsigned int*)kk, W1);
	movsd(W1, W2, 80);
	movsd((unsigned int*)kk, HT, 5);

	initW2(ssid, slen + 4, 64+slen + 4, W1);
	sha1_sub((unsigned int*)kk, W1);
	reverseH(kk);

	movsd(H, (unsigned int*)u, 5);
	initW1(k1, W1);
	sha1_sub((unsigned int*)u, W1);
	movsd(W1, W3, 80);
	movsd((unsigned int*)u, (unsigned int*)uu, 5);

	initW2(kk,20,84, W1);
	sha1_sub((unsigned int*)u, W1);
	reverseH(u);
	movsd((unsigned int*)u, (unsigned int*)O, 5);
	for (i = 1; i < 4096; ++i)
	{
		movsd(HT, (unsigned int*)kk, 5);
		movsd(W2, W1, 80);
		
		initW2(u, 20, 84, W1);
		sha1_sub((unsigned int*)kk, W1);
		reverseH(kk);

		movsd((unsigned int*)uu, (unsigned int*)u, 5);
		movsd(W3, W1, 80);
		initW2(kk, 20, 84, W1);
		sha1_sub((unsigned int*)u, W1);
		reverseH(u);
		xors_((unsigned int*)O, (unsigned int*)u, 5);
	}

	*(unsigned int*)( ssid + slen ) = 0x02000000;
	movsd(HT, (unsigned int*)kk, 5);
	movsd(W2, W1, 80);

	initW2(ssid, slen+4, 64+slen+4, W1);
	sha1_sub((unsigned int*)kk, W1);
	reverseH(kk);

	movsd((unsigned int*)uu, (unsigned int*)u, 5);
	movsd(W3, W1, 80);
	initW2(kk, 20, 84, W1);
	sha1_sub((unsigned int*)u, W1);
	reverseH(u);
	movsd((unsigned int*)u, (unsigned int*)(O+20),5);
	for (i = 1; i < 4096; ++i)
	{
		movsd(HT, (unsigned int*)kk, 5);
		movsd(W2, W1, 80);

		initW2(u, 20, 84, W1);
		sha1_sub((unsigned int*)kk, W1);
		reverseH(kk);

		movsd((unsigned int*)uu, (unsigned int*)u, 5);
		movsd(W3, W1, 80);
		initW2(kk, 20, 84, W1);
		sha1_sub((unsigned int*)u, W1);
		reverseH(u);
		xors_((unsigned int*)( O + 20 ), (unsigned int*)u, 5);
	}
}

