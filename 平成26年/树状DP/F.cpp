/*
双调欧几里得旅行商问题
POJ2677

求解过程：

（1）首先将各点按照x坐标从小到大排列，时间复杂度为O(nlgn)。

（2）寻找子结构：定义从Pi到Pj的路径为：从Pi开始，从右到左一直到P1，然后从左到右一直到Pj。在这个路径上，会经过P1到Pmax(i,j)之间的所有点且只经过一次。

在定义d(i,j)为满足这一条件的最短路径。我们只考虑i>=j的情况。

同时，定义dist(i,j)为点Pi到Pj之间的直线距离。

（3）最优解：我们需要求的是d(n,n)。

关于子问题d(i,j)的求解，分三种情况：

A、当j < i - 1时，d(i,j) = d(i-1,j) + dist(i - 1,i)。

由定义可知，点Pi-1一定在路径Pi-Pj上，而且又由于j<i-1,因此Pi的左边的相邻点一定是Pi-1.因此可以得出上述等式。

B、当j = i - 1时，与Pi左相邻的那个点可能是P1到Pi-1总的任何一个。因此需要递归求出最小的那个路径：

d(i,j) = d(i,i-1) = min{d(k,j) + dist(i,k)},其中1 <= k <= j。

C、当j=i时，路径上最后相连的两个点可能是P1-Pi、P2-Pi...Pi-1-Pi。

因此有：

d(i,i) = min{d(i,1)+dist(1,i),...,d(i,i-1),dist(i-1,i)}.。
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct POINT{
	int x,y;
};

int n;
double D[300][300];
POINT pts[300];

double dist(int i, int j)
{
	double dx,dy;
	dx = pts[i].x-pts[j].x;
	dy = pts[i].y-pts[j].y;
	return sqrt(dx*dx+dy*dy);
}

int comp(const void *A, const void *B)
{
	POINT *a, *b;

	a = (POINT*)A;
	b = (POINT*)B;
	return (a->x - b->x);
}

double calc()
{
	int i,j;
	double cur;

	D[0][0] = 0;
	D[1][0] = dist(1,0);
	D[1][1] = 2 * dist(1, 0);
	for(i=2;i<n;i++)
	{
		// j < i - 1
		for(j=0;j<i-1;j++)
		{
			D[i][j] = D[i-1][j] + dist(i-1, i);
		}

		// j = i - 1
		D[i][i-1] = D[i-1][0] + dist(i, 0);
		for(j=1;j<i-1;j++)
		{
			cur = D[i-1][j] + dist(i, j);
			if(cur < D[i][i-1])
			{
				D[i][i-1] = cur;
			}
		}

		// j = i 除i = n - 1外没必要算。i=n-1的场合，见下面注释掉的代码
		// 为了维护算法完整易于理解，这里保留这段不必要的代码
		D[i][i] = D[i][0] + dist(i, 0);
		for(j=1;j<=i-1;j++)
		{
			cur = D[i][j] + dist(i, j);
			if(cur < D[i][i])
			{
				D[i][i] = cur;
			}
		}
	}
	//D[n-1][n-1] = D[n-1][n-2] + dist(n-1, n-2);一定会是这样，不可能两边都不和n-2连

	return D[n-1][n-1];
}

int main()
{
	int i;

	while(scanf("%d",&n)!=EOF)
	{
		for(i=0;i<n;i++)
		{
			scanf("%d %d",&pts[i].x,&pts[i].y);
		}
		qsort(pts, n, sizeof(POINT), comp);
		printf("%.2lf\n",calc());
	}
	return 0;
}

