#include <stdio.h>
#include <gmp.h>
#include <time.h>
int main()
{
    unsigned long long m[8],a[8];
    mpz_t Mi[8],M,mi[8],ai[8];
    size_t i,j;
    clock_t tim;
    for(i=0;i<8;++i)
        scanf("%llu",m+i);
    for(i=0;i<8;++i)
        scanf("%llu",a+i);
    tim=clock();
    for(i=0;i<8;++i)
    {
        mpz_init(mi[i]);
        mpz_import(mi[i],1,-1,sizeof m[0],0,0,m+i);
    }
    for(i=0;i<8;++i)
    {
        mpz_init(ai[i]);
        mpz_import(ai[i],1,-1,sizeof a[0],0,0,a+i);
    }
    mpz_init_set_ui(M,1);
    for(i=0;i<8;++i)
        mpz_mul(M,M,mi[i]);
    for(i=0;i<8;++i)
    {
        mpz_init_set_ui(Mi[i],1);
        for(j=0;j<8;++j)
            if(j!=i)
                mpz_mul(Mi[i],Mi[i],mi[j]);
    }
    for(i=0;i<8;++i)
    {
        mpz_t g,s;
        mpz_inits(g,s,NULL);
        mpz_gcdext(g,s,NULL,Mi[i],mi[i]);
        mpz_mul(Mi[i],Mi[i],s);
        mpz_mul(Mi[i],Mi[i],ai[i]);
        mpz_clears(g,s,NULL);
    }
    for(i=1;i<8;++i)
        mpz_add(Mi[0],Mi[0],Mi[i]);
    mpz_fdiv_r(Mi[0],Mi[0],M);
    j=0;
    for(i=1;i<8;++i)
        if(mi[j]<mi[i])
            j=i;
    if(mpz_cmp(Mi[0],mi[j])<0)
        mpz_add(Mi[0],Mi[0],M);
    mpz_out_str(stdout,10,Mi[0]);
    for(i=0;i<8;++i)
        mpz_clear(mi[i]);
    for(i=0;i<8;++i)
        mpz_clear(ai[i]);
    for(i=0;i<8;++i)
        mpz_clear(Mi[i]);
    mpz_clear(M);
    putchar('\n');
    fprintf(stderr,"%f\n",(double)(clock()-tim)/CLOCKS_PER_SEC);
    return 0;
}
