/*
HDU3923 Invoker
Polya定理直接套用公式题

Kael有c种颜色的法球，n个可以构成一个环。问构成的不重复环（含翻转）有多少种。

旋转:
n个点顺时针或者逆时针旋转k个位置的置换，循环数为gcd(n,k)
所以一共是Σk,c^gcd(n,k)=Σi,c^i*φ(n/i)（也可以不转换直接用左式算）

翻转：
奇数->n次都是(n/2对对换+1个不动)
偶数->n/2次是(n/2对对换)，n/2次是(2个不动+n/2-1对对换)

合计2n种
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;

typedef signed long long int SINT;
const SINT MODN = 1000000007;

#define PRIMENUM 26
SINT primes[PRIMENUM] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};

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

SINT calc(SINT c, SINT n)
{
	SINT ret = 0;
	SINT i;

	//旋转
	for(i=1;i<=n;i++)
	{
		if(n % i > 0)
			continue;
		ret += ApBmC(c, i, MODN) * Euler(n / i);
		ret %= MODN;
	}
	//翻转
	if((n & 1) == 0)
	{
		ret += ApBmC(c, (n >> 1), MODN) * (c + 1) * (n >> 1);
		ret %= MODN;
	}else{
		ret += ApBmC(c, (n >> 1) + 1, MODN) * n;
		ret %= MODN;
	}
	//除以总变换
	ret *= Divisor(n << 1, MODN);
	ret %= MODN;

	return ret;
}

int main()
{
	int t, a, b, i;

	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		scanf("%d%d",&a,&b);
		printf("Case #%d: %lld\n",i,calc(a, b));
	}
	return 0;
}