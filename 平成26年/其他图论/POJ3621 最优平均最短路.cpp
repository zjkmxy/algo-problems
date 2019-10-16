//二分判负环，注意复环（经过一个点两次）负权必含负权子环，所以不需要考虑
#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAXN 1005
#define MAXM 5005
#define MAXI 0x3fffffff

struct NODE
{
	bool inol;
	double dis;
	int weigh;
	int incnt;
	int head;
}node[MAXN];

struct EDGE
{
	int ed;
	int len;
	int next;
}edge[MAXM];

int N, M;

bool SPFA(double ave)
{
	int i, cur, ed;
	double dis;
	queue<int> oplst;

	for(i=1;i<=N;i++)
	{
		node[i].dis = MAXI;
		node[i].incnt = 0;
		node[i].inol = false;
	}
	node[1].dis = 0;
	node[1].incnt = 1;
	node[1].inol = true;
	oplst.push(1);
	while(!oplst.empty())
	{
		cur = oplst.front();
		oplst.pop();
		node[cur].inol = false;
		for(i=node[cur].head;i>=0;i=edge[i].next)
		{
			ed = edge[i].ed;
			dis = node[cur].dis + edge[i].len * ave - node[ed].weigh;
			if(node[ed].dis > dis)
			{
				node[ed].dis = dis;
				node[ed].incnt++;
				if(!node[ed].inol)
				{
					oplst.push(ed);
					node[ed].inol = true;
				}
				if(node[ed].incnt > N)
					return true;
			}
		}
	}
	return false;
}

double BiSech()
{
	double a = 0.0, b = MAXI, mid;

	while(b - a > 0.0001)
	{
		mid = (b + a) / 2.0;
		if(SPFA(mid))
			a = mid;
		else
			b = mid;
		if(b < 10.0)
			b = b;
	}
	return (a + b) / 2.0;
}

int main()
{
	int i, a, b, c;

	scanf("%d%d",&N,&M);
	for(i=1;i<=N;i++)
	{
		scanf("%d",&node[i].weigh);
		node[i].head = -1;
	}
	for(i=1;i<=M;i++)
	{
		scanf("%d%d%d",&a,&b,&c);
		edge[i].ed = b;
		edge[i].len = c;
		edge[i].next = node[a].head;
		node[a].head = i;
	}
	printf("%.2lf\n",BiSech());
	return 0;
}
