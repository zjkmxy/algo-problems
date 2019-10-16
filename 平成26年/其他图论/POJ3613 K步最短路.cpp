//Floyd + 倍增
#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAXT 205
#define MAXI 1005
#define MAXK 25
#define MAXINT 0x3fffffff

int icode[MAXI];  //Ii的连续映射
int Floyd[MAXK][MAXT][MAXT];  //连续K步最短路
int ans[2][MAXT][MAXT];
int N, T, K, S, E;

void FloydCalc()
{
	int p, q, i, j, k;
	
	//1步最短路已经处理好了
	for(p=1,q=2;q<=K;p++,q<<=1)
	{
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				Floyd[p][i][j] = MAXINT;
			}
		}
		for(j=0;j<N;j++)
		{
			for(i=0;i<N;i++)
			{
				for(k=0;k<N;k++)
				{
					Floyd[p][i][k] = min(Floyd[p][i][k], Floyd[p-1][i][j] + Floyd[p-1][j][k]);
				}
			}
		}
	}
}

int GetAns()
{
	int i, j, k, p, q;
	int cur = 0;

	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			if(i == j)
				ans[0][i][j] = 0;
			else
				ans[0][i][j] = MAXINT;
		}
	}
	cur = 1;
	for(p=0,q=1;q<=K;p++,q<<=1)
	{
		if((K & q)==0)
			continue;

		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				ans[cur][i][j] = MAXINT;
			}
		}
		for(k=0;k<N;k++)
		{
			for(i=0;i<N;i++)
			{
				for(j=0;j<N;j++)
				{
					ans[cur][i][j] = min(ans[cur][i][j], ans[cur^1][i][k] + Floyd[p][k][j]);
				}
			}
		}
		cur^=1;
	}
	return ans[cur^1][S][E];
}

void input()
{
	int i, j;
	int l, i1, i2;

	scanf("%d%d%d%d",&K,&T,&S,&E);
	memset(icode, -1, sizeof(icode));
	for(i=0;i<MAXT;i++)
	{
		for(j=0;j<MAXT;j++)
		{
			Floyd[0][i][j] = MAXINT;
		}
	}
	icode[S] = 0;
	if(S != E)
	{
		icode[E] = 1;
		N = 2;
		S = 0;
		E = 1;
	}else{
		N = 1;
		S = 0;
		E = 0;
	}
	for(i=0;i<T;i++)
	{
		scanf("%d%d%d",&l,&i1,&i2);
		if(icode[i1] < 0)
		{
			icode[i1] = N;
			i1 = N;
			N++;
		}else i1 = icode[i1];
		if(icode[i2] < 0)
		{
			icode[i2] = N;
			i2 = N;
			N++;
		}else i2 = icode[i2];
		Floyd[0][i1][i2] = min(Floyd[0][i1][i2], l);
		Floyd[0][i2][i1] = min(Floyd[0][i2][i1], l);
	}
}

int main()
{
	input();
	FloydCalc();
	printf("%d\n",GetAns());
	return 0;
}
