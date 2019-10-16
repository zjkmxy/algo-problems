/*
˫��ŷ���������������
POJ2677

�����̣�

��1�����Ƚ����㰴��x�����С�������У�ʱ�临�Ӷ�ΪO(nlgn)��

��2��Ѱ���ӽṹ�������Pi��Pj��·��Ϊ����Pi��ʼ�����ҵ���һֱ��P1��Ȼ�������һֱ��Pj�������·���ϣ��ᾭ��P1��Pmax(i,j)֮������е���ֻ����һ�Ρ�

�ڶ���d(i,j)Ϊ������һ���������·��������ֻ����i>=j�������

ͬʱ������dist(i,j)Ϊ��Pi��Pj֮���ֱ�߾��롣

��3�����Ž⣺������Ҫ�����d(n,n)��

����������d(i,j)����⣬�����������

A����j < i - 1ʱ��d(i,j) = d(i-1,j) + dist(i - 1,i)��

�ɶ����֪����Pi-1һ����·��Pi-Pj�ϣ�����������j<i-1,���Pi����ߵ����ڵ�һ����Pi-1.��˿��Եó�������ʽ��

B����j = i - 1ʱ����Pi�����ڵ��Ǹ��������P1��Pi-1�ܵ��κ�һ���������Ҫ�ݹ������С���Ǹ�·����

d(i,j) = d(i,i-1) = min{d(k,j) + dist(i,k)},����1 <= k <= j��

C����j=iʱ��·������������������������P1-Pi��P2-Pi...Pi-1-Pi��

����У�

d(i,i) = min{d(i,1)+dist(1,i),...,d(i,i-1),dist(i-1,i)}.��
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

		// j = i ��i = n - 1��û��Ҫ�㡣i=n-1�ĳ��ϣ�������ע�͵��Ĵ���
		// Ϊ��ά���㷨����������⣬���ﱣ����β���Ҫ�Ĵ���
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
	//D[n-1][n-1] = D[n-1][n-2] + dist(n-1, n-2);һ���������������������߶�����n-2��

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

