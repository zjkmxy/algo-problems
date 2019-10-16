#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
using namespace std;

#define MAXN 5005
#define MAXI 0x3fffffff
#define BOOL long
#define TRUE  1
#define FALSE 0
#define ERROR -1

int dp[MAXN][MAXN];  //Çø¼äDP
BOOL pan[MAXN][MAXN];
char str[MAXN];

void calc_pan(int i, int j)
{
	if(pan[i][j] != ERROR)
		return ;
	if(i >= j)
	{
		pan[i][j] = TRUE;
	}else{
		calc_pan(i+1, j);
		calc_pan(i, j-1);
		calc_pan(i+1, j-1);
		pan[i][j] = pan[i+1][j-1] && (str[i] == str[j]);
	}
}

void calc_dp(int p, int q)
{
	if(dp[p][q] >= 0)
		return ;
	if(p == q)
	{
		dp[p][q] = 1;
		return ;
	}
	if(p > q)
	{
		dp[p][q] = 0;
		return ;
	}
	calc_dp(p+1, q);
	calc_dp(p, q-1);
	calc_dp(p+1, q-1);
	dp[p][q] = dp[p+1][q] + dp[p][q-1] - dp[p+1][q-1] + pan[p][q];
}

int main()
{
	int i, q, a, b, len;

	scanf("%s",str);
	len = strlen(str);
	memset(pan, -1, sizeof(pan));
	memset(dp, -1, sizeof(dp));
	calc_pan(0, len-1);
	calc_dp(0, len-1);
	scanf("%d",&q);
	for(i=0;i<q;i++)
	{
		scanf("%d%d",&a,&b);
		a--;
		b--;
		printf("%d\n",dp[a][b]);
	}
	return 0;
}
