#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 1005
#define MAXI 0x0fffffffffffffff
typedef long long int LL;

LL cost[MAXN][MAXN], dp[MAXN][MAXN], sum[MAXN], sum2[MAXN];  //到第i号为止，炸后一个，且炸了j个的最小解
int arr[MAXN], que[MAXN], front, back;
int n, m;

bool input()
{
	int i, j;

	scanf("%d%d",&n,&m);
	if(n == 0)
		return false;

	for(i=1;i<=n;i++)
	{
		scanf("%d",&arr[i]);
		sum[i] = sum[i-1] + arr[i];
		sum2[i] = sum2[i-1] + arr[i] * arr[i];
	}

	for(i=1;i<=n;i++)
	{
		for(j=i+1;j<=n;j++)
		{
			cost[i][j] = cost[i][j-1] + (sum[j-1] - sum[i-1]) * arr[j];
			//cost[i][j] = ((sum[j] - sum[i-1]) * (sum[j] - sum[i-1]) - sum2[j] + sum2[i-1]) / 2;
		}
	}

	return true;
}

LL gety(int i, int j)
{
	return 2 * dp[i][j] + sum[i] * sum[i] + sum2[i];
}

inline LL getx(int i)
{
	return 2 * sum[i];
}

void oridp()
{
	int i, j, k;

	for(i=1;i<=n;i++)
		dp[i][0] = cost[1][i];
	for(j=1;j<=m;j++)
	{
		dp[1][j] = 0;
		for(i=2;i<=n;i++)
		{
			dp[i][j] = MAXI;
			for(k=1;k<i;k++)
				dp[i][j] = min(dp[i][j], dp[k][j-1] + cost[k+1][i]);
		}
	}
}

LL dpcalc()
{
	int i, j, x, y;

	for(i=1;i<=n;i++)
		dp[i][0] = cost[1][i];
	for(j=1;j<=m;j++)
	{
		front = back = 0;
		que[0] = 1;
		dp[1][j] = 0;
		for(i=2;i<=n;i++)
		{
			while(front < back && (gety(que[front], j-1) - gety(que[front+1], j-1) >= sum[i] * (getx(que[front]) - getx(que[front+1]))))
				front++;
			dp[i][j] = dp[que[front]][j-1] + cost[que[front]+1][i];
			while(front < back)
			{
				x = que[back-1];
				y = que[back];
				if((gety(x, j-1) - gety(y, j-1)) * (getx(y) - getx(i)) >= (gety(y, j-1) - gety(i, j-1)) * (getx(x) - getx(y)))
					back--;
				else
					break;
			}
			back++;
			que[back] = i;
		}
	}

	return dp[n][m];
}

int main()
{
	while(input())
	{
		//oridp();
		printf("%lld\n",dpcalc());
	}
	return 0;
}
