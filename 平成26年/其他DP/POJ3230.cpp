#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 105
#define MAXI 0x0fffffff

int n, m;
int dp[MAXN][MAXN];  //第n天在第m个城市以后能拿多少钱
int fee[MAXN][MAXN], profit[MAXN][MAXN];

int calc()
{
	int i, j, k;

	for(i=0;i<=n;i++)
	{
		dp[i][m] = profit[m][i];
	}
	for(j=m-1;j>=0;j--)
	{
		for(i=1;i<=n;i++)
		{
			dp[i][j] = -MAXI;
			for(k=1;k<=n;k++)
			{
				dp[i][j] = max(dp[i][j], dp[k][j+1] - fee[i][k] + profit[j][i]);
			}
		}
	}
	return dp[1][0];
}

bool input()
{
	int i, j;

	scanf("%d%d",&n,&m);
	if(n == 0)
		return false;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%d",&fee[i][j]);
		}
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%d",&profit[i][j]);
		}
	}
	return true;
}

int main()
{
	while(input())
	{
		printf("%d\n",calc());
	}
	return 0;
}
