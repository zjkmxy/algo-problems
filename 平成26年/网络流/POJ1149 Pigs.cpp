/*
Poj 1149 pigs
可以如下理解：
每个人先拿到能打开的所有猪，买走合适的，剩下的留着。
当其他有能打开该笼子的人来时，直接从这个人手里拿猪。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 300
#define MAXI 0x7fffffff
#define MAXC 0x0fffffff

int graph[MAXP][MAXP]; //流量邻接矩阵
int dist[MAXP];        //反向节点层次数组
int gap[MAXP];         //dist=i的节点个数
int pprev[MAXP];       //前驱结点
int lowf[MAXP];        //最小载流量（因为要回溯，所以必须用数组记）
int sp,tp,np;          //源，汇，顶点数

#define MAXN 2000
int cage[MAXN];    //猪的个数
int owner[MAXN];   //上一个买主
int n,m;

int build_dist()
{
	queue<int> oplst;
	int a, b;

	memset(dist, -1, sizeof(int) * np);
	memset(gap, 0, sizeof(int) * np);
	dist[tp] = 0;
	gap[0]++;
	oplst.push(tp);
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		for(b=0;b<np;b++)
		{
			if((dist[b] >= 0) || (graph[b][a] <= 0))
				continue;

			dist[b] = dist[a] + 1;
			gap[dist[b]]++;
			oplst.push(b);
		}
	}

	return dist[sp];
}

int SAPCalc()
{
	int a, b, ret, mindis;

	ret = 0;
	if(build_dist() == -1)
		return 0;
	a = sp;
	memset(lowf, 0, sizeof(int) * np);
	memset(pprev, -1, sizeof(int) * np);
	while(dist[sp] < np)
	{
		lowf[sp] = MAXI;
		for(b=0;b<np;b++)
		{
			if((graph[a][b] > 0)&&(dist[a] == dist[b] + 1)&&(dist[b] >= 0))
			{
				break;
			}
		}
		if(b < np)  //找到允许边
		{
			lowf[b] = min(lowf[a], graph[a][b]);
			pprev[b] = a;
			a = b;
			if(a == tp) //找到增广路
			{
				ret += lowf[tp];
				while(pprev[a] != -1)
				{
					graph[pprev[a]][a] -= lowf[tp];
					graph[a][pprev[a]] += lowf[tp];
					a = pprev[a];
				}//开始下一次的寻找
				a = sp;
				memset(lowf, 0, sizeof(int) * np);
				memset(pprev, -1, sizeof(int) * np);
			}
		}else{      //没有允许边
			//重标号dist
			mindis = MAXI;
			for(b=0;b<np;b++)
			{
				if((graph[a][b] > 0)&&(mindis > dist[b] + 1)&&(dist[b] >= 0))
				{
					mindis = dist[b] + 1;
				}
			}
			gap[dist[a]]--;
			if(gap[dist[a]] == 0)
				break;
			dist[a] = mindis;
			gap[mindis]++;
			if(a != sp)
				a = pprev[a];
		}
	}

	return ret;
}

bool input()
{
	int i, j, a, b, k;

	if(scanf("%d%d",&m,&n)==EOF)
		return false;

	for(i=1;i<=m;i++)
		scanf("%d",&cage[i]);

	memset(owner, 0, sizeof(int) * (m + 1));
	memset(graph, 0, sizeof(graph));
	for(i=1;i<=n;i++)
	{
		scanf("%d",&a);
		for(j=0;j<a;j++)
		{
			scanf("%d",&k);
			if(owner[k] == 0)
			{
				graph[0][i] += cage[k];
				owner[k] = i;
			}else{
				graph[owner[k]][i] = MAXC;
				owner[k] = i;
			}
		}
		scanf("%d",&b);
		graph[i][n+1] = b;
	}
	sp = 0;
	tp = n + 1;
	np = n + 2;

	return true;
}

int main()
{
	while(input())
	{
		printf("%d\n",SAPCalc());
	}
	return 0;
}