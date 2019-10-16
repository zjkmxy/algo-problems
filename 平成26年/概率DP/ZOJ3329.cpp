/*
ZOJ 3329
���⣺���������ӣ��ֱ���k1,k2,k3���档
ÿ�������ӣ����������ֱ�Ϊa,b,c�������0����������������ӵķ���֮�͡�
����������nʱ����������Ϸ��������������ʼ����Ϊ0

��dp[i]��ʾ�ﵽi��ʱ����Ŀ��״̬��������pkΪͶ��k�ֵĸ��ʣ�p0Ϊ�ص�0�ĸ���
��dp[i]=��(pk*dp[i+k])+dp[0]*p0+1;
����dp[0]�й�ϵ������dp[0]������������Ϊ����
��dp[i]=A[i]*dp[0]+B[i];
�������������ұߵõ���
dp[i]=��(pk*A[i+k]*dp[0]+pk*B[i+k])+dp[0]*p0+1
     =(��(pk*A[i+k])+p0)dp[0]+��(pk*B[i+k])+1;
     ����A[i]=(��(pk*A[i+k])+p0)
     B[i]=��(pk*B[i+k])+1
     �ȵ������A[0]��B[0].
     ��ô  dp[0]=B[0]/(1-A[0]);
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
