#include <stdio.h>
#include <assert.h>
#include <string.h>
#define DEBUG
#ifdef DEBUG
#include <time.h>
#endif
enum{LEN=32};
// a+=b
void add(unsigned int*a,size_t *asz,const unsigned int*b,size_t bsz);
// a*=b
void mul(unsigned int*a,size_t *size,unsigned long long b);
// a%=b
void rem(unsigned int *a,size_t *asz,const unsigned int*b,size_t bsz);
// ax+by=g -> return x
unsigned long long exgcd(const unsigned int*a,size_t size,unsigned long long b);
int main()
{
    unsigned long long mi[8],ai[8],result;
    unsigned int Mi[8][LEN],M[LEN];
    size_t i,j,Mi_size[8]={0},M_size=0;
#ifdef DEBUG
    clock_t tim;
#endif
    for(i=0;i<8;++i)
        scanf("%llu",mi+i);
    for(i=0;i<8;++i)
        scanf("%llu",ai+i);
#ifdef DEBUG
    tim=clock();
#endif
    for(i=0;i<8;++i)
        mul(M,&M_size,mi[i]);
    for(i=0;i<8;++i)
        for(j=0;j<8;++j)
            if(i!=j)
                mul(Mi[i],Mi_size+i,mi[j]);
    for(i=0;i<8;++i)
    {
        unsigned long long x=exgcd(Mi[i],Mi_size[i],mi[i]);
        mul(Mi[i],Mi_size+i,x);
        mul(Mi[i],Mi_size+i,ai[i]);
    }
    for(i=1;i<8;++i)
        add(Mi[0],Mi_size,Mi[i],Mi_size[i]);
    rem(Mi[0],Mi_size,M,M_size);
    assert(Mi_size[0]<=2);
    switch(Mi_size[0])
    {
        case 0:result=0;break;
        case 1:result=Mi[0][0];break;
        case 2:result=*(unsigned long long*)Mi[0];break;
    }
    if(M_size<=2)
    {
        for(i=1;i<8;++i)
            if(mi[0]<mi[i])
                mi[0]=mi[i];
        if(result<mi[0])
        {
            unsigned long long MM=0;
            switch(M_size)
            {
                case 0:MM=0;break;
                case 1:MM=M[0];break;
                case 2:MM=*(unsigned long long*)M;break;
            }
            result+=MM;
        }
    }
    printf("%llu\n",result);
#ifdef DEBUG
    printf("time:%gs\n",(double)(clock()-tim)/CLOCKS_PER_SEC);
#endif
    return 0;
}
void add(unsigned int*a,size_t *asz,const unsigned int*b,size_t bsz)
{
    unsigned int v[LEN]={0};
    size_t vsz=0;
    unsigned long long r=0;
    while(vsz<*asz||vsz<bsz)
    {
        if(vsz<*asz)
            r+=a[vsz];
        if(vsz<bsz)
            r+=b[vsz];
        v[vsz++]=(unsigned int)r;
        r>>=32;
    }
    if(r)
        v[vsz++]=(unsigned int)r;
    memcpy(a,v,sizeof v);
    *asz=vsz;
    assert(*asz<LEN);
}
void mul(unsigned int*a,size_t *size,unsigned long long b)
{
    unsigned long long r=0;
    size_t i;
    if(!*size)
    {
        *(unsigned long long*)a=b;
        *size=b>>32?2:1;
    }
    else if(b>>32)
    {
        unsigned long long c=(unsigned int)b;
        b>>=32;
        for(i=0;i<*size;++i)
        {
            unsigned int t[3]={0};
            *(unsigned long long*)t=a[i]*c+(r&-1U);
            *(unsigned long long*)(t+1)+=a[i]*b+(r>>32);
            a[i]=t[0];
            r=*(unsigned long long*)(t+1);
        }
        if(r)
        {
            *(unsigned long long*)(a+*size)=r;
            *size+=r>>32?2:1;
        }
    }
    else
    {
        for(i=0;i<*size;++i)
        {
            r+=a[i]*b;
            a[i]=(unsigned int)r;
            r>>=32;
        }
        if(r)
            a[(*size)++]=(unsigned int)r;
    }
    assert(*size<LEN);
}
void rem(unsigned int *a,size_t *asz,const unsigned int*b,size_t bsz)
{
    unsigned int r[LEN];
    size_t rsz=0,l=*asz<<5,i;
    while(l--)
    {
        if(!rsz)
        {
            if(a[l>>5]>>(l&31)&1)
                r[rsz++]=1;
        }
        else
        {
            i=rsz-1;
            if(r[rsz-1]>>31)
                r[rsz++]=1;
            while(i)
            {
                r[i]=r[i]<<1|r[i-1]>>31;
                --i;
            }
            r[0]=(r[0]<<1)|((a[l>>5]>>(l&31))&1);
        }
        if(rsz>bsz)
        {
            for(i=bsz;i;--i)
               *(unsigned long long*)(r+i-1)-=b[i-1];
            while(rsz&&!r[rsz-1]) --rsz;
        }
        else if(rsz&&rsz==bsz)
        {
            for(i=rsz;i&&r[i-1]==b[i-1];--i);
            if(!i||r[i-1]>b[i-1])
            {
                r[rsz-1]-=b[bsz-1];
                for(i=rsz-1;i;--i)
                    *(unsigned long long*)(r+i-1)-=b[i-1];
                while(rsz&&!r[rsz-1]) --rsz;
            }
        }
    }
    memcpy(a,r,sizeof r);
    *asz=rsz;
    assert(*asz<LEN);
}
unsigned long long exgcd(const unsigned int*a,size_t size,unsigned long long b)
{
    long long x[2]={0,1};
    unsigned long long aa=b,bb=0,r;
    if(1==size)
        bb=a[0]%b;
    else
    {
        unsigned int tmp[LEN];
        size_t tsz=size;
        memcpy(tmp,a,sizeof tmp);
        rem(tmp,&tsz,(unsigned int*)&b,b>>32?2:1);
        assert(tsz<=2);
        switch(tsz)
        {
            case 0:bb=0;break;
            case 1:bb=*tmp;break;
            case 2:bb=*(unsigned long long*)tmp;break;
        }
    }
    do
    {
        long long t=x[0];x[0]=x[1];x[1]=t-aa/bb*x[0];
        r=aa%bb;aa=bb;bb=r;
    }while(bb);
    if(x[0]>=0)
        return (unsigned long long)x[0];
    else
        return (unsigned long long)((long long)b+x[0]);
}
