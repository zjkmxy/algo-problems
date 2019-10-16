/*
题意：在一条不满地雷的路上，你现在的起点在1处。在N个点处布有地雷，1<=N<=10。地雷点的坐标范围：[1,100000000].
每次前进p的概率前进一步，1-p的概率前进1-p步。问顺利通过这条路的概率。就是不要走到有地雷的地方。
 
设dp[i]表示到达i点的概率，则 初始值 dp[1]=1.
很容易想到转移方程： dp[i]=p*dp[i-1]+(1-p)*dp[i-2];
但是由于坐标的范围很大，直接这样求是不行的，而且当中的某些点还存在地雷。
 
N个有地雷的点的坐标为 x[1],x[2],x[3]```````x[N].
我们把道路分成N段：
1~x[1];
x[1]+1~x[2];
x[2]+1~x[3];
`
`
`
x[N-1]+1~x[N].
 
这样每一段只有一个地雷。我们只要求得通过每一段的概率。乘法原理相乘就是答案。
对于每一段，通过该段的概率等于1-踩到该段终点的地雷的概率。
 
就比如第一段 1~x[1].  通过该段其实就相当于是到达x[1]+1点。那么p[x[1]+1]=1-p[x[1]].
但是这个前提是p[1]=1,即起点的概率等于1.对于后面的段我们也是一样的假设，这样就乘起来就是答案了。
 
对于每一段的概率的求法可以通过矩阵乘法快速求出来。
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef double MATRIX[2][2];

void mul(MATRIX a, MATRIX b)
{
	int i, j, k;
	MATRIX x;

	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			x[i][j] = 0;
			for(k=0;k<2;k++)
			{
				x[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			a[i][j] = x[i][j];
		}
	}
}

void powm(MATRIX a, int n, MATRIX x)
{
	MATRIX tmp;

	x[0][0] = x[1][1] = 1.0;
	x[0][1] = x[1][0] = 0.0;
	tmp[0][0] = a[0][0];
	tmp[0][1] = a[0][1];
	tmp[1][0] = a[1][0];
	tmp[1][1] = a[1][1];
	while(n > 0)
	{
		if(n & 1)
			mul(x, tmp);
		mul(tmp, tmp);
		n >>= 1;
	}
}

int comp(const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}

double domain(int n, double p)
{
	MATRIX mp, mr;
	int x[20], i;
	double ret = 1.0;

	x[0] = 0;
	for(i=1;i<=n;i++)
	{
		scanf("%d",x+i);
	}
	qsort(x + 1, n, sizeof(int), comp);

	mp[0][0] = p;
	mp[0][1] = 1;
	mp[1][0] = 1 - p;
	mp[1][1] = 0;
	//(1,0)*mp^n = (mp[0][0], mp[0][1])
	for(i=1;i<=n;i++)
	{
		if(x[i] <= x[i-1])
			continue;
		powm(mp, x[i] - x[i-1] - 1, mr);
		ret *= (1.0 - mr[0][0]);  //1减去踩中地雷的概率
	}

	return ret;
}

int main()
{
	int n;
	double p;

	while(scanf("%ld%lf",&n,&p)!=EOF)
	{
		printf("%.7lf\n",domain(n, p));
	}

	return 0;
}
