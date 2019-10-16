/*
先求到T的距离，然后A-star做。
*/
#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAXN 1005
#define MAXM 100100
#define MAXI 0x3fffffff

struct NODE
{
	int head;
	int tail;
	int dist;
	bool inol;
}node[MAXN];

struct STEP
{
	short p;
	int len;
};

struct EDGE
{
	short st;
	short ed;
	short len;
	int next;
	int prev;
}edge[MAXM];

int M, ecnt;
int N, S, T, K;

bool operator<(const STEP a, const STEP b)
{
	if(a.len + node[a.p].dist != b.len + node[b.p].dist)
		return (a.len + node[a.p].dist < b.len + node[b.p].dist);
	else
		return (a.len < b.len);
}

void search_init()
{
	queue<STEP> oplst;
	STEP cur;
	int j;
	int ed;

	cur.p = T;
	cur.len = 0;
	oplst.push(cur);
	node[T].dist = 0;
	node[T].inol = true;
	while(!oplst.empty())
	{
		cur = oplst.front();
		oplst.pop();
		node[cur.p].inol = false;
		for(j=node[cur.p].tail;j>=0;j=edge[j].prev)
		{
			ed = edge[j].st;
			if(node[ed].dist > cur.len + edge[j].len)
			{
				cur.p = ed;
				cur.len += edge[j].len;
				node[ed].dist = cur.len;
				if(!node[cur.p].inol)
				{
					oplst.push(cur);
					node[cur.p].inol = true;
				}
				cur.len -= edge[j].len;
			}
		}
	}
}

void search_init2()
{
	queue<int> oplst;
	int cur;
	int j;
	int ed;

	cur = T;
	oplst.push(cur);
	node[T].dist = 0;
	node[T].inol = true;
	while(!oplst.empty())
	{
		cur = oplst.front();
		oplst.pop();
		node[cur].inol = false;
		for(j=node[cur].tail;j>=0;j=edge[j].prev)
		{
			ed = edge[j].st;
			if(node[ed].dist > node[cur].dist + edge[j].len)
			{
				node[ed].dist = node[cur].dist + edge[j].len;
				if(!node[ed].inol)
				{
					oplst.push(ed);
					node[ed].inol = true;
				}
			}
		}
	}
}

int search_body()
{
	priority_queue<STEP> oplst;
	STEP cur;
	int j;

	if(node[S].dist == MAXI)
		return -1;
	if(S == T)  //当s==t时,距离为0的路不能算在这k短路中,所以需要求k+1短路;  
		K++;
	cur.p = S;
	cur.len = 0;
	oplst.push(cur);
	while(!oplst.empty())
	{
		cur = oplst.top();
		oplst.pop();
		if(cur.p == T)
		{
			K--;
			if(K == 0)
				return cur.len;
		}
		for(j=node[cur.p].head;j>=0;j=edge[j].next)
		{
			cur.p = edge[j].ed;
			cur.len += edge[j].len;
			oplst.push(cur);
			cur.len -= edge[j].len;
		}
	}
	return -1;
}

void add_edge(short a, short b, short len)
{
	edge[ecnt].st = a;
	edge[ecnt].ed = b;
	edge[ecnt].len = len;
	edge[ecnt].next = node[a].head;
	node[a].head = ecnt;
	edge[ecnt].prev = node[b].tail;
	node[b].tail = ecnt;
	ecnt++;
}

void input()
{
	int i, a, b, k;

	scanf("%d%d",&N,&M);
	ecnt = 0;
	for(i=1;i<=N;i++)
	{
		node[i].dist = MAXI;
		node[i].head = -1;
		node[i].tail = -1;
		node[i].inol = false;
	}
	for(i=0;i<M;i++)
	{
		scanf("%d%d%d",&a,&b,&k);
		add_edge(a, b, k);
	}
	scanf("%d%d%d",&S,&T,&K);
}

int main()
{
	input();
	search_init2();
	printf("%d\n",search_body());
	return 0;
}
