#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
using namespace std;

typedef long long LL;
#define MAXN 21
int comb[MAXN][MAXN];  //a����ȡb��
LL stir[MAXN];        //�׳�
int N;
double P;
double prob[MAXN][MAXN];  //a���ڵ��ͼ��b����ͨ��ĸ���

void init_comb()
{
	int i, j;

	for(i=0;i<MAXN;i++)
	{
		for(j=0;j<MAXN;j++)
		{
			if(j > i)
			{
				comb[i][j] = 0;
			}else if(j==i || j==0)
			{
				comb[i][j] = 1;
			}else{
				comb[i][j] = comb[i-1][j] + comb[i-1][j-1];
			}
		}
	}
	stir[0] = 1;
	for(i=1;i<MAXN;i++)
		stir[i] = i * stir[i-1];
}

void dp_calc()
{
	int i, j, k, l;
	double tmp;

	prob[1][1] = 1.00;
	for(i=2;i<=N;i++)
	{
		prob[i][1] = 1.00;
		for(j=2;j<=i;j++)
		{
			tmp = 0.0;
			for(k=1;k<i;k++)
			{
				for(l=1;l<=k;l++)
				{
					tmp += prob[k][l] * prob[i-k][j-l] * comb[i][k] * pow(1.0-P, k*(i-k));
				}
			}
			prob[i][j] = tmp / ((1<<j) - 2);  //���ڷ�ĸΪʲô�����ֵ����Ҳ��֪�������ǿ��Կ϶����ǣ������ܹ��õ���ȷ�Ľ����
			prob[i][1] -= prob[i][j];
		}
	}
}

int main()
{
	init_comb();
	scanf("%d%lf",&N,&P);
	dp_calc();
	printf("%.3lf\n",prob[N][1]);
	return 0;
}

/*
����Ľⷨ�ǣ�

         * dp[i]Ϊ�ڵ�ǰ����£�����i�������ͨ�㼯�ĸ���
         *
         * ��Ҫ����dp[i]����Ҫ��i-1�ĵ��У����ֳ�һ��j-1���Ӽ������Թ���C(i-1,j-1)���Ӽ�
         * Ȼ���ٽ����j-1�Ӽ��������¼���ĵ㣬��ʹ��j���㹹����ͨ�㼯������Ϊdp[j]
         * ��ʹ��i���㲻��ͨ������������˵�������ּ�֮��û����ͨ�ıߣ�һ����j*(i-j)�ֿ��ܵı�
         * ���Ը���Ϊ(1-p)^(j*(i-j))
         *
         * ��j=(1,i-1)����������ӵ�һ�𣬵õ��ľ�����i���������£����ܹ�����ͨͼ�ĸ���
         * ����dp[i]=1-sigma(C[i-1][j-1]*dp[j]*pow(1-p,double(j*(i-j)))
         *

*/