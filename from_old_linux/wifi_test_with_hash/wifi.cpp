#include<iostream>
#include<cstdio>
#include<cstring>

#include"hash.h"

using namespace std;

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		cerr<<argv[0]<<" ssid key"<<endl;
		return 1;
	}
	unsigned char *o=hash::pbkdf2(HMAC,reinterpret_cast<const unsigned char*>(argv[2]),strlen(argv[2]),reinterpret_cast<const unsigned char*>(argv[1]),strlen(argv[1]),4096,32);
	if(o)
	{
		for(unsigned int i=0;i<32;++i)
			printf("%02X",o[i]);
		puts("");
		delete[] o;
	}
	return 0;
}
