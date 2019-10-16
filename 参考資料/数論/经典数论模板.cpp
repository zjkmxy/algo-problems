#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;

typedef signed long long int SINT;

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

long nmod[32];        //n快速取幂用
long fcnt[100];       //n各个素因子幂次
long faci[100];       //n素因子
long fval[100];       //n素因子积
long nfac;            //n素因子个数

//筛法预处理素数（3410个素数基本恰好满足10^9的因子分解）
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

//位运算快速欧几里得
SINT gcd(SINT m, SINT n)
{
	SINT temp;
	if(n > m)
	{
		temp = n;
		n = m;
		m = temp;
	}
	while((n != m)&&(n != 1))
	{

		if((m&1)&&(n&1))
		{
			temp = ((m - n) >> 1);
			if(temp < 0)
				temp = -temp;
			if(n > m)
				n = m;
			m = temp;
			continue;
		}else	if(m&1)
		{
			n >>= 1;
			continue;
		}else	if(n&1)
		{
			m >>= 1;
			continue;
		}else
		{
			return (gcd(m >> 1, n >> 1)<<1);
		}
	}
	return n;
}

//扩展欧几里得算法
SINT ExtendGcd(SINT a,SINT b,SINT &x,SINT &y)
{
	SINT d,tmp;

	if(b==0)
	{
		x = 1;
		y = 0;
		return a;
	}
	d = ExtendGcd(b,a%b,x,y);
	tmp = x;
	x = y;
	y = tmp - (a/b)*y;

	return d;
}

//x对y乘法逆元
SINT Divisor(SINT x, SINT y)
{
	SINT d, ret, tmp;

	d = ExtendGcd(x,y,ret,tmp);
	//不互质则无解
	if(d != 1)  
		return -1;
	//变成最小正
	ret = (ret % y + y) % y;
	return ret;
}

//欧拉函数
SINT Euler(SINT x)
{
	int i;
	SINT ret = x, n = x;

	if(x == 0)
		return 0;
	if(x == 1)
		return 1;
	for(i=0;(i<PRIMENUM)&&(primes[i]<=n);i++)  //这里其实只需要平方小于n就够了
	{
		if(n % primes[i] > 0)
			continue;
		ret = ret / primes[i] * (primes[i] - 1);
		while(n % primes[i] == 0)
			n /= primes[i];
	}
	//剩余质数
	if(n > 1)
		ret = ret / n * (n - 1);
	return ret;
}

//乘法取模（防越位）
SINT mult_mod(SINT a, SINT b, SINT MODN)
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

//快速模取幂
SINT ApBmC(SINT A, SINT B, SINT C)
{
	SINT ret = 1;

	if(C == 1)
		return 0;
	if(B == 0)
		return 1;

	A = A % C;

	while(B >= 1)
	{
		if((B & 1) == 1)
		{
			ret = (ret * A) % C;
		}

		B >>= 1; 
		A = (A * A) % C;
	}

	return ret;
}

//大B优化快速模取幂
SINT ApBmCH(SINT A, SINT B, SINT C)
{
	int ret = 1, d, p;

	if(C == 1)
		return 0;
	if(B == 0)
		return 1;

	A = A % C;
	d = gcd(A, C);
	p = Euler(C / d);
	if(B > p)
	{
		B %= p;
		ret = ApBmC(A, B, C);
		if(d > 1)
		{
			ret *= ApBmC(d, p, C);    //A^B = A^(B % p)*A^p (mod C)
			ret %= C;
		}
	}else{
		ret = ApBmC(A, B, C);
	}

	return ret;
}

//DFS(0,1,1)遍历fact分解数的因子
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
		//这里进行最后的计算
	}
}

//模线性方程ax=b(mod n)的最小正解（会做n/=d，以后全部解是ret加上新的n*i）
SINT SolvEqu(SINT a, SINT b, SINT &n)
{
	long long int x,y,d,min,i;

	d = ExtendGcd(a, n, &x, &y);
	if(d < 0)
		d = -d;
	if(b % d != 0)
	{
		return -1;
	}
	b /= d;
	n /= d;
	x *= b;
	i = -x / n;   //因为x+n*i(i=0..d-1)是全部解，所以令其为0的i就可以寻找最小正解
	min = x + n * i;
	if(min < 0)
		min += n;
	
	return min;
}
