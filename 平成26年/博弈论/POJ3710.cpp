/*
拆树枝问题，奇数环视同1，偶数环视同0
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 5005

struct NODE
{
	int head;
	int SG;
	int seq;
}node[MAXN];

struct EDGE
{
	int ed;
	int next;
	int revs;
	bool par;
}edge[MAXN];

int gseq, N, n, m, ecnt;

int DFS(int id, int dep)
{
	int j, ed, sg;

	node[id].seq = dep;
	node[id].SG = 0;
	for(j=node[id].head;j>=0;j=edge[j].next)
	{
		if(edge[j].par)
			continue;
		edge[j].par = true;
		edge[j+edge[j].revs].par = true;
		ed = edge[j].ed;
		if(node[ed].seq < dep)
		{
			gseq = node[ed].seq;
			if((node[ed].seq - dep) % 2 == 0)
				return -1;  //奇数环
			else
				return -2;  //偶数环
		}else{
			sg = DFS(ed, dep+1);
			if(sg < 0 && gseq < dep)
				return sg;
			if(sg < 0)
			{
				sg += 2;
			}else{
				sg += 1;
			}
			node[id].SG ^= sg;
		}
	}
	return node[id].SG;
}

bool domain()
{
	int i, j, ret = 0;
	int a, b;

	if(scanf("%d",&N)==EOF)
		return false;

	for(i=0;i<N;i++)
	{
		ecnt = 0;
		scanf("%d%d",&n,&m);
		for(j=1;j<=n;j++)
		{
			node[j].SG = -1;
			node[j].seq = 0x3fffffff;
			node[j].head = -1;
		}
		for(j=1;j<=m;j++)
		{
			scanf("%d%d",&a,&b);
			edge[ecnt].ed = b;
			edge[ecnt].par = false;
			edge[ecnt].next = node[a].head;
			edge[ecnt].revs = 1;
			node[a].head = ecnt;
			ecnt++;
			edge[ecnt].ed = a;
			edge[ecnt].par = false;
			edge[ecnt].next = node[b].head;
			edge[ecnt].revs = -1;
			node[b].head = ecnt;
			ecnt++;
		}
		ret ^= DFS(1, 1);
	}

	if(ret > 0)
		printf("Sally\n");
	else
		printf("Harry\n");

	return true;
}

int main()
{
	while(domain());
	return 0;
}
