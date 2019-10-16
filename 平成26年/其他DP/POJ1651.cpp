#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 105
#define MAXI 0x0fffffff

int arr[MAXN], dp[MAXN][MAXN];

void dp_calc(int p, int q)
{
	int i;

	if(dp[p][q] >= 0)
		return ;
	if(q - p <= 1)
	{
		dp[p][q] = 0;
		return ;
	}
	if(q - p == 2)
	{
		dp[p][q] = arr[p] * arr[p+1] * arr[q];
		return ;
	}
	dp[p][q] = MAXI;
	for(i=p+1;i<q;i++)
	{
		dp_calc(p, i);
		dp_calc(i, q);
		dp[p][q] = min(dp[p][q], dp[p][i] + dp[i][q] + arr[p] * arr[i] * arr[q]);
	}
}

int main()
{
	int n, i;

	scanf("%d",&n);
	memset(dp, -1, sizeof(dp));
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	dp_calc(0, n-1);
	printf("%d\n",dp[0][n-1]);
	return 0;
}
