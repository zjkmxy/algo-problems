/*
POJ1286 Necklace of Beads
��HDU3923���򻯰�
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;

typedef signed long long int SINT;

#define PRIMENUM 26
SINT primes[PRIMENUM] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};

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
SINT ApB(SINT A, SINT B)
{
	SINT ret = 1;

	while(B >= 1)
	{
		if((B & 1) == 1)
		{
			ret = ret * A;
		}

		B >>= 1; 
		A = A * A;
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
		ret += ApB(c, i) * Euler(n / i);
	}
	//��ת
	if((n & 1) == 0)
	{
		ret += ApB(c, (n >> 1)) * (c + 1) * (n >> 1);
	}else{
		ret += ApB(c, (n >> 1) + 1) * n;
	}
	//�����ܱ任
	ret /= (n << 1);

	return ret;
}

int main()
{
	int t, a, b, i;

	while(true)
	{
		scanf("%d",&a);
		if(a == 0)
		{
			printf("0\n");
			continue;
		}
		if(a == -1)
			break;
		printf("%lld\n",calc(3, a));
	}
	return 0;
}