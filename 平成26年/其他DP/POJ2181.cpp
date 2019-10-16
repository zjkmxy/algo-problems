#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 150005
int dp[MAXN][2], arr[MAXN], n;

//到第i号药剂为止，取用情况为奇数偶数时的最大距离
int dpcalc()
{
	int i;

	dp[0][0] = dp[0][1] = 0;
	for(i=1;i<=n;i++)
	{
		dp[i][0] = max(dp[i-1][0], dp[i-1][1] + arr[i]);
		dp[i][1] = max(dp[i-1][1], dp[i-1][0] - arr[i]);
	}
	return max(dp[n][0], dp[n][1]);
}

int main()
{
	int i;

	scanf("%d",&n);
	for(i=1;i<=n;i++)
		scanf("%d",&arr[i]);
	printf("%d\n",dpcalc());
	return 0;
}
