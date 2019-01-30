#define _countof(x) (sizeof(x)/sizeof(*(x)))

#define K(t) ((t)<40?((t)<20?0x5A827999:0x6ED9EBA1):((t)<60?0x8F1BBCDC:0xCA62C1D6))
#define F(t,B,C,D) ((t)<40?((t)<20?(((B)&(C))|((~B)&(D))):((B)^(C)^(D))):((t)<60?((B)&(C)|(B)&(D)|(C)&(D)):((B)^(C)^(D))))

unsigned int rol(unsigned int v, unsigned char i)
{
	unsigned int _ret=0;
	__asm__ __volatile__(
		"rol %%cl,%%eax"
		:"=a"(_ret)
		:"a"(v),"c"(i)
	);
	return _ret;
}

void _rol(unsigned int *v,unsigned char i)
{
	__asm__ __volatile__(
		"rol %%cl,%%eax"
		:"=a"(*v)
		:"a"(*v),"c"(i)
	);
}

void _rolw(unsigned short *v,unsigned char i)
{
	__asm__ __volatile__(
		"rol %%cl,%%ax"
		:"=a"(*v)
		:"a"(*v),"c"(i)
	);
}

void movsl(const unsigned int *s, unsigned int *d, unsigned int l)
{
	__asm__ __volatile__(
		"cld\n"
		"rep movsl"
		:
		:"S"(s),"D"(d),"c"(l)
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

void _SHA1(const unsigned char* src, unsigned long long len, unsigned char O[])
{
	unsigned long long i = 0, j = 0, k = 0, n = ( ( len + 8 ) >> 6 ) + 1, ln = len << 3;
	unsigned int* H = (unsigned int*)O;
	H[0] = 0x67452301;
	H[1] = 0xEFCDAB89;
	H[2] = 0x98BADCFE;
	H[3] = 0x10325476;
	H[4] = 0xC3D2E1F0;
	for (k = 0; k < n; ++k)
	{
		unsigned int a, b, c, d, e, W[80] = { 0 };
		for (i = 0; i < 16; ++i)
			for (j = 0; j < 4; ++j)
			{
				unsigned long long l = k * 64 + i * 4 + j;
				if (l < len)
				{
					if (src[l])
						W[i] |= (unsigned int)src[l] << ( 8 * ( 3 - j ) );
				}
				else if (l == len)
					W[i] |= 0x80 << ( 8 * ( 3 - j ) );
				else break;
			}
		if (k == n - 1)
		{
			W[14] = ln >> 32 & 0xffffffff;
			W[15] = ln & 0xffffffff;
		}
		while (i < 80)
		{
			W[i] = rol(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			++i;
		}
		a = H[0]; b = H[1]; c = H[2]; d = H[3]; e = H[4];
		for (i = 0; i < 80; ++i)
		{
			unsigned int t = rol(a, 5) + F(i, b, c, d) + e + W[i] + K(i);
			e = d; d = c; c = rol(b, 30); b = a; a = t;
		}
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
	}
	for(i=0;i<5;++i)
	{
		_rol(&H[i],16);
		_rolw((unsigned short*)&H[i],8);
		_rolw((unsigned short*)&H[i]+1,8);
	}
}

void _SHA1_ex(const unsigned char* t, const unsigned char *src, unsigned long long len, unsigned char O[])
{
	unsigned long long i = 0, j = 0, k = 0, n = ( ( len + 8 ) >> 6 ) + 1, ln = ( len << 3 ) + 512;
	unsigned int* H = (unsigned int*)O;
	H[0] = 0x67452301;
	H[1] = 0xEFCDAB89;
	H[2] = 0x98BADCFE;
	H[3] = 0x10325476;
	H[4] = 0xC3D2E1F0;
	{
		unsigned int a, b, c, d, e, W[80] = { 0 };
		for (i = 0; i < 16; ++i)
			for (j = 0; j < 4; ++j)
			{
				if (t[( i << 2 ) + j])
					W[i] |= (unsigned int)t[( i << 2 ) + j] << ( 8 * ( 3 - j ) );
			}
		while (i < 80)
		{
			W[i] = rol(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			++i;
		}
		a = H[0]; b = H[1]; c = H[2]; d = H[3]; e = H[4];
		for (i = 0; i < 80; ++i)
		{
			unsigned int t = rol(a, 5) + F(i, b, c, d) + e + W[i] + K(i);
			e = d; d = c; c = rol(b, 30); b = a; a = t;
		}
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
	}
	for (k = 0; k < n; ++k)
	{
		unsigned int a, b, c, d, e, W[80] = { 0 };
		for (i = 0; i < 16; ++i)
			for (j = 0; j < 4; ++j)
			{
				unsigned long long l = k * 64 + i * 4 + j;
				if (l < len)
				{
					if (src[l])
						W[i] |= (unsigned int)src[l] << ( 8 * ( 3 - j ) );
				}
				else if (l == len)
					W[i] |= 0x80 << ( 8 * ( 3 - j ) );
				else break;
			}
		if (k == n - 1)
		{
			W[14] = ln >> 32 & 0xffffffff;
			W[15] = ln & 0xffffffff;
		}
		while (i < 80)
		{
			W[i] = rol(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			++i;
		}
		a = H[0]; b = H[1]; c = H[2]; d = H[3]; e = H[4];
		for (i = 0; i < 80; ++i)
		{
			unsigned int t = rol(a, 5) + F(i, b, c, d) + e + W[i] + K(i);
			e = d; d = c; c = rol(b, 30); b = a; a = t;
		}
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
	}
	for(i=0;i<5;++i)
	{
		_rol(&H[i],16);
		_rolw((unsigned short*)&H[i],8);
		_rolw((unsigned short*)&H[i]+1,8);
	}
}

void _HMAC(const unsigned char* key, unsigned long long klen, const  unsigned char* data, unsigned long long dlen, unsigned char O[])
{
	unsigned char k0[64] = { 0 }, k1[64 + 20] = { 0 };
	unsigned long long i;
	if (klen > 64)
		_SHA1(key, klen, k0);
	else
		movsb(key, k0, (unsigned int)klen);
	movsb(k0, k1, _countof(k0));
	for (i = 0; i < _countof(k0); i++)
		k0[i] ^= 0x36;
	for (i = 0; i < _countof(k0); i++)
		k1[i] ^= 0x5c;
	_SHA1_ex(k0, data, dlen, k1 + 64);
	_SHA1(k1, _countof(k1)*sizeof( unsigned char ), O);
}

void _PBKDF2(const unsigned char *password, unsigned long long plen, unsigned char *salt, unsigned long long slen, unsigned long long c, unsigned long long dkLen, unsigned char O[])
{
	const unsigned long long hLen = 20;
	unsigned long long l = dkLen / hLen;
	unsigned long long r = dkLen%hLen;
	unsigned long long i,j,k;
	unsigned char f[20],u[20];
	for (i = 1; i <= l+1; ++i)
	{
		*(unsigned int*)(salt+slen) = (unsigned int)i<<24;
		_HMAC(password, plen, salt, slen + 4, u);
		movsb(u, f, _countof(u));
		for (j = 2; j <= c; ++j)
		{
			_HMAC(password, plen, u, sizeof u, u);
			for (k = 0; k < _countof(f); ++k)
				f[k] ^= u[k];
		}
		movsb(f, O + ( i - 1 )*hLen, (i<=l)?(unsigned int)sizeof f:(unsigned int)r);
	}
}
