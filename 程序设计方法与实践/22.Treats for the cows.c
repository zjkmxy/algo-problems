#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int N;
int arr[2005];
int sum[2005];
int dp[2005][2005];
int ans;

int max(int a, int b)
{
	return a>b ? a : b;
}

int dp_calc(int p, int q)
{
	if(dp[p][q] > 0)
		return dp[p][q];
	if(p == q)
	{
		dp[p][q] = arr[p];
	} else{
		dp[p][q] = max(dp_calc(p + 1, q), dp_calc(p, q - 1)) + sum[q] - sum[p - 1];
	}
	return dp[p][q];
}

int main()
{
	int i;
	scanf("%d", &N);
	for(i = 1; i <= N; i++)
	{
		scanf("%d", &arr[i]);
		sum[i] = sum[i - 1] + arr[i];
	}
	printf("%d\n", dp_calc(1, N));
	return 0;
}