#include"asms.h"

unsigned int rol(unsigned int a,unsigned char l)
{
	register unsigned int _ret=0;
	__asm__ __volatile__(
		"rol %%cl,%%eax"
		:"=a"(_ret)
		:"a"(a),"c"(l)
	);
	return _ret;
}

void _rol(unsigned int *a,unsigned char l)
{
	__asm__ __volatile__(
		"rol %%cl,%%eax"
		:"=a"(*a)
		:"0"(*a),"c"(l)
	);
}

void _rol(unsigned short *a,unsigned char l)
{
	__asm__ __volatile__(
		"rol %%cl,%%ax"
		:"=a"(*a)
		:"0"(*a),"c"(l)
	);
}

void movsb(const unsigned char *s,unsigned char *d,unsigned int len)
{
	__asm__ __volatile__(
		"cld\n"
		"rep movsb"
		:
		:"S"(s),"D"(d),"c"(len)
	);
}

void movsl(const unsigned int *s,unsigned int *d,unsigned int len)
{
	__asm__ __volatile__(
		"cld\n"
		"rep movsl"
		:
		:"S"(s),"D"(d),"c"(len)
	);
}

void stosl(unsigned int *d,unsigned int val,unsigned int len)
{
	__asm__ __volatile__(
		"cld\n"
		"rep stosl"
		:
		:"D"(d),"a"(val),"c"(len)
	);
}

void xors(unsigned char *s,unsigned char val,unsigned int len)
{
	__asm__ __volatile__(
		"1:\n"
		"xor %%al,-1(%0,%%rcx,1)\n"
		"loop 1b"
		:
		:"S"(s),"c"(len),"a"(val)
	);
}

void xors(unsigned short *s,unsigned short val,unsigned int len)
{
	__asm__ __volatile__(
		"1:\n"
		"xor %%ax,-2(%0,%%rcx,2)\n"
		"loop 1b"
		:
		:"S"(s),"c"(len),"a"(val)
	);
}

void xors(unsigned int *s,unsigned int val,unsigned int len)
{
	__asm__ __volatile__(
		"1:\n"
		"xor %%eax,-4(%0,%%rcx,4)\n"
		"loop 1b"
		:
		:"S"(s),"c"(len),"a"(val)
	);
}

void xors(unsigned int *s,unsigned int *v,unsigned int len)
{
	__asm__ __volatile__(
	 	"1:\n"
	 	"movl -4(%0,%%rcx,4),%%eax\n"
	 	"xor %%eax,-4(%1,%%rcx,4)\n"
	 	"loop 1b"
	 	:
	 	:"S"(v),"D"(s),"c"(len)
	 	:"%eax"
	);
}

void xors(unsigned char *s,unsigned char *v,unsigned int len)
{
	__asm__ __volatile__(
	 	"1:\n"
	 	"movb -1(%0,%%rcx,1),%%al\n"
	 	"xor %%al,-1(%1,%%rcx,1)\n"
	 	"loop 1b"
	 	:
	 	:"S"(v),"D"(s),"c"(len)
	 	:"%al"
	);
}

void xors(unsigned short *s,unsigned short *v,unsigned int len)
{
	__asm__ __volatile__(
	 	"1:\n"
	 	"movw -2(%0,%%rcx,2),%%ax\n"
	 	"xor %%ax,-2(%1,%%rcx,2)\n"
	 	"loop 1b"
	 	:
	 	:"S"(v),"D"(s),"c"(len)
	 	:"%ax"
	);
}
