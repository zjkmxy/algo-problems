//HDU4405
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXN 100005
#define MAXM 1005

struct NODE{
	double dp;
	int head;
	int tail;
}node[MAXN];
struct EDGE{
	int st;
	int ed;
	int hnext;
	int tnext;
}edge[MAXM];
int m, n;
const double p = 1.00 / 6.00;

double calc_dp()
{
	int i, j;

	for(i=n;i>=0;i--)
	{
		if(node[i].dp < 0)
		{
			node[i].dp = 0;
			for(j=1;j<=6 && i+j<=n;j++)
			{
				node[i].dp += node[i+j].dp * p;
			}
			if(i < n)
				node[i].dp++;
		}
		for(j=node[i].tail;j>=0;j=edge[j].tnext)
		{
			//if(i < n)
			//{
				node[edge[j].st].dp = node[i].dp;
			//}else{
			//	node[edge[j].st].dp = 1;  //直接移动到终点符合题意吗？
			//}
		}
	}

	return node[0].dp;
}

bool input()
{
	int i;
	scanf("%d%d",&n,&m);
	if(n==0 && m==0)
		return false;

	for(i=0;i<=n;i++)
	{
		node[i].dp = -1;
		node[i].head = -1;
		node[i].tail = -1;
	}
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&edge[i].st,&edge[i].ed);
		edge[i].hnext = node[edge[i].st].head;
		node[edge[i].st].head = i;
		edge[i].tnext = node[edge[i].ed].tail;
		node[edge[i].ed].tail = i;
	}

	return true;
}

int main()
{
	while(input())
	{
		printf("%.4lf\n",calc_dp());
	}
	return 0;
}
