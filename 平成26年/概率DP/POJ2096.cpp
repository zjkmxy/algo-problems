/*
POJ 2096
概率DP
writed by kuangbin

dp求期望
逆着递推求解
题意：（题意看题目确实比较难道，n和s都要找半天才能找到）
   一个软件有s个子系统，会产生n种bug
   某人一天发现一个bug,这个bug属于一个子系统，属于一个分类
   每个bug属于某个子系统的概率是1/s,属于某种分类的概率是1/n
   问发现n种bug,每个子系统都发现bug的天数的期望。

求解：
         dp[i][j]表示已经找到i种bug,j个系统的bug，达到目标状态的天数的期望
         dp[n][s]=0;要求的答案是dp[0][0];
         dp[i][j]可以转化成以下四种状态:
              dp[i][j],发现一个bug属于已经有的i个分类和j个系统。概率为(i/n)*(j/s);
              dp[i][j+1],发现一个bug属于已有的分类，不属于已有的系统.概率为 (i/n)*(1-j/s);
              dp[i+1][j],发现一个bug属于已有的系统，不属于已有的分类,概率为 (1-i/n)*(j/s);
              dp[i+1][j+1],发现一个bug不属于已有的系统，不属于已有的分类,概率为 (1-i/n)*(1-j/s);
        整理便得到转移方程

另注：设A->B概率为p，A->A概率为1-p，则
		EA = EB + 1 / p
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXN 1005
double dp[MAXN][MAXN];

double calc_dp(int n, int s)
{
	int i, j;
	double p;  //非自身转移概率

	for(i=n;i>=0;i--)
	{
		for(j=s;j>=0;j--)
		{
			p = 1.00 - (double)(i * j) / (double)(n * s);
			if(i < n || j < s)
				dp[i][j] = 1.00 / p;
			else
				dp[i][j] = 0;

			//以下为非自身转移条件期望(EB)
			if(j < s)
				dp[i][j] += ((double)(i*(s - j)) / (double)(n*s)) * dp[i][j+1] / p;
			if(i < n)
				dp[i][j] += ((double)((n - i)*j) / (double)(n*s)) * dp[i+1][j] / p;
			if(i < n && j < s)
				dp[i][j] += ((double)((n - i)*(s - j)) / (double)(n*s)) * dp[i+1][j+1] / p;
		}
	}

	return dp[0][0];
}

int main()
{
	int n, s;

	while(scanf("%d%d",&n,&s)!=EOF)
	{
		printf("%.4lf\n",calc_dp(n, s));
	}
	return 0;
}
