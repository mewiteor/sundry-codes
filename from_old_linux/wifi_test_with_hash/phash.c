#include<stdio.h>
#include<string.h>

#include"p.h"

int main()
{
	unsigned char H[20];
	unsigned char src[256];
	unsigned int i;
	scanf("%[^\n]",src);
	_SHA1(src,strlen(src),H);
	for(i=0;i<20;++i)
		printf("%02X",H[i]);
	puts("");
	return 0;
}
