/*
POJ2112 Optimal Milking

有K台挤奶机器和C头牛(统称为物体），每台挤奶机器只能容纳M头牛进行挤奶。
现在给出dis[K + C][K + C]的矩阵，dis[i][j]若不为0则表示第i个物体到第j个物体之间有路，dis[i][j]就是该路的长度。（1 <= K <= 30,1 <= C <= 200）

现在问你怎么安排这C头牛到K台机器挤奶，使得需要走最长路程到挤奶机器的奶牛所走的路程最少，求出这个最小值。


网络流模型：
每个奶牛最终都只能到达一个挤奶器，每个挤奶器只能有M个奶牛，可把奶牛看做网络流中的流。
每个奶牛和挤奶器都是一个节点，添加一个源，连边到所有奶牛节点，这些边容量都是1。
添加一个汇点，每个挤奶器都连边到它。这些边的容量都是M。
二分答案，对牛和机器建二分图即可。

*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 500
#define MAXI 0x7fffffff
#define MAXL 0x0fffffff

int graph[MAXP][MAXP]; //流量邻接矩阵
int depth[MAXP];       //节点层次
int sp,tp,np;          //源，汇，顶点数

int K, C, M, mdis;
int dis[MAXP][MAXP];

int build_depth()
{
	queue<int> oplst;
	int a, b;

	memset(depth, -1, sizeof(int) * np);
	depth[sp] = 0;
	oplst.push(sp);
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		for(b=0;b<np;b++)
		{
			if((depth[b] >= 0) || (graph[a][b] <= 0))
				continue;

			depth[b] = depth[a] + 1;
			if(b == tp)
				break;
			else 
				oplst.push(b);
		}
	}

	return depth[tp];
}

int find(int a, int low)
{
	int ret, b;

	if(a == tp)
		return low;
	for(b=0;b<np;b++)
	{
		if((depth[b] != depth[a] + 1)||(graph[a][b] <= 0))  //必须是b=a+1才可以，差更多不可以
			continue;
		ret = find(b, min(low, graph[a][b]));
		if(ret <= 0)
			continue;
		graph[a][b] -= ret;
		graph[b][a] += ret;
		return ret;
	}
	return 0;
}

int Dinic()
{
	int ret, ad;

	ret = 0;
	while(build_depth() > 0)
	{
		ad = find(sp, MAXI);
		while(ad > 0)
		{
			ret += ad;
			ad = find(sp, MAXI);
		}
	}

	return ret;
}

void Floyd(int n)   //Floyd暴力三角求距离
{
	int i,j,k;
	for(k=1;k<=n;k++)
	{
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				if((i != k)&&(j != k))
				{
					if(dis[i][j] > dis[i][k] + dis[k][j])
						dis[i][j] = dis[i][k] + dis[k][j];
				}
			}
		}
	}
}

bool judge(int mindis)
{
	int i, j;

	memset(graph, 0, sizeof(graph));
	sp = 0;
	tp = K + C + 1;
	np = K + C + 2;
	for(i=K+1;i<=K+C;i++)
	{
		graph[0][i] = 1;
	}
	for(j=1;j<=K;j++)
	{
		graph[j][tp] = M;
	}
	for(i=K+1;i<=K+C;i++)
	{
		for(j=1;j<=K;j++)
		{
			graph[i][j] = (dis[i][j] <= mindis) ? 1 : 0;
		}
	}

	return (Dinic() == C);
}

int bisearch()
{
	int start,end,mid;

	start = 0;
	end = mdis;

	while(end > start + 1)
	{
		mid = (start + end) / 2;
		if(judge(mid))
		{
			end = mid;
		}else{
			start = mid;
		}
	}

	if(judge(start))
		return start;
	return end;
}

bool input()
{
	int i, j;

	if(scanf("%d%d%d",&K,&C,&M)==EOF)
		return false;

	for(i=1;i<=K+C;i++)
	{
		for(j=1;j<=K+C;j++)
		{
			scanf("%d",&dis[i][j]);
			if((i != j)&&(dis[i][j] == 0))
				dis[i][j] = MAXL;
			mdis = max(mdis, dis[i][j]);
		}
	}
	Floyd(K+C);
	mdis = 0;
	for(i=1;i<=K+C;i++)
	{
		for(j=1;j<=K+C;j++)
		{
			mdis = max(mdis, dis[i][j]);
		}
	}

	return true;
}

int main()
{
	while(input())
	{
		printf("%d\n",bisearch());
	}

	return 0;
}