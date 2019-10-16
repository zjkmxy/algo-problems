#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef long long int LLI;
LLI dp1[9][16], dp2[9][16];
LLI calc(int n, int k)
{
	int i, j;
	LLI cnt;
	if(k >= n * 2)
		return 0;
	memset(dp1, 0, sizeof(dp1));
	memset(dp2, 0, sizeof(dp2));
	//奇数十字格
	dp1[0][0] = 1;
	if(n & 1)
		cnt = n / 2 + 1;
	else
		cnt = n / 2;
	for(i=1;i<=cnt;i++)
	{
		for(j=0;j<=k && j<(i<<1);j++)
		{
			dp1[i][j] = dp1[i-1][j];
			if(j >= 1)
				dp1[i][j] += dp1[i-1][j-1] * (i * 2 - j) * (i * 2 > n ? 1 : 2);
			if(j >= 2 && i * 2 <= n)
				dp1[i][j] += dp1[i-1][j-2] * (i * 2 - j + 1) * (i * 2 - j);
		}
	}
	//偶数十字格
	dp2[0][0] = 1;
	cnt = n / 2;
	for(i=1;i<=cnt;i++)
	{
		for(j=0;j<=k && j<=(i<<1);j++)
		{
			dp2[i][j] = dp2[i-1][j];
			if(j >= 1)
				dp2[i][j] += dp2[i-1][j-1] * (i * 2 - j + 1) * (i * 2 >= n ? 1 : 2);
			if(j >= 2 && i * 2 < n)
				dp2[i][j] += dp2[i-1][j-2] * (i * 2 - j + 2) * (i * 2 - j + 1);
		}
	}
	cnt = 0;
	for(i=0;i<=k;i++)
	{
		if(n & 1)
			cnt += dp1[(n>>1)+1][i] * dp2[(n>>1)][k-i];
		else
			cnt += dp1[(n>>1)][i] * dp2[(n>>1)][k-i];
	}
	return cnt;
}

int main()
{
	int n, k;
	while(scanf("%d%d",&n,&k)!=EOF)
	{
		if(n == 0 && k == 0)
			break;
		printf("%lld\n",calc(n, k));
	}
	return 0;
}