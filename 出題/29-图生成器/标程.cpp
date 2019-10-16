#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
using namespace std;

typedef long long LL;
#define MAXN 21
int comb[MAXN][MAXN];  //a个中取b个
LL stir[MAXN];        //阶乘
int N, K;
double P;
double prob[MAXN][MAXN];  //a个节点的图有b个连通块的概率

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
			prob[i][j] = tmp / ((1<<j) - 2);  //关于分母为什么是这个值，我也不知道，但是可以肯定的是，这样能够得到正确的结果。
			prob[i][1] -= prob[i][j];
		}
	}
}

int main()
{
	init_comb();
	while(scanf("%d%lf%d",&N,&P,&K)!=EOF)
	{
		dp_calc();
		printf("%.3lf\n",prob[N][K]);
	}
	return 0;
}