#include<iostream>
#include<cmath>
using namespace std;
typedef double SIGNAL;
const SIGNAL PI = 3.1415926535897932384626;

struct COMP
{
    SIGNAL re;
    SIGNAL im;

    COMP(SIGNAL re=0.0, SIGNAL im=0.0)
    {
        this->re = re;
        this->im = im;
    };
	//注意：这四个&是为了加速！
    COMP operator+(const COMP &other)const
    {
        return COMP(this->re+other.re, this->im+other.im);
    };
    COMP operator-(const COMP &other)const
    {
        return COMP(this->re-other.re, this->im-other.im);
    };
    COMP operator*(const COMP &other)const
    {
        return COMP(this->re*other.re - this->im*other.im, this->im*other.re + this->re*other.im);
    };
    COMP operator/(const COMP &other)const
    {
        SIGNAL divs = other.re*other.re + other.im*other.im;
        return COMP((this->re*other.re + this->im*other.im) / divs, (this->im*other.re - this->re*other.im) / divs);
    };
};

//M，FFT次数；
void FFT(int M, COMP seq_in[], COMP seq_out[], bool tran)
{
	int n = (1<<M);
    int i, j, k, tmp, p;
    COMP t1, t2, wn, wo;

    //倒位
    tmp = 0;
    for(i=0;i<n;i++)
    {
        seq_out[i] = COMP(seq_in[tmp]);
        if(i==n-1)
            break;
        for(j=1<<(M-1);(tmp&j);j>>=1)
            tmp ^= j;
        tmp|= j;
    }

    //计算
    for(i=0,tmp=1;i<M;i++,tmp<<=1)
    {
        for(j=0;j<n;j+=(tmp<<1))
        {
            wn = COMP(1, 0);
            wo = COMP(cos(PI / tmp), tran ? -sin(PI / tmp) : sin(PI / tmp));
            for(k=0;k<tmp;k++)
            {
                p = j + k;
                t1 = seq_out[p];
                t2 = seq_out[p|tmp] * wn;
                seq_out[p] = t1 + t2;
                seq_out[p|tmp] = t1 - t2;
                wn = wn * wo;
            }
        }
    }
	//复原
	if(!tran)
	{
		for(i=0;i<n;i++)
		{
			seq_out[i].re /= n;
			seq_out[i].im /= n;
		}
	}
}

COMP x1[200005], x2[200005], x[200005];
COMP f1[200005], f2[200005], f[200005];

int main()
{
    char a[100005],b[100005];
    int i,len1,len2,len,loglen;
    int t,over;
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
        //init
        for(i=0;a[i];i++)
            x1[i]=COMP(a[i]-'0');
        for(;i<len;i++)
            x1[i]=COMP(0);
        for(i=0;b[i];i++)
            x2[i]=COMP(b[i]-'0');
        for(;i<len;i++)
            x2[i]=COMP(0);
        
        FFT(loglen, x1, f1, true);
        FFT(loglen, x2, f2, true);

        for(i=0;i<len;i++)
            f[i] = f1[i]*f2[i];
        FFT(loglen, f, x, false);
        
        //将x1.re从浮点数转化为十进制整型存入a
        for(i=(len1+len2)/2-2,over=len=0;i>=0;i--)
        {
            t=x[i].re+over+0.5;
            a[len++]= t%10;
            over = t/10;
        }
        while(over){
            a[len++]=over%10;
            over/=10;
        }
        
        //output
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