#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 1200
#define MAXK 15
#define MAXI 0x0fffffff
long dp[MAXN][MAXK];

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

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

void dp_calc()
{
	int i, j, k;

	dp[0][0] = 1;
	for(j=0;j<PRIMENUM;j++)
	{
		for(i=MAXN-primes[j];i>=0;i--)  //防止一个素数被不同数多次使用，故倒着计算
		{
			for(k=MAXK-1;k>0;k--)       //防止一个数字多次加同一个素数，故倒着计算
			{
				dp[i+primes[j]][k] += dp[i][k-1];
			}
		}
	}
}

int main()
{
	int n, k;

	init_prime();
	dp_calc();
	while(true)
	{
		scanf("%d%d",&n,&k);
		if(n==0 && k==0)
			break;
		printf("%d\n",dp[n][k]);
	}
	return 0;
}
