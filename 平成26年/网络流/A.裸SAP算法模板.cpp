#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 300
#define MAXI 0x7fffffff

int graph[MAXP][MAXP]; //流量邻接矩阵
int dist[MAXP];        //反向节点层次数组
int gap[MAXP];         //dist=i的节点个数
int pprev[MAXP];       //前驱结点
int lowf[MAXP];        //最小载流量（因为要回溯，所以必须用数组记）
int sp,tp,np;          //源，汇，顶点数

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
			if(mindis >= MAXP)  //这里不加的话会RE……
				break;
			gap[mindis]++;
			if(a != sp)
				a = pprev[a];
		}
	}

	return ret;
}

bool input()
{
	int n, i, a, b, w;

	if(scanf("%d%d",&n,&np)==EOF)
		return false;

	sp = 0;
	tp = np - 1;
	memset(graph, 0, sizeof(graph));
	for(i=0;i<n;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		a--;
		b--;
		graph[a][b] += w;
	}
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