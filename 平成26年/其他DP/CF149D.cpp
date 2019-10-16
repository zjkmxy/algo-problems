#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
using namespace std;

#define MAXN 1005
#define MODN 1000000007

char str[MAXN];
int len, pair[MAXN];
long long dp[MAXN][MAXN][3][3];

int ps[MAXN];
void init_pair()
{
	int i, cnt = 0;

	for(i=0;i<len;i++)
	{
		if(str[i] == '(')
		{
			ps[cnt] = i;
			cnt++;
		}else{
			cnt--;
			pair[i] = ps[cnt];
			pair[ps[cnt]] = i;
		}
	}
}

long long dp_calc(int p, int q, int a, int b)
{
	int i, j;

	if(dp[p][q][a][b] >= 0)
		return dp[p][q][a][b];

	dp[p][q][a][b] = 0;
	if(p >= q)
	{
		return dp[p][q][a][b];
	}
	if(q == p + 1)
	{
		if((a > 0 && b == 0) || (a == 0 && b > 0))
		{
			dp[p][q][a][b] = 1;
		}
		return dp[p][q][a][b];
	}
	if(pair[q] == p)
	{
		if((a > 0 && b > 0) || (a == 0 && b == 0))
		{
			return dp[p][q][a][b];
		}
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(i == a && i > 0)
					continue;
				if(j == b && j > 0)
					continue;
				dp[p][q][a][b] += dp_calc(p+1, q-1, i, j);
				dp[p][q][a][b] %= MODN;
			}
		}
		return dp[p][q][a][b];
	}else{
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(i == j && i > 0)
					continue;
				dp[p][q][a][b] += dp_calc(p, pair[p], a, i) * dp_calc(pair[p]+1, q, j, b) % MODN;
				dp[p][q][a][b] %= MODN;
			}
		}
		return dp[p][q][a][b];
	}
}

int main()
{
	int i, j;
	long long ans = 0;

	scanf("%s",str);
	len = strlen(str);
	init_pair();
	memset(dp, -1, sizeof(dp));
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			ans += dp_calc(0, len-1, i, j);
			ans %= MODN;
		}
	}
	printf("%lld\n",ans);
	return 0;
}
