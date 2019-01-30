#include"hash.h"

#define COUNT(x) (sizeof(x)/sizeof(*(x)))

void hash::sha1_init(unsigned int *H)
{
	H[0] = 0x67452301;
	H[1] = 0xEFCDAB89;
	H[2] = 0x98BADCFE;
	H[3] = 0x10325476;
	H[4] = 0xC3D2E1F0;
}

void hash::sha1_initW(const unsigned char *s,unsigned int W[])
{
	for(unsigned int i=0;i<16;++i)
		W[i]=s[0]<<24|s[1]<<16|s[2]<<8|s[3];
	for(unsigned int i=16;i<80;++i)
		W[i]=rol(W[i-3]^W[i-8]^W[i-14]^W[i-16],1);
}

void hash::sha1_initW(const unsigned char *s,unsigned int len,unsigned int tlen,unsigned int W[])
{
	stosl(W,0,80);
	for(unsigned int i=0;i<16;++i)
		for(unsigned int j=0;j<4;++j)
		{
			unsigned long long lt=(i<<2)+j;
			if(lt<len)
				W[i]|=*s++<<(8*(3-j));
			else if(lt==len)
				W[i]|=0x80<<(8*(3-j));
			else break;
		}
	unsigned long long ln = tlen<<3;
	W[14]=ln>>32;
	W[15]=ln;
	for(unsigned int i=16;i<80;++i)
		W[i]=rol(W[i-3]^W[i-8]^W[i-14]^W[i-16],1);
}

void hash::sha1_sub(unsigned int *H,unsigned int W[])
{
	unsigned int ae[5];
	movsl(H,ae,5);
	for(unsigned int i=0;i<20;++i)
	{
		unsigned int t=rol(ae[0],5)+((ae[1]&ae[2])|(~ae[1]&ae[3]))+ae[4]+W[i]+0x5A827999;
		ae[4]=ae[3];ae[3]=ae[2];ae[2]=rol(ae[1],30);ae[1]=ae[0];ae[0]=t;
	}

	for(unsigned int i=20;i<40;++i)
	{
		unsigned int t=rol(ae[0],5)+(ae[1]^ae[2]^ae[3])+ae[4]+W[i]+0x6ED9EBA1;
		ae[4]=ae[3];ae[3]=ae[2];ae[2]=rol(ae[1],30);ae[1]=ae[0];ae[0]=t;
	}

	for(unsigned int i=40;i<60;++i)
	{
		unsigned int t=rol(ae[0],5)+((ae[1]&ae[2])|(ae[1]&ae[3])|(ae[2]&ae[3]))+ae[4]+W[i]+0x8F1BBCDC;
		ae[4]=ae[3];ae[3]=ae[2];ae[2]=rol(ae[1],30);ae[1]=ae[0];ae[0]=t;
	}

	for(unsigned int i=60;i<80;++i)
	{
		unsigned int t=rol(ae[0],5)+(ae[1]^ae[2]^ae[3])+ae[4]+W[i]+0xCA62C1D6;
		ae[4]=ae[3];ae[3]=ae[2];ae[2]=rol(ae[1],30);ae[1]=ae[0];ae[0]=t;
	}
	for(unsigned int i=0;i<5;++i)
		H[i]+=ae[i];
}

void hash::reverseH(unsigned int *H)
{
	for(unsigned int i=0;i<5;++i)
	{
		_rol(&H[i],16);
		_rol((unsigned short*)&H[i],8);
		_rol(((unsigned short*)&H[i]+1),8);
	}
}

void hash::sha1(const unsigned char *src,unsigned long long len,unsigned char *out)
{
	unsigned int *H=reinterpret_cast<unsigned int*>(out);
	sha1_init(H);
	unsigned long long n=((len+8)>>6)+1;
	for(unsigned long long i=0;i<n;++i,src+=64)
	{
		unsigned int W[80];
		if(i<n-1)
			sha1_initW(src,W);
		else
			sha1_initW(src,len%64,len,W);
		sha1_sub(H,W);
	}
	reverseH(H);
}

void hash::sha1(const unsigned char *k,const unsigned char *src,unsigned long long len,unsigned char *out)
{
	unsigned int *H=reinterpret_cast<unsigned int*>(out);
	sha1_init(H);
	unsigned long long n=((len+8)>>6)+1;
	unsigned int W[80];
	sha1_initW(k,W);
	sha1_sub(H,W);
	for(unsigned long long i=0;i<n;++i,src+=64)
	{
		if(i<n-1)
			sha1_initW(src,W);
		else
			sha1_initW(src,len%64,len+64,W);
		sha1_sub(H,W);
	}
	reverseH(H);
}

void hash::hmac(const unsigned char *key,unsigned long long klen,const unsigned char *data,unsigned long long dlen,unsigned char *O)
{
	unsigned char k0[64]={0},k1[64+20]={0};
	if(klen>64)
		sha1(key,klen,k0);
	else
		movsb(key,k0,(unsigned int)klen);
	movsb(k0,k1,COUNT(k0));
	xors(reinterpret_cast<unsigned int*>(k0),0x36363636,64/sizeof(unsigned int));
	xors(reinterpret_cast<unsigned int*>(k1),0x5c5c5c5c,64/sizeof(unsigned int));
	sha1(k0,data,dlen,k1+64);
	sha1(k1,sizeof(k1),O);
}

unsigned char *hash::sha1(const unsigned char *src,unsigned long long len)
{
	unsigned char *out=new unsigned char[20];
	sha1(src,len,out);
	return out;
}

unsigned char *hash::hmac(const unsigned char *key,unsigned long long klen,const unsigned char *data,unsigned long long dlen)
{
	unsigned char *o = new unsigned char[20];
	hmac(key,klen,data,dlen,o);
	return o;
}

void hash::pbkdf2(void (*func)(const unsigned char *key,unsigned long long klen,const unsigned char *data,unsigned long long dlen,unsigned char *O),const unsigned char *password,unsigned long long plen,const unsigned char *salt,unsigned long long slen,unsigned long long c,unsigned long long dklen,unsigned long long hlen,unsigned char *O)
{
	unsigned long long l=dklen/hlen,r=dklen%hlen;
	unsigned char *f=new unsigned char[hlen],*u=new unsigned char[hlen];
	unsigned char *_salt = new unsigned char[slen+sizeof(unsigned int)];
	movsb(salt,_salt,slen);
	for(unsigned long long i=1;i<=l+(r?1:0);++i)
	{
		unsigned int* ii = reinterpret_cast<unsigned int*>(_salt+slen);
		*ii=i;
		_rol(ii,16);
		_rol(reinterpret_cast<unsigned short*>(ii),8);
		_rol(reinterpret_cast<unsigned short*>(ii)+1,8);
		func(password,plen,_salt,slen+sizeof(unsigned int),u);
		movsb(u,f,hlen);
		for(unsigned long long j=2;j<c;++j)
		{
			func(password,plen,u,hlen*sizeof(unsigned char),u);
			xors(f,u,hlen);
		}
		movsb(f,O+(i-1)*hlen,i<=l?(unsigned int)(sizeof(unsigned char)*hlen):(unsigned int)r);
	}
	delete[] f;
	delete[] u;
	delete[] _salt;
}

unsigned char *hash::pbkdf2(PBKDF2_HASHFUNC func,const unsigned char *password,unsigned long long plen,const unsigned char *salt,unsigned long long slen,unsigned long long c,unsigned long long dklen)
{
	unsigned char *out = reinterpret_cast<unsigned char*>(0);
	switch(func)
	{
	case HMAC:
		out = new unsigned char[dklen];
		pbkdf2(hmac,password,plen,salt,slen,c,dklen,20,out);
		break;
	default:
		break;
	}
	return out;
}
