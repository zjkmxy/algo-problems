#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

#define MAXN 1000005

const int tpn[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

struct EDGE
{
	int x, y, w;
	int next;
	bool vis;
}edge[MAXN];
int ehead[MAXN], ecnt, indeg[MAXN];
int ST[MAXN], IND[MAXN], RES[MAXN], EGE[MAXN], ind, res;
int n;

void addedge(int x, int y, int w)
{
	edge[ecnt].x = x;
	edge[ecnt].y = y;
	edge[ecnt].w = w;
	edge[ecnt].next = ehead[x];
	edge[ecnt].vis = false;
	ehead[x] = ecnt;
	ecnt++;
}

void buildmap()
{
	int i, dst, j;

	ecnt = 0;
	for(i=0;i<tpn[n-1];i++)
	{
		ehead[i] = -1;
		indeg[i] = 10;
	}
	for(i=0;i<tpn[n-1];i++)
	{
		dst = (i * 10) % tpn[n-1];
		for(j=9;j>=0;j--)
		{
			addedge(i, dst+j, j);
		}
	}
}

void DFSVis(int st)
{
	int i;
	bool flag;

	ind = 0;
	res = 0;
	ST[0] = st;
	IND[0] = ehead[st];
	while(ind >= 0)
	{
		flag = false;
		for(i=IND[ind];i>=0;i=edge[i].next)
		{
			if(edge[i].vis)
				continue;

			edge[i].vis = true;
			ind++;
			ST[ind] = edge[i].y;
			IND[ind] = ehead[edge[i].y];
			EGE[ind] = i;
			flag = true;
			break;
		}
		if(flag)
			continue;

		RES[res] = EGE[ind];
		res++;
		ind--;
	}
	res--;
}

int main()
{
	int i;

	while(true)
	{
		scanf("%d",&n);
		if(n == 0)
			break;
		if(n == 1)
		{
			printf("0123456789\n");
			continue;
		}
		buildmap();
		for(i=0;i<n-1;i++)
			printf("0");
		DFSVis(0);
		for(res--;res>=0;res--)
		{
			printf("%d",edge[RES[res]].w);
		}
		printf("\n");
	}
	return 0;
}
