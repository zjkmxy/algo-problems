/*
ZOJ 3329
题意：有三个骰子，分别有k1,k2,k3个面。
每次掷骰子，如果三个面分别为a,b,c则分数置0，否则加上三个骰子的分数之和。
当分数大于n时结束。求游戏的期望步数。初始分数为0

设dp[i]表示达到i分时到达目标状态的期望，pk为投掷k分的概率，p0为回到0的概率
则dp[i]=∑(pk*dp[i+k])+dp[0]*p0+1;
都和dp[0]有关系，而且dp[0]就是我们所求，为常数
设dp[i]=A[i]*dp[0]+B[i];
代入上述方程右边得到：
dp[i]=∑(pk*A[i+k]*dp[0]+pk*B[i+k])+dp[0]*p0+1
     =(∑(pk*A[i+k])+p0)dp[0]+∑(pk*B[i+k])+1;
     明显A[i]=(∑(pk*A[i+k])+p0)
     B[i]=∑(pk*B[i+k])+1
     先递推求得A[0]和B[0].
     那么  dp[0]=B[0]/(1-A[0]);
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXN 505
#define MAXD 20
double A[MAXN], B[MAXN], p[MAXD];
int n, k1, k2, k3, a, b, c;

double calc_dp()
{
	int i, j, k;
	
	memset(p, 0, sizeof(p));
	p[0] = 1.00 / k1 / k2 / k3;
	for(i=1;i<=k1;i++)
		for(j=1;j<=k2;j++)
			for(k=1;k<=k3;k++)
				p[i+j+k] += p[0];
	p[a+b+c] -= p[0];
	memset(A, 0, sizeof(A));
	memset(B, 0, sizeof(B));
	for(i=n;i>=0;i--)
	{
		A[i] = p[0];
		B[i] = 1;
		for(j=1;j<=k1+k2+k3;j++)
		{
			A[i] += A[i+j] * p[j];
			B[i] += B[i+j] * p[j];
		}
	}

	return B[0] / (1 - A[0]);
}

int main()
{
	int t;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d%d%d%d%d%d",&n,&k1,&k2,&k3,&a,&b,&c);
		printf("%.15lf\n",calc_dp());
	}
	return 0;
}
