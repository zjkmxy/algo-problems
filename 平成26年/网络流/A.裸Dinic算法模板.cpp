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
int depth[MAXP];       //节点层次
int sp,tp,np;          //源，汇，顶点数

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
		printf("%d\n",Dinic());
	}

	return 0;
}