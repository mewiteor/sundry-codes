#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#include<time.h>

#include"wifi.h"

typedef struct
{
	int start,end;
} Arg;

pthread_t thr[4];
pthread_mutex_t mtx;
pthread_cond_t cond;
unsigned char con=1;
int g_i=-1;


void *f(void *s)
{
	char ssid[36]="zzzzz",pass[64];
	unsigned char O[40]={0},
		OO[32]=
	{
		0x06,0x9A,0x72,0xE7,0x2E,0xCA,0xE4,0xA6,
		0x35,0x0C,0x74,0x2E,0xC8,0x97,0xFB,0xF4,
		0x12,0x20,0x6F,0x84,0x33,0xD2,0xBE,0x8F,
		0x48,0xCA,0x33,0x32,0xD7,0x87,0x21,0x9F
	};
	Arg* arg=(Arg*)s;
	unsigned int i=arg->start,en=arg->end,l=strlen(ssid);
	pthread_cond_signal(&cond);
	while(con&&i<en)
	{
		snprintf(pass,64,"%08u",i);
		wifi(pass,8,ssid,l,O);
		if(!memcmp(O,OO,sizeof OO))
		{
			pthread_mutex_lock(&mtx);
			con=0;
			g_i=i;
			pthread_mutex_unlock(&mtx);
			break;
		}
		if(!(i%100))
		{
			if(i<25000000)
				printf("\033[1;0H\033[K");
			else if(i<50000000)
				printf("\033[2;0H\033[K");
			else if(i<75000000)
				printf("\033[3;0H\033[K");
			else
				printf("\033[4;0H\033[K");
			printf("%d\n",i);
		}
		++i;
	}
	return i;
}

void sigfun(int n)
{
	con=0;
}

int main()
{
	int i=0,iReturn=0,rr[4];
	void *r;
	const struct sigaction sig = {sigfun};
	FILE *file;
	time_t st_time;
	double duration;
	iReturn=sigaction(SIGINT,&sig,NULL);
	if(-1==iReturn)
	{
		perror("sigaction");
		return -1;
	}
	file=fopen("cfg","r");
	if(file==NULL)
	{
		perror("fopen");
		return -1;
	}
	pthread_mutex_init(&mtx,0);
	pthread_cond_init(&cond,0);
	printf("\033[2J\n");
	st_time = time(NULL);
	for(i=0;i<4;++i)
	{
		Arg arg={-1,(i+1)*25000000};
		fscanf(file,"%d",&arg.start);
		rr[i]=arg.start;
		pthread_create(thr+i,NULL,f,&arg);
		pthread_cond_wait(&cond,&mtx);
	}
	fclose(file);
	file=fopen("cfg","w");
	for(i=0;i<4;++i)
	{
		pthread_join(thr[i],&r);
		switch(i)
		{
			case 0:printf("\033[1;0H\033[K");break;
			case 1:printf("\033[2;0H\033[K");break;
			case 2:printf("\033[3;0H\033[K");break;
			case 3:printf("\033[4;0H\033[K");break;
			default:break;
		}
		printf("%d\n",(int)r);
		fprintf(file,"%d\n",(int)r);
		rr[i]=(int)r-rr[i];
	}
	fclose(file);
	duration = difftime(time(NULL),st_time);
	if(g_i>=0)
	{
		printf("\033[Kvalue:%08d\n",g_i);
		file=fopen("val","w");
		fprintf(file,"value:%08d\n",g_i);
		fclose(file);
	}
	for(i=0,iReturn=0;i<4;++i)
		iReturn+=rr[i];
	printf("\033[Kcalculator values per second:%lf\n",iReturn/duration);
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mtx);
	return 0;
}

