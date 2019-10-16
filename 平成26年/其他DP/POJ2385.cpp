#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 1005
#define MAXM 35

int dp[MAXN][MAXM][2];  //在i时间至多走j次且最终位于第k树下的最好数目
bool fst[MAXN];
int n, m;

int dpcalc()
{
	int i, j;

	for(i=1;i<=n;i++)
	{
		dp[i][0][0] = dp[i-1][0][0] + (fst[i] ? 1 : 0);
		dp[i][0][1] = 0;
	}
	if(m == 0)
		return dp[n][0][0];

	for(j=1;j<=m;j++)
	{
		for(i=1;i<=n;i++)
		{
			dp[i][j][0] = max(dp[i-1][j][0] + (fst[i] ? 1 : 0), dp[i-1][j-1][1] + (fst[i] ? 1 : 0));
			dp[i][j][1] = max(dp[i-1][j][1] + (fst[i] ? 0 : 1), dp[i-1][j-1][0] + (fst[i] ? 0 : 1));
		}
	}
	return max(dp[n][m][0], dp[n][m][1]);
}

int main()
{
	int i, a;

	scanf("%d%d",&n,&m);
	for(i=1;i<=n;i++)
	{
		scanf("%d",&a);
		if(a == 1)
			fst[i] = true;
		else
			fst[i] = false;
	}
	printf("%d\n",dpcalc());
	return 0;
}
