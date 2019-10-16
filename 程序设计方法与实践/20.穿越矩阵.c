#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int map[105][12];
int dp[105][12];
int pre[105][12];
int N, M;
int ans, trace[105];

int get_last(int p)
{
	int i;
	for(i=N-1;i>=0;i--)
	{
		p = pre[i][p];
	}
	return p;
}

void DP_Calc()
{
	int i, j, t, p;

	for(j=0;j<M;j++)
	{
		dp[N-1][j] = map[N-1][j];
	}
	for(i=N-2;i>=0;i--)
	{
		for(j=0;j<M;j++)
		{
			dp[i][j] = 0x3fffffff;
			for(t=3,p=j-1;t--;p++)
			{
				p = (p + M) % M;
				if(dp[i+1][p] + map[i][j] < dp[i][j] || (dp[i+1][p] + map[i][j] == dp[i][j] && p < pre[i][j]))
				{
					dp[i][j] = dp[i+1][p] + map[i][j];
					pre[i][j] = p;
				}
			}
		}
	}
	ans = 0x3fffffff;
	for(j=0;j<M;j++)
	{
		if(dp[0][j] < ans)
		{
			ans = dp[0][j];
			p = j;
		}
	}
	for(i=0;i<N;i++)
	{
		trace[i] = p;
		p = pre[i][p];
	}
}

int input()
{
	int i, j;
	if(scanf("%d%d",&M,&N)==EOF)
		return 0;
	for(j=0;j<M;j++)
	{
		for(i=0;i<N;i++)
		{
			scanf("%d",&map[i][j]);
		}
	}

	return 1;
}

int main()
{
	int i, j;
	while(input())
	{
		DP_Calc();
		for(i=0;i<N;i++)
			printf(i?" %d":"%d",trace[i]+1);
		printf("\n%d\n",ans);
	}
	return 0;
}