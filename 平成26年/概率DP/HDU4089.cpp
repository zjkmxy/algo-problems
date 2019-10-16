/*
HDU 4098
���⣺��n�����Ŷӵ����ڹ����ϼ�����Ϸ��Tomato���ڵ�m����
���ڶ����еĵ�һ���ˡ���һ�������
1������ʧ�ܣ����ڶ����еȴ���һ�μ������Ϊp1)
2��ʧȥ���ӣ������У�Ȼ�����ڶ��е���󣨸���Ϊp2��
3������ɹ����뿪���У�����Ϊp3��
4��������̱����������ֹͣ��������˶��޷������ˡ�
�������̱��ʱTomato�ڶ����е�λ��<=k�ĸ���

������
����DP��
��dp[i][j]��ʾi�����Ŷ�,Tomato���ڵ�j��λ�ã��ﵽĿ��״̬�ĸ���(j<=i)
dp[n][m]��������
j==1:    dp[i][1]=p1*dp[i][1]+p2*dp[i][i]+p4;
2<=j<=k: dp[i][j]=p1*dp[i][j]+p2*dp[i][j-1]+p3*dp[i-1][j-1]+p4;
k<j<=i:  dp[i][j]=p1*dp[i][j]+p2*dp[i][j-1]+p3*dp[i-1][j-1];
����
j==1:    dp[i][1]=p*dp[i][i]+p41;
2<=j<=k: dp[i][j]=p*dp[i][j-1]+p31*dp[i-1][j-1]+p41;
k<j<=i:  dp[i][j]=p*dp[i][j-1]+p31*dp[i-1][j-1];

����:
p=p2/(1-p1);
p31=p3/(1-p1)
p41=p4/(1-p1)

�������̿��Եõ�
[i][1] = p^i * [i][1] + (k=1..i-1)p^k * P[i+1-k] + (k=1..i-1) p^(i-k) * p3 * dp[i-1][k] + p4
ʽ��P[nn]=p4(nn<=K)��0(nn>K)
��֮���ɡ�
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
