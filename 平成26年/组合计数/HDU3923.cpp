/*
HDU3923 Invoker
Polya����ֱ�����ù�ʽ��

Kael��c����ɫ�ķ���n�����Թ���һ�������ʹ��ɵĲ��ظ���������ת���ж����֡�

��ת:
n����˳ʱ�������ʱ����תk��λ�õ��û���ѭ����Ϊgcd(n,k)
����һ���Ǧ�k,c^gcd(n,k)=��i,c^i*��(n/i)��Ҳ���Բ�ת��ֱ������ʽ�㣩

��ת��
����->n�ζ���(n/2�ԶԻ�+1������)
ż��->n/2����(n/2�ԶԻ�)��n/2����(2������+n/2-1�ԶԻ�)

�ϼ�2n��
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;

typedef signed long long int SINT;
const SINT MODN = 1000000007;

#define PRIMENUM 26
SINT primes[PRIMENUM] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};

//��չŷ������㷨
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

//x��y�˷���Ԫ
SINT Divisor(SINT x, SINT y)
{
	SINT d, ret, tmp;

	d = ExtendGcd(x,y,ret,tmp);
	//���������޽�
	if(d != 1)  
		return -1;
	//�����С��
	ret = (ret % y + y) % y;
	return ret;
}

//ŷ������
SINT Euler(SINT x)
{
	int i;
	SINT ret = x, n = x;

	if(x == 0)
		return 0;
	if(x == 1)
		return 1;
	for(i=0;(i<PRIMENUM)&&(primes[i]<=n);i++)  //������ʵֻ��Ҫƽ��С��n�͹���
	{
		if(n % primes[i] > 0)
			continue;
		ret = ret / primes[i] * (primes[i] - 1);
		while(n % primes[i] == 0)
			n /= primes[i];
	}
	//ʣ������
	if(n > 1)
		ret = ret / n * (n - 1);
	return ret;
}

//����ģȡ��
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

	//��ת
	for(i=1;i<=n;i++)
	{
		if(n % i > 0)
			continue;
		ret += ApBmC(c, i, MODN) * Euler(n / i);
		ret %= MODN;
	}
	//��ת
	if((n & 1) == 0)
	{
		ret += ApBmC(c, (n >> 1), MODN) * (c + 1) * (n >> 1);
		ret %= MODN;
	}else{
		ret += ApBmC(c, (n >> 1) + 1, MODN) * n;
		ret %= MODN;
	}
	//�����ܱ任
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