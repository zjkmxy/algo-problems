/*
问有多少种N的排列，满足元素之间特定顺位增减关系。
令dp[i][j]表示以j为结尾的1..i的排列有多少种
计算dp[i+1][j]时，把前面所有大于等于j的全部+1，于是就不重复了
维护前缀和即可。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
#include<cctype>
using namespace std;

#define MAXN 1005
int dp[MAXN][MAXN];
int sum[MAXN][MAXN];
char str[MAXN];
int N;

bool domain()
{
	int i, j;
	if(gets(str)==NULL)
		return false;

	dp[1][1] = 1;
	sum[1][1] = 1;
	N = strlen(str) + 1;
	for(i=2;i<=N;i++)
	{
		for(j=1;j<=i;j++)
		{
			if(str[i-2] == 'I')
				dp[i][j] = sum[i-1][j-1];
			else if(str[i-2] == 'D')
				dp[i][j] = (sum[i-1][i-1] - sum[i-1][j-1] + 1000000007) % 1000000007;
			else
				dp[i][j] = sum[i-1][i-1];
			sum[i][j] = (sum[i][j-1] + dp[i][j]) % 1000000007;
		}
	}
	printf("%d\n",sum[N][N]);

	return true;
}

int main()
{
	while(domain());
	return 0;
}
