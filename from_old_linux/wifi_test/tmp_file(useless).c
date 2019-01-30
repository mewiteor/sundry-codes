#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<malloc.h>

#include"wifi.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
unsigned char con=1;

void *f(void *s)
{
	int n=*(int*)s,i,j;
	char ssid[36]="zzzzz",pass[64];
	unsigned char O[40],OO[32]=
	{
		0x06,0x9A,0x72,0xE7,0x2E,0xCA,0xE4,0xA6,
		0x35,0x0C,0x74,0x2E,0xC8,0x97,0xFB,0xF4,
		0x12,0x20,0x6F,0x84,0x33,0xD2,0xBE,0x8F,
		0x48,0xCA,0x33,0x32,0xD7,0x87,0x21,0x9F
	};
	unsigned long long l = strlen(ssid);
	pthread_cond_signal(&cond);
	for(i=8+14*n;con&&i<8+14+14*n;++i)
	{
		for(j=' ';con&&j<127;++j)
		{
			memset(pass,j,i);
			wifi(pass,i,ssid,l,O);
			if(!memcmp(O,OO,sizeof OO))
			{
				int *_ret=(int *)malloc(sizeof(int)*2);
				pthread_mutex_lock(&mutex);
				con=0;
				pthread_mutex_unlock(&mutex);
				_ret[0]=i;
				_ret[1]=j;
				return _ret;
			}
		}
		printf("%d\n",i);
	}
	return NULL;
}

int main()
{
	int i;
	pthread_t thr[4];
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	for(i=0;i<4;++i)
	{
		pthread_create(thr+i,NULL,f,&i);
		pthread_cond_wait(&cond,&mutex);
	}
	for(i=0;i<4;++i)
	{
		int *n;
		pthread_join(thr[i],(void**)&n);
		if(n)
		{
			char pass[64];
			memset(pass,n[1],n[0]);
			pass[n[0]]='\0';
			printf("password:%s\n",pass);
			free(n);
		}
	}
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	return 0;
}
