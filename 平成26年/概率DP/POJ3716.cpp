#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
using namespace std;

const double PREPQ[5] = {0.0625, 0.25, 0.375, 0.25, 0.625};	//每一种PQ分布的先验概率
const double PRENR[7] = {0.015625, 0.09375, 0.234375, 0.3125, 0.234375, 0.09375, 0.015625};	//每一种骰子红色数的先验概率
double COND[7][7][7][7][5];	//条件概率

double aftnr[7][7][7][7];	//后验概率
double aftpq[5]	;			//后验概率
int P, Q;

//计算条件概率
//期望值为a+b+c+d
void calc_pq(double a, double b, double c, double d, double pq[5])
{
	a /= 6.0;
	b /= 6.0;
	c /= 6.0;
	d /= 6.0;
	pq[4] = a * b * c * d;
	pq[3] = (1.0-a) * b * c * d;
	pq[3] += (1.0-b) * a * c * d;
	pq[3] += (1.0-c) * b * a * d;
	pq[3] += (1.0-d) * b * c * a;
	pq[2] = (1.0-a) * (1.0-b) * c * d;
	pq[2] += (1.0-a) * (1.0-c) * b * d;
	pq[2] += (1.0-a) * (1.0-d) * b * c;
	pq[2] += (1.0-b) * (1.0-c) * a * d;
	pq[2] += (1.0-b) * (1.0-d) * a * c;
	pq[2] += (1.0-c) * (1.0-d) * a * b;
	pq[1] = (1.0-a) * (1.0-b) * (1.0-c) * d;
	pq[1] += (1.0-a) * (1.0-b) * (1.0-d) * c;
	pq[1] += (1.0-a) * (1.0-c) * (1.0-d) * b;
	pq[1] += (1.0-b) * (1.0-c) * (1.0-d) * a;
	pq[0] = (1.0-a) * (1.0-b) * (1.0-c) * (1.0-d);
}

//初始化条件概率
void init_cond()
{
	int i, j, k, l;

	for(i=0;i<=6;i++)
	{
		for(j=0;j<=6;j++)
		{
			for(k=0;k<=6;k++)
			{
				for(l=0;l<=6;l++)
				{
					calc_pq(i, j, k, l, COND[i][j][k][l]);
				}
			}
		}
	}
}

//计算后验概率
double calc_aft()
{
	int i, j, k, l, m;
	double ret;

	ret = 0.0;
	for(i=0;i<=6;i++)
	{
		for(j=0;j<=6;j++)
		{
			for(k=0;k<=6;k++)
			{
				for(l=0;l<=6;l++)
				{
					aftnr[i][j][k][l] = COND[i][j][k][l][P] * COND[i][j][k][l][Q] * PRENR[i] * PRENR[j] * PRENR[k] * PRENR[l] / PREPQ[P] / PREPQ[Q];
					ret += aftnr[i][j][k][l];
				}
			}
		}
	}

	//调平全概率单位
	for(i=0;i<=6;i++)
	{
		for(j=0;j<=6;j++)
		{
			for(k=0;k<=6;k++)
			{
				for(l=0;l<=6;l++)
				{
					aftnr[i][j][k][l] /= ret;
				}
			}
		}
	}

	for(m=0;m<=5;m++)
		aftpq[m] = 0.0;
	ret = 0.0;
	for(i=0;i<=6;i++)
	{
		for(j=0;j<=6;j++)
		{
			for(k=0;k<=6;k++)
			{
				for(l=0;l<=6;l++)
				{
					//for(m=0;m<=5;m++)
					//{
						//这样计算的话精度误差较大
					//	aftpq[m] += aftnr[i][j][k][l] * COND[i][j][k][l][m];
					//}
					ret += aftnr[i][j][k][l] * (i + j + k + l) / 6.0;
				}
			}
		}
	}
	//ret = 0.0;
	//for(m=0;m<=4;m++)
	//	ret += m * aftpq[m];
	return ret;
}

int main()
{
	int t;

	init_cond();
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d",&P,&Q);
		printf("%.3lf\n",calc_aft());
	}
	return 0;
}
