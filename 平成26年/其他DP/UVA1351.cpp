#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
using namespace std;

#define MAXN 205
#define MAXI 0x3fffffff
int dp[MAXN][MAXN];  //Çø¼äDP
char str[MAXN];

bool brkstr(int l, int r, int k)
{
	int i, j;

	for(i=0;i<k;i++)
	{
		for(j=1;l+j*k+i<=r;j++)
		{
			if(str[l+i] != str[l+j*k+i])
				return false;
		}
	}
	return true;
}

int digcnt(int k)
{
	int i;

	for(i=0;k>0;i++)
	{
		k /= 10;
	}
	return i;
}

int dpcalc(int l, int r)
{
	int len = r - l + 1, i;

	if(dp[l][r] >= 0)
		return dp[l][r];
	if(len <= 0)
	{
		dp[l][r] = 0;
		return 0;
	}else if(len == 1)
	{
		dp[l][r] = 1;
		return 1;
	}

	dp[l][r] = MAXI;
	for(i=l;i<r;i++)
	{
		dp[l][r] = min(dp[l][r], dpcalc(l, i) + dpcalc(i+1, r));
	}

	for(i=1;i<=len/2;i++)
	{
		if(len % i != 0)
			continue;
		if(brkstr(l, r, i))
		{
			dp[l][r] = min(dp[l][r], digcnt(len / i) + dpcalc(l, l+i-1) + 2);
		}
	}

	return dp[l][r];
}

int main()
{
	int t, len;

	scanf("%d",&t);
	while(t--)
	{
		scanf("%s", str);
		len = strlen(str);
		memset(dp, -1, sizeof(dp));
		printf("%d\n",dpcalc(0, len-1));
	}
	return 0;
}
