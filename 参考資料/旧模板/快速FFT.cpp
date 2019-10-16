const double PI = 3.1415926535897932384626;

struct COMP
{
    double re;
    double im;

    COMP(double re=0.0, double im=0.0)
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
        double divs = other.re*other.re + other.im*other.im;
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
============================================================
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
inline uint32 mult_mod(uint32 a, uint32 b, uint32 p)
{
    uint64 A(a), B(b);
	return (A * B % p);
}

//快速模取幂
uint32 ApBmC(uint32 A, uint32 B, uint32 C)
{
    uint32 ret = 1;

    A = A % C;
    while(B >= 1)
    {
        if((B & 1) == 1)
            ret = mult_mod(ret, A, C);
        B >>= 1; 
        A = mult_mod(A, A, C);
    }
    return ret;
}

inline uint32 add_mod(uint32 a, uint32 b, uint32 p)
{
    uint32 n = a + b;
    if (n < a)
        n -= p;
    else if (n >= p)
        n -= p;
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