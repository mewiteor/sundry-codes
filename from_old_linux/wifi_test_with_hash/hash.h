#ifndef _HASH_H_
#define _HASH_H_ 1

#include"asms.h"

enum PBKDF2_HASHFUNC
{
	HMAC
};

class hash
{
private:
	static void sha1_init(unsigned int *H);
	static void sha1_initW(const unsigned char *s,unsigned int W[]);
	static void sha1_initW(const unsigned char *s,unsigned int len,unsigned int tlen,unsigned int W[]);
	static void sha1_sub(unsigned int *H,unsigned int W[]);
	static void reverseH(unsigned int *H);
	static void sha1(const unsigned char *src,unsigned long long len,unsigned char *out);
	static void sha1(const unsigned char *k,const unsigned char *src,unsigned long long len,unsigned char *out);
	static void hmac(const unsigned char *key,unsigned long long klen,const unsigned char *data,unsigned long long dlen,unsigned char *O);
	static void pbkdf2(void (*func)(const unsigned char *key,unsigned long long klen,const unsigned char *data,unsigned long long dlen,unsigned char *O),const unsigned char *password,unsigned long long plen,const unsigned char *salt,unsigned long long slen,unsigned long long c,unsigned long long dklen,unsigned long long hlen,unsigned char *O);
public:
	static unsigned char *sha1(const unsigned char *src,unsigned long long len);
	static unsigned char *hmac(const unsigned char *key,unsigned long long klen,const unsigned char *data,unsigned long long dlen);
	static unsigned char *pbkdf2(PBKDF2_HASHFUNC func,const unsigned char *password,unsigned long long plen,const unsigned char *salt,unsigned long long slen,unsigned long long c,unsigned long long dklen);
};
#endif
