#ifndef _ASMS_H_
#define _ASMS_H_ 1

unsigned int rol(unsigned int a,unsigned char l);

void _rol(unsigned int *a,unsigned char l);

void _rol(unsigned short *a,unsigned char l);

void movsb(const unsigned char *s,unsigned char *d,unsigned int len);

void movsl(const unsigned int *s,unsigned int *d,unsigned int len);

void stosl(unsigned int *d,unsigned int val,unsigned int len);

void xors(unsigned char *s,unsigned char val,unsigned int len);

void xors(unsigned short *s,unsigned short val,unsigned int len);

void xors(unsigned int *s,unsigned int val,unsigned int len);

void xors(unsigned char *s,unsigned char *v,unsigned int len);

void xors(unsigned short *s,unsigned short *v,unsigned int len);

void xors(unsigned int *s,unsigned int *v,unsigned int len);

#endif // _ASMS_H_
