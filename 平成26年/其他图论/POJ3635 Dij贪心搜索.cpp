#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAXM 20005
#define MAXN 1005
#define MAXF 105
#define MAXI 0x3fffffff

struct STAT
{
	int pos;
	int fuel;
	int cost;
	STAT(int p=0, int f=0, int c=0)
	{
		pos = p;
		fuel = f;
		cost = c;
	};
};

bool operator<(const STAT a, const STAT other)
{
		return a.cost > other.cost;
}

struct NODE
{
	int head;
	int price;
	int cost[MAXF];
	bool vis[MAXF];
}node[MAXN];

struct EDGE
{
	int ed;
	int len;
	int next;
}edge[MAXM];
int N, M, ST, ED, cap, Q, ecnt;

int Dijkstra()
{
	int j, ed, tmp;
	STAT cur(ST, 0, 0);
	priority_queue<STAT> oplst;

	node[ST].cost[0] = 0;
	oplst.push(cur);
	while(!oplst.empty())
	{
		cur = oplst.top();
		oplst.pop();
		node[cur.pos].vis[cur.fuel] = true;
		if(cur.pos == ED)
		{
			return cur.cost;
		}
		//在可能加油最优的场合，加一点油
		if(cur.fuel + 1 <= cap && !node[cur.pos].vis[cur.fuel+1] && node[cur.pos].cost[cur.fuel+1] > cur.cost + node[cur.pos].price)
		{
			node[cur.pos].cost[cur.fuel+1] = cur.cost + node[cur.pos].price;
			oplst.push(STAT(cur.pos, cur.fuel+1, cur.cost + node[cur.pos].price));
		}
		//起跑
		for(j=node[cur.pos].head;j>=0;j=edge[j].next)
		{
			tmp = cur.fuel - edge[j].len;
			ed = edge[j].ed;
			if(tmp < 0)
				continue;
			if(node[ed].vis[tmp] || node[ed].cost[tmp] <= cur.cost)
				continue;
			node[ed].cost[tmp] = cur.cost;
			oplst.push(STAT(ed, tmp, cur.cost));
		}
	}
	//return node[ED].cost[0];
	return MAXI;
}

int main()
{
	int i, j, a, b, c;

	scanf("%d%d",&N,&M);
	for(i=0;i<N;i++)
	{
		scanf("%d",&node[i].price);
		node[i].head = -1;
	}
	ecnt = 0;
	for(i=0;i<M;i++)
	{

		scanf("%d%d%d",&a,&b,&c);
		edge[ecnt].ed = b;
		edge[ecnt].len = c;
		edge[ecnt].next = node[a].head;
		node[a].head = ecnt;
		ecnt++;
		edge[ecnt].ed = a;
		edge[ecnt].len = c;
		edge[ecnt].next = node[b].head;
		node[b].head = ecnt;
		ecnt++;
	}
	scanf("%d",&Q);
	for(a=0;a<Q;a++)
	{
		scanf("%d%d%d",&cap,&ST,&ED);
		for(i=0;i<N;i++)
		{
			for(j=0;j<=cap;j++)
			{
				node[i].vis[j] = false;
				node[i].cost[j] = MAXI;
			}
		}
		b = Dijkstra();
		if(b < MAXI)
			printf("%d\n",b);
		else
			printf("impossible\n");
	}
	return 0;
}
