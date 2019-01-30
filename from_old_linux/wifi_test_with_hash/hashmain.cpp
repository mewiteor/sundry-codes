#include<iostream>
#include<string>
#include<cstdio>
#include"hash.h"
using namespace std;
int main()
{
	string s;
	cin>>s;
	unsigned char *o=hash::sha1(reinterpret_cast<const unsigned char*>(s.c_str()),s.length());
	for(int i=0;i<20;++i)
		printf("%02X",o[i]);
	puts("");
	delete[] o;
	return 0;
}

