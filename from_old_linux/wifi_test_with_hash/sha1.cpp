#include<iostream>
#include<cstdio>
#include<cstring>
#include"hash.h"

using namespace std;

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		cerr<<argv[0]<<" xxx"<<endl;
		return 1;
	}
	unsigned char* o = hash::sha1(reinterpret_cast<const unsigned char*>(argv[1]),strlen(argv[1]));
	for(unsigned int i=0;i<20;++i)
		printf("%02X",o[i]);
	puts("");
	delete[] o;
	return 0;
}
