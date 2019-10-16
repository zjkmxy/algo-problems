/*
POJ 2096
����DP
writed by kuangbin

dp������
���ŵ������
���⣺�����⿴��Ŀȷʵ�Ƚ��ѵ���n��s��Ҫ�Ұ�������ҵ���
   һ�������s����ϵͳ�������n��bug
   ĳ��һ�췢��һ��bug,���bug����һ����ϵͳ������һ������
   ÿ��bug����ĳ����ϵͳ�ĸ�����1/s,����ĳ�ַ���ĸ�����1/n
   �ʷ���n��bug,ÿ����ϵͳ������bug��������������

��⣺
         dp[i][j]��ʾ�Ѿ��ҵ�i��bug,j��ϵͳ��bug���ﵽĿ��״̬������������
         dp[n][s]=0;Ҫ��Ĵ���dp[0][0];
         dp[i][j]����ת������������״̬:
              dp[i][j],����һ��bug�����Ѿ��е�i�������j��ϵͳ������Ϊ(i/n)*(j/s);
              dp[i][j+1],����һ��bug�������еķ��࣬���������е�ϵͳ.����Ϊ (i/n)*(1-j/s);
              dp[i+1][j],����һ��bug�������е�ϵͳ�����������еķ���,����Ϊ (1-i/n)*(j/s);
              dp[i+1][j+1],����һ��bug���������е�ϵͳ�����������еķ���,����Ϊ (1-i/n)*(1-j/s);
        �����õ�ת�Ʒ���

��ע����A->B����Ϊp��A->A����Ϊ1-p����
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
	double p;  //������ת�Ƹ���

	for(i=n;i>=0;i--)
	{
		for(j=s;j>=0;j--)
		{
			p = 1.00 - (double)(i * j) / (double)(n * s);
			if(i < n || j < s)
				dp[i][j] = 1.00 / p;
			else
				dp[i][j] = 0;

			//����Ϊ������ת����������(EB)
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
