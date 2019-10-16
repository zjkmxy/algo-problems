/*
HDU1850 Being a Good Boy in Spring Festival

Nim博弈：
Nim和（异或）为0的点全是P点，问有几种方案的话，只要能把当前Nim和消掉就好。
所以就是sum^arr[i]<=arr[i]的数目。

SG值就是sum，不解释。
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int arr[105];
int main()
{
	int n, i, sum, ret;
	while(scanf("%d",&n)!=EOF)
	{
		if(n==0)
			return 0;
		sum = 0;
		for(i=0;i<n;i++)
		{
			scanf("%d",&arr[i]);
			sum ^= arr[i];
		}
		ret = 0;
		if(sum == 0)
		{
			printf("0\n");
			continue;
		}
		for(i=0;i<n;i++)
		{
			if(arr[i] >= (sum ^ arr[i]))
				ret++;
		}
		printf("%d\n",ret);
	}
	return 0;
}
