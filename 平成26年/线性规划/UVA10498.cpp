//线性规划单纯形，不用考虑无解情况
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<cmath>
using namespace std;

#define MAXN 50
#define MAXB 50
#define MAXI 0x7fffffff
typedef double MATRIX[MAXB+1][MAXN+1];
typedef double VERTN[MAXN+1];
typedef double VERTB[MAXB+1];
typedef double VALUE;
typedef int LABEL[MAXN+MAXB+1];

MATRIX mA;
VERTB mb;
VERTN mc;
VALUE mv;
VERTN mx;
LABEL mlab;
int n, m;

//旋转，将基本变量l转为非基本变量e，通过改变标号节省空间
void pivot(MATRIX A, VERTB b, VERTN c, VALUE &v, int nn, int nb, LABEL lab, int l, int e, int base)
{
	int i, j;

	//对换标号
	swap(lab[nn+l], lab[e]);

	//变更l的方程为e的方程
	b[l] /= A[l][e];
	for(j=base;j<=nn;j++)
	{
		if(j != e)
		{
			A[l][j] /= A[l][e];
		}
	}
	A[l][e] = 1.00 / A[l][e];

	//变更其他方程
	for(i=1;i<=nb;i++)
	{
		if(i==l)
			continue;

		b[i] -= A[i][e] * b[l];
		for(j=base;j<=nn;j++)
		{
			if(j != e)
			{
				A[i][j] -= A[i][e] * A[l][j];
			}
		}
		A[i][e] = -A[i][e] * A[l][e];
	}

	//变更目标函数
	v += c[e] * b[l];
	for(j=base;j<=nn;j++)
	{
		if(j != e)
		{
			c[j] -= c[e] * A[l][j];
		}
	}
	c[e] = -c[e] * A[l][e];
}

//单纯形计算，假定已经init过了
//返回最大化的v以及相应的x，或者false表示无界
bool simplex(MATRIX A, VERTB b, VERTN c, LABEL lab, int nn, int nb, VERTN x, VALUE &v)
{
	int i, minj, minl;
	double minv;
	bool found;

	while(true)
	{
		minj = MAXI;
		for(i=1;i<=nn;i++)
		{
			if((c[i]>0)&&(lab[i]<minj))
			{
				minj = i;
			}
		}
		if(minj == MAXI)  //已经得到解
			break;

		found = false;
		for(i=1;i<=nb;i++)
		{
			if(A[i][minj]<=0)
				continue;
			if(!found)
			{
				minv = b[i] / A[i][minj];
				minl = i;
				found = true;
				continue;
			}
			if(b[i] / A[i][minj] < minv)
			{
				minv = b[i] / A[i][minj];
				minl = i;
				continue;
			}
			if((b[i] / A[i][minj] == minv)&&(lab[i+nn] < lab[minl+nn]))
			{
				minv = b[i] / A[i][minj];
				minl = i;
				continue;
			}
		}
		if(!found)
			return false;  //无界

		pivot(A, b, c, v, nn, nb, lab, minl, minj, 1);
	}

	//求出x
	for(i=1;i<=nn;i++)
	{
		if(lab[i]<=nn)
		{
			x[lab[i]] = 0;  //始终是非基本变量
		}
	}
	for(i=nn+1;i<=nn+nb;i++)
	{
		if(lab[i]<=nn)
		{
			x[lab[i]] = b[i-nn];  //旋转成基本变量
		}
	}
	return true;
}

//通过辅助线性规划寻求初始解，如果无解返回FALSE
bool init_simplex(MATRIX A, VERTB b, LABEL lab, int nn, int nb, VERTN oric, VALUE &oriv)
{
	VALUE v = 0;
	VERTN c;
	int i, j, k = -1, s;
	int minj, minl;
	double minv;
	bool found;

	for(i=1;i<=nb;i++)
	{
		if((k<0)||(b[i]<b[k]))
			k = i;
	}
	if(b[k] >= 0)
		return true;

	//构造辅助规划
	for(i=1;i<=nb;i++)
	{
		A[i][0] = -1;
	}
	for(i=1;i<=nn;i++)
	{
		c[i] = 0;
	}
	c[0] = -1;
	pivot(A, b, c, v, nn, nb, lab, k, 0, 0);

	//寻求新的最大解
	while(true)
	{
		minj = MAXI;
		for(i=0;i<=nn;i++)
		{
			if((c[i]>0)&&(lab[i]<minj))
			{
				minj = i;
			}
		}
		if(minj == MAXI)  //已经得到解
			break;

		found = false;
		for(i=1;i<=nb;i++)
		{
			if(A[i][minj]<=0)
				continue;
			if(!found)
			{
				minv = b[i] / A[i][minj];
				minl = i;
				found = true;
				continue;
			}
			if(b[i] / A[i][minj] < minv)
			{
				minv = b[i] / A[i][minj];
				minl = i;
				continue;
			}
			if((b[i] / A[i][minj] == minv)&&(lab[i+nn] < lab[minl+nn]))
			{
				minv = b[i] / A[i][minj];
				minl = i;
				continue;
			}
		}
		if(!found)
			return false;  //无界

		pivot(A, b, c, v, nn, nb, lab, minl, minj, 0);
	}

	//检查x0值
	for(i=0;i<=nn+nb;i++)
	{
		if(lab[i]==0)
			break;
	}
	//if((i > nn)&&(b[i-nn] != 0))  精度问题会WA
	if((i > nn)&&(b[i-nn] > 0))
		return false;  //无解

	if(i > nn)
	{
		pivot(A, b, c, v, nn, nb, lab, i - nn, 0, 0);
		i = 0;
	}

	//修正oriv和oric
	for(j=0;j<=nn;j++)
	{
		c[j] = oric[j];
		oric[j] = 0;
	}
	for(j=0;j<=nn+nb;j++)
	{
		k = lab[j];
		if(k > nn)
			continue;
		if(j <= nn)
		{
			oric[j] += c[k];
		}else{
			for(s=0;s<=nn;s++)
			{
				oric[s] -= c[k] * A[j-nn][s];
			}
			oriv += c[k] * b[j-nn];
		}
	}
	if(i <= nn)
	{
		//交换lab[0]和lab[i]
		for(j=1;j<=nb;j++)
		{
			swap(A[j][i], A[j][0]);
		}
		swap(oric[i], oric[0]);
		swap(lab[0], lab[i]);
	}else{
		//pivot(A, b, oric, oriv, nn, nb, lab, i - nn, 0, 0);
	}
	return true;
}

int main()
{
	int i, j;
	while(scanf("%d%d",&n,&m)!=EOF)
	{
		for(i=0;i<=m+n;i++)
		{
			mlab[i] = i;
		}
		mv = 0;
		for(i=1;i<=n;i++)
		{
			scanf("%lf",&mc[i]);
		}
		for(i=1;i<=m;i++)
		{
			for(j=1;j<=n;j++)
				scanf("%lf",&mA[i][j]);
			scanf("%lf",&mb[i]);
		}
		simplex(mA, mb, mc, mlab, n, m, mx, mv);
		printf("Nasa can spend %.0f taka.\n", ceil(m*mv));  
	}
	return 0;
}