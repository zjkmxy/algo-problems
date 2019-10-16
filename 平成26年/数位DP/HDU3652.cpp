/*
统计到N为止有多少个含13且被13整除的数，和网上不同的版本
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

typedef unsigned long long LL;
LL dp[12][10][2][13];  //位数，元素，是否包含，余数

void dp_calc()
{
	int i, j, k, l, pow, mod;

	for(j=0;j<=9;j++)
	{
		dp[0][j][0][j] = 1;
	}

	for(i=1,pow=10;i<12;i++,pow*=10)
	{
		for(j=0;j<=9;j++)
		{
			for(l=0;l<13;l++)
			{
				mod = l - j * pow;
				mod = (mod % 13 + 13) % 13;
				for(k=0;k<=9;k++)
				{
					dp[i][j][0][l] += dp[i-1][k][0][mod];
					dp[i][j][1][l] += dp[i-1][k][1][mod];
				}
				if(j == 1)
				{
					dp[i][j][1][l] += dp[i-1][3][0][mod];
					dp[i][j][0][l] -= dp[i-1][3][0][mod];
				}
			}
		}
	}
}

LL calc(LL ans)
{
	LL ret = ans;
	int dig[30], i = 0, j, k, mod, pow = 1, tmp;
	bool flag = false;

	while(ret > 0)
	{
		dig[i] = ret % 10;
		ret /= 10;
		i++;
		pow *= 10;
	}
	dig[i] = 0;
	mod = 0;
	ret = 0;
	for(j=i-1,pow/=10;j>=0;j--,pow/=10)
	{
		for(k=0;k<dig[j];k++)
		{
			ret += dp[j][k][1][(13-mod)%13];
			if(flag)
				ret += dp[j][k][0][(13-mod)%13];
			if(j < i-1 && k == 3 && dig[j+1] == 1 && !flag)
			{
				ret += dp[j][k][0][(13-mod)%13];
			}
		}
		if(j < i-1 && dig[j] == 3 && dig[j+1] == 1)
		{
			flag = true;
		}

		mod += dig[j] * pow;
		mod %= 13;
	}
	if(flag && (ans % 13 == 0))
		ret++;

	return ret;
}

int main()
{
	int t;
	LL ans;

	dp_calc();
	while(scanf("%I64u",&ans)!=EOF)
	{
		printf("%I64u\n",calc(ans));
	}

	return 0;
}
