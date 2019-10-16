/*
统计到N为止有多少个49，和网上不同的版本
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

typedef unsigned long long LL;
LL dp[30][10][2];  //到第i位以j开头有无49的个数

void dp_calc()
{
	int i, j, k;

	for(j=0;j<=9;j++)
	{
		dp[0][j][0] = 1;
		dp[0][j][1] = 0;
	}

	for(i=1;i<30;i++)
	{
		for(j=0;j<=9;j++)
		{
			dp[i][j][0] = 0;
			dp[i][j][1] = 0;
			for(k=0;k<=9;k++)
			{
				dp[i][j][0] += dp[i-1][k][0];
				dp[i][j][1] += dp[i-1][k][1];
			}
			if(j == 4)
			{
				dp[i][4][1] += dp[i-1][9][0];
				dp[i][4][0] -= dp[i-1][9][0];
			}
		}
	}

	dp[0][0][0] = 1;
}

LL calc(LL ans)
{
	LL ret = 0;
	int dig[30], i = 0, j, k;
	bool flag = false;

	while(ans > 0)
	{
		dig[i] = ans % 10;
		ans /= 10;
		i++;
	}
	dig[i] = 0;
	for(j=i-1;j>=0;j--)
	{
		for(k=0;k<dig[j];k++)
		{
			ret += dp[j][k][1];
			if(flag)
			{
				ret += dp[j][k][0];
			}
		}
		if(j < i-1 && dig[j] == 9 && dig[j+1] == 4)
		{
			flag = true;
		}
	}
	if(flag)
		ret++;

	return ret;
}

int main()
{
	int t;
	LL ans;

	dp_calc();
	scanf("%d",&t);
	while(t--)
	{
		scanf("%I64u",&ans);
		ans = calc(ans);
		printf("%I64u\n",ans);
	}

	return 0;
}