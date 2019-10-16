/*
HDU 4098
题意：有n个人排队等着在官网上激活游戏。Tomato排在第m个。
对于队列中的第一个人。有一下情况：
1、激活失败，留在队列中等待下一次激活（概率为p1)
2、失去连接，出队列，然后排在队列的最后（概率为p2）
3、激活成功，离开队列（概率为p3）
4、服务器瘫痪，服务器停止激活，所有人都无法激活了。
求服务器瘫痪时Tomato在队列中的位置<=k的概率

解析：
概率DP；
设dp[i][j]表示i个人排队,Tomato排在第j个位置，达到目标状态的概率(j<=i)
dp[n][m]就是所求
j==1:    dp[i][1]=p1*dp[i][1]+p2*dp[i][i]+p4;
2<=j<=k: dp[i][j]=p1*dp[i][j]+p2*dp[i][j-1]+p3*dp[i-1][j-1]+p4;
k<j<=i:  dp[i][j]=p1*dp[i][j]+p2*dp[i][j-1]+p3*dp[i-1][j-1];
化简：
j==1:    dp[i][1]=p*dp[i][i]+p41;
2<=j<=k: dp[i][j]=p*dp[i][j-1]+p31*dp[i-1][j-1]+p41;
k<j<=i:  dp[i][j]=p*dp[i][j-1]+p31*dp[i-1][j-1];

其中:
p=p2/(1-p1);
p31=p3/(1-p1)
p41=p4/(1-p1)

迭代方程可以得到
[i][1] = p^i * [i][1] + (k=1..i-1)p^k * P[i+1-k] + (k=1..i-1) p^(i-k) * p3 * dp[i-1][k] + p4
式中P[nn]=p4(nn<=K)或0(nn>K)
解之即可。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;

#define MAXN 2005
#define eps 1e-8

int N, M, K;
double p1, p2, p3, p4;
double dp[MAXN][MAXN], pp[MAXN];

bool domain()
{
	double tmp;
	int i, j;

	if(scanf("%d%d%d%lf%lf%lf%lf",&N,&M,&K,&p1,&p2,&p3,&p4)==EOF)
		return false;

	if(p4 < eps)
	{
		printf("0.00000\n");
		return true;
	}
	p2 /= 1.00 - p1;
	p3 /= 1.00 - p1;
	p4 /= 1.00 - p1;

	pp[0] = 1.00;
	for(i=1;i<=N;i++)
	{
		pp[i] = pp[i-1] * p2;
	}

	for(i=1;i<=N;i++)
	{
		tmp = 1.00;
		for(j=max(1, i+1-K);j<i;j++)
		{
			tmp += pp[j];
		}
		tmp *= p4;
		for(j=1;j<i;j++)
			tmp += pp[i-j] * p3 * dp[i-1][j];
		dp[i][1] = tmp / (1.00 - pp[i]);
		for(j=2;j<=i;j++)
		{
			if(j <= K)
			{
				dp[i][j] = p2 * dp[i][j-1] + p3 * dp[i-1][j-1] + p4;
			}else{
				dp[i][j] = p2 * dp[i][j-1] + p3 * dp[i-1][j-1];
			}
		}
	}
	printf("%.5lf\n",dp[N][M]);

	return true;
}

int main()
{
	while(domain());
	return 0;
}
