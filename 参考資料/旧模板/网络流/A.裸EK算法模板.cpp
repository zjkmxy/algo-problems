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
int pprev[MAXP];       //前驱结点
bool visit[MAXP];
int sp,tp,np;             //源，汇，顶点数

int Augment()
{
	queue<int> oplst;
	int ret, a, b;

	memset(pprev, -1, sizeof(int) * np);
	memset(visit, 0, sizeof(bool) * np);
	oplst.push(sp);
	visit[sp] = true;
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		//这里不改visit
		for(b=0;b<np;b++)
		{
			if((graph[a][b] <= 0)||(visit[b]))
				continue;

			pprev[b] = a;
			visit[b] = true;
			if(b == tp)
			{
				break;
			}else{
				oplst.push(b);
			}
		}
	}
	if(pprev[tp] == -1)
		return 0;

	ret = MAXI;
	a = tp;
	while(pprev[a] != -1)
	{
		ret = min(ret, graph[pprev[a]][a]);
		a = pprev[a];
	}
	a = tp;
	while(pprev[a] != -1)
	{
		graph[pprev[a]][a] -= ret;
		graph[a][pprev[a]] += ret;
		a = pprev[a];
	}
	return ret;
}

int EKCalc()
{
	int ret, ad;

	ret = 0;
	ad = Augment();
	while(ad > 0)
	{
		ret += ad;
		ad = Augment();
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
		printf("%d\n",EKCalc());
	}

	return 0;
}