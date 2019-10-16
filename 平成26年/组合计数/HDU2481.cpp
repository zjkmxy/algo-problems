/*
HDU2481 Toy

非常难的一道题，用Burnside原理+不重复计数。
getd的内容见论文《若干图类的生成树数》。结果是W[n]=4W[n-1]-4W[n-2]+W[n-3]
因为m不确定，所以提前乘好n，最后直接除n即可。
ans / n % m = (ans % (m * n) / n) % m
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

typedef signed long long int SINT;
typedef SINT MATRIX[3][3];
typedef SINT VECTOR[3];

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

long nmod[32];        //n快速取幂用
long fcnt[100];       //n各个素因子幂次
long faci[100];       //n素因子
long fval[100];       //n素因子积
long nfac;            //n素因子个数
SINT ans;             //最终答案

int n, m;
SINT MODN;
MATRIX mat[32];
//const VECTOR W0 = {0, 1, 5};  //W0,W1,W2

//乘法取模（防越位）
SINT mult_mod(SINT a, SINT b)
{
	SINT ret = 0, tmp = a;

	if(b<0)
		b += MODN;
	if(tmp<0)
		tmp += MODN;
	while(b)
	{
		if(b&0x1)
		{
			ret += tmp;
			if(ret >= MODN)
				ret -= MODN;
		}
		tmp <<= 1;
		if(tmp >= MODN)
			tmp -= MODN;
		b>>=1;
	}
	return ret;
}

//矩阵处理
void mul(MATRIX mata, MATRIX matb, MATRIX prod)
{
	int i, j, k;
	SINT sum;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			sum = 0;
			for(k=0;k<3;k++)
			{
				sum += mult_mod(mata[i][k], matb[k][j]);
			}
			prod[i][j] = (sum % MODN);
		}
	}
}
void matpow_init()
{
	int i;
	for(i=1;i<32;i++)
	{
		mul(mat[i-1], mat[i-1], mat[i]);
	}
}
void mul_vec(VECTOR a, MATRIX b, VECTOR prod)
{
	int i, j;
	SINT sum;
	for(i=0;i<3;i++)
	{
		sum = 0;
		for(j=0;j<3;j++)
		{
			sum += mult_mod(a[j], b[j][i]);
		}
		prod[i] = sum % MODN;
	}
}


//筛法预处理素数
void init_prime()
{
	int i, j = 0, k = 0;
	for(i=2;k<PRIMENUM;i++)
	{
		if(notprime[i])
			continue;

		for(j=i+i; j < 32000; j+=i)
		{
			notprime[j] = true;
		}
		primes[k] = i;
		k++;
	}
}

//因子分解
void fact(int x)
{
	int i;

	nfac = 0;
	for(i=0;(i<PRIMENUM)&&(x>1);i++)
	{
		if(x % primes[i] == 0)
		{
			faci[nfac] = primes[i];
			fcnt[nfac] = 0;
			fval[nfac] = 1;
			do{
				x /= primes[i];
				fval[nfac] *= primes[i];
				fcnt[nfac]++;
			}while(x % primes[i] == 0);
			nfac++;
		}
	}
	if(x > 1)  //x剩余自身为素数（这个很容易忘）
	{
		faci[nfac] = x;
		fcnt[nfac] = 1;
		fval[nfac] = x;
		nfac++;
	}
}

SINT getd(int cnt)  
{
	VECTOR temp[2];
	int i, cur = 0;
	SINT ret = 0;

	memset(temp, 0, sizeof(temp));
	temp[cur][0] = 0;
	temp[cur][1] = 1;
	temp[cur][2] = 5;
	for(i=0;cnt>0;i++,cnt>>=1)
	{
		if(cnt & 1)
		{
			mul_vec(temp[cur], mat[i], temp[cur^1]);
			cur ^= 1;
		}
	}
	ret = temp[cur][0];

	return ret;
}

void DFS(int pfi, int fc, int phi)  //素因子序号，当前因子，n/i的欧拉函数值
{
	int i, p;

	if(pfi < nfac)
	{
		for(i=0,p=fval[pfi];i<=fcnt[pfi];i++,p/=faci[pfi])
		{
			DFS(pfi + 1, fc * (fval[pfi] / p), phi * (p - p / faci[pfi]));
		}
	}else{
		ans += mult_mod(getd(fc), phi);
		ans %= MODN;
	}
}

int main()
{
	init_prime();
	while(scanf("%d%d",&n, &m)!=EOF)
	{
		MODN = (SINT)n * (SINT)m;  //预先乘好了，到时候直接除

		memset(mat[0], 0, sizeof(mat[0]));
		mat[0][0][2] = 1;
		mat[0][1][0] = 1;
		mat[0][1][2] = -4;
		mat[0][2][1] = 1;
		mat[0][2][2] = 4;
		matpow_init();

		ans = 0;
		fact(n);
		DFS(0, 1, 1);
		ans = (ans / n % m + m) % m;
		printf("%lld\n",ans);
	}
	return 0;
}
