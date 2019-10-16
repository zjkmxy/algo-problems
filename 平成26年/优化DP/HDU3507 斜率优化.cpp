#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 500005
#define MAXI 0x0fffffffffffffff
typedef long long int LL;
LL arr[MAXN], sum[MAXN], dp[MAXN];
int que[MAXN], front, back;
int n, m;

bool input()
{
	int i;

	if(scanf("%d%d",&n,&m)==EOF)
		return false;
	for(i=1;i<=n;i++)
	{
		scanf("%lld",&arr[i]);
		sum[i] = arr[i] + sum[i-1];
	}
	return true;
}

inline LL gety(int k)
{
	return (dp[k] + sum[k] * sum[k]);
}

inline LL getx(int k)
{
	return 2 * sum[k];
}

LL oridp()
{
	int i, j;

	for(i=1;i<=n;i++)
	{
		dp[i] = MAXI;
		for(j=0;j<i;j++)
		{
			dp[i] = min(dp[i], dp[j] + (sum[i] - sum[j]) * (sum[i] - sum[j]) + m);
		}
	}

	return dp[n];
}

LL dpcalc()
{
	int i, x, y;

	que[0] = 0;
	back = front = 0;

	for(i=1;i<=n;i++)
	{
		while(front < back && (gety(que[front]) - gety(que[front+1])) >= sum[i] * (getx(que[front]) - getx(que[front+1])))
		{
			front++;
		}
		dp[i] = dp[que[front]] + (sum[i] - sum[que[front]]) * (sum[i] - sum[que[front]]) + m;
		while(front < back)
		{
			x = que[back-1];
			y = que[back];
			if((gety(x) - gety(y)) * (getx(y) - getx(i)) >= (gety(y) - gety(i)) * (getx(x) - getx(y)))
				back--;
			else
				break;
		}
		back++;
		que[back] = i;
	}

	return dp[n];
}

int main()
{
	while(input())
	{
		printf("%lld\n",dpcalc());
	}
	return 0;
}
