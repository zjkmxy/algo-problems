#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
using namespace std;

#define MAXN 0x00200000
#define MODN 0xC0000001
#define MAXM 21
#define FAC0 1948709276
#define SPEC 0x30000000

typedef long long LLI;
typedef unsigned long long uint64;
typedef unsigned long uint32;

int maxn, maxm;
uint32 fp_tab[MAXN]; //幂次表


//乘法取模
uint32 mult_mod(uint32 a, uint32 b, uint32 p)
{
    uint32 _a = a;
    uint32 _b = b;   

    __asm
    {
    mov eax, a;
    mul b;
    mov ebx, eax;
    mov eax, 2863311530;
    mov ecx, edx;
    mul edx;
    shld edx, eax, 1;
    mov eax, 3221225473;

    mul edx;
    sub ebx, eax;
    mov eax, 3221225473;
    sbb ecx, edx;
    jc addback;

            neg ecx;
            and ecx, eax;
            sub ebx, ecx;

    sub ebx, eax;
    sbb edx, edx;
    and eax, edx;
            addback:
    add eax, ebx;          
    };  
}

//快速模取幂
uint32 ApBmC(uint32 A, uint32 B, uint32 C)
{
    uint32 ret = 1;

    A = A % C;
    while(B >= 1)
    {
        if((B & 1) == 1)
        {
            ret = mult_mod(ret, A, C);
        }

        B >>= 1; 
        A = mult_mod(A, A, C);
    }

    return ret;
}

uint32 add_mod(uint32 a, uint32 b, uint32 p)
{
    uint32 n = a + b;
    if (n < a)
    {
        n -= p;
    }
    else if (n >= p)
    {
        n -= p;
    }
    return n;
}

inline uint32 sub_mod(uint32 a, uint32 b, uint32 p)
{
    return (a < b)?(a+p-b):(a-b);
}

void init(int mm)
{
    int i;
    uint32 f0, l;

    fp_tab[0] = 1;
    maxm = mm;
    maxn = (1<<mm);
    l = SPEC / maxn;
    f0 = ApBmC(2, l, MODN);
    for(i=1;i<=maxn;i++)
        fp_tab[i] = mult_mod(fp_tab[i-1], f0, MODN);
}

void FNTT(int M, uint32 seq_in[], uint32 seq_out[], bool tran)
{
    int n = (1<<M);
    int flg, flg2;
    int i, j, k, tmp, p;
    uint32 t1, t2;
    uint32 modn = MODN;
    uint32 divs = ApBmC(n, MODN-2, MODN);

    //倒位
    tmp = 0;
    for(i=0;i<n;i++)
    {
        seq_out[i] = seq_in[tmp];
        if(i==n-1)
            break;
        j = M - 1;
        while(tmp & (1<<j))
        {
            tmp ^= (1<<j);
            j--;
        }
        tmp|= (1<<j);
    }

    //计算
    for(i=0,tmp=1,flg=1<<(maxm-1);i<M;i++,tmp<<=1,flg>>=1)
    {
        for(j=0;j<n;j+=(tmp<<1))
        {
			flg2 = tran ? 0 : maxn;
            for(k=0;k<tmp;k++)
            {
                p = j + k;
                t1 = seq_out[p];
                t2 = mult_mod(seq_out[p|tmp], fp_tab[flg2], modn);
				if(tran)
                {
					flg2 += flg;
                }else{
					flg2 -= flg;
                }
                seq_out[p] = add_mod(t1, t2, modn);
                seq_out[p|tmp] = sub_mod(t1, t2, modn);
            }
        }
    }

    if(!tran)
    {
        for(i=0;i<n;i++)
            seq_out[i] = mult_mod(seq_out[i], divs, modn);
    }
}
//快速卷积计算
void volv(int n, uint32 in_a[], uint32 in_b[], uint32 out[])
{
    int m = 0, i;
    uint32 *FA, *FB;
    n = (n << 1) - 1;
    while(n > 0)
    {
        n>>=1;
        m++;
    }
    n = (1 << m);
    FA = new uint32[n];
    FB = new uint32[n];

    FNTT(m, in_a, FA, true);
    FNTT(m, in_b, FB, true);
    for(i=0;i<n;i++)
        FA[i] = mult_mod(FA[i], FB[i], MODN);
    FNTT(m, FA, out, false);

    delete[] FA;
    delete[] FB;
}

///////////////////////

uint32 x1[200005], x2[200005], x[200005];
uint32 f1[200005], f2[200005], f[200005];

int main()
{
    char a[100005],b[100005];
    int i,len1,len2,len,loglen;
    int t,over;

    init(20);
    while(scanf("%s%s",a,b)!=EOF)
    {
        len1=strlen(a)<<1;
        len2=strlen(b)<<1;
        len=1;
        loglen=0;
        while(len<len1)
            len<<=1, loglen++;
        while(len<len2)
            len<<=1, loglen++;
        for(i=0;a[i];i++)
            x1[i]=a[i]-'0';
        for(;i<len;i++)
            x1[i]=0;
        for(i=0;b[i];i++)
            x2[i]=b[i]-'0';
        for(;i<len;i++)
            x2[i]=0;
        
        FNTT(loglen, x1, f1, true);
        FNTT(loglen, x2, f2, true);

        for(i=0;i<len;i++)
            f[i] = mult_mod(f1[i], f2[i], MODN);
        FNTT(loglen, f, x, false);
        
        for(i=(len1+len2)/2-2,over=len=0;i>=0;i--)
        {
            t=x[i]+over;
            a[len++]= t%10;
            over = t/10;
        }
        while(over){
            a[len++]=over%10;
            over/=10;
        }
        
        for(len--;len>=0&&!a[len];len--);
        if(len<0)
            putchar('0');
        else
            for(;len>=0;len--)
                putchar(a[len]+'0');
        putchar('\n');
    }
    return 0;
}