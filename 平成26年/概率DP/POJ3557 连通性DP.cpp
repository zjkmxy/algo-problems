#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
using namespace std;

typedef long long LL;
#define MAXN 21
int comb[MAXN][MAXN];  //a个中取b个
LL stir[MAXN];        //阶乘
int N;
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
	scanf("%d%lf",&N,&P);
	dp_calc();
	printf("%.3lf\n",prob[N][1]);
	return 0;
}

/*
另外的解法是：

         * dp[i]为在当前情况下，构成i个点的连通点集的概率
         *
         * 若要计算dp[i]，则要从i-1的点中，划分出一个j-1的子集，所以共有C(i-1,j-1)的子集
         * 然后再将这个j-1子集，加上新加入的点，若使这j个点构成连通点集，概率为dp[j]
         * 若使这i个点不连通，则在上面所说的两个字集之间没有连通的边，一共有j*(i-j)种可能的边
         * 所以概率为(1-p)^(j*(i-j))
         *
         * 将j=(1,i-1)的所有情况加到一起，得到的就是有i个点的情况下，不能构成连通图的概率
         * 所以dp[i]=1-sigma(C[i-1][j-1]*dp[j]*pow(1-p,double(j*(i-j)))
         *

*/