#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 3010
#define MAXM 9000005

int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos, now;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt, n2;

struct PR{
	int p, q;
}hate[MAXN],like[MAXN],dis[MAXN];
int sdis;
int n, A, B;

void addedge(int st, int ed)
{
	edge[ecnt] = ed;
	enext[ecnt] = ehead[st];
	ehead[st] = ecnt;
	ecnt++;
}

void Tarjan(int cur)  //Tarjan缩点
{
	int i, j;

	DFN[cur] = low[cur] = now;
	now++;
	oplst[pos] = cur;
	pos++;
	vis[cur] = 1;
	for(i=ehead[cur];i>=0;i=enext[i])
	{
		j = edge[i];
		if(vis[j] == 2)  //废除顶点
			continue;
		if(vis[j] == 0)
		{
			Tarjan(j);
			low[cur] = min(low[cur], low[j]);
		}else{
			low[cur] = min(low[cur], DFN[j]);
		}
	}
	if(DFN[cur] == low[cur])
	{
		do{
			pos--;
			j = oplst[pos];
			vis[j] = 2;
			parset[j] = n2;
			setcnt[cur]++;
		}while(cur != j);
		n2++;
	}
}

bool judge(int maxdis)
{
	int i, j;

	ecnt = 0;
	for(i=0;i<(n<<1);i++)
	{
		ehead[i] = -1;
	}
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if((dis[i].p + dis[j].p > maxdis) && (dis[i].q + dis[j].q > maxdis))
				return false;
			if(dis[i].p + dis[j].p > maxdis)
			{
				addedge(i, j + n);
				addedge(j, i + n);
			}
			if(dis[i].q + dis[j].q > maxdis)
			{
				addedge(i + n, j);
				addedge(j + n, i);
			}
			if(dis[i].p + sdis + dis[j].q > maxdis)
			{
				addedge(i, j);
				addedge(j + n, i + n);
			}
			if(dis[i].q + sdis + dis[j].p > maxdis)
			{
				addedge(i + n, j + n);
				addedge(j, i);
			}
		}
	}
	for(i=0;i<A;i++)
	{
		addedge(hate[i].p, hate[i].q + n);
		addedge(hate[i].q, hate[i].p + n);
		addedge(hate[i].p + n, hate[i].q);
		addedge(hate[i].q + n, hate[i].p);
	}
	for(i=0;i<B;i++)
	{
		addedge(like[i].p, like[i].q);
		addedge(like[i].q, like[i].p);
		addedge(like[i].p + n, like[i].q + n);
		addedge(like[i].q + n, like[i].p + n);
	}
	//Tarjan缩点
	n2 = 0;
	memset(vis, 0, sizeof(vis));
	now = 0;
	pos = 0;
	for(i=0;i<(n<<1);i++)
	{
		if(vis[i] == 0)
			Tarjan(i);
	}
	for(i=0;i<n;i++)
	{
		if(parset[i] == parset[i+n])  //同一组的不能在同一个强连通分量里
			return false;
	}
	return true;
}


int BiSearch()
{
	int begin = 0, end = 8000000, mid;

	while(end > begin + 1)
	{
		mid = (begin + end) / 2;
		if(judge(mid))
		{
			end = mid;
		}else{
			begin = mid;
		}
	}

	if(judge(begin))
	{
		return begin;
	}else if(judge(end)){
		return end;
	}else{
		return -1;
	}
}

int dist(int x1, int y1, int x2, int y2)
{
	return (labs(x2 - x1) + labs(y2 - y1));
}

bool input()
{
	int i, s1x, s1y, s2x, s2y, x, y;
	if(scanf("%d%d%d",&n,&A,&B)==EOF)
		return false;

	scanf("%d%d%d%d",&s1x,&s1y,&s2x,&s2y);
	sdis = dist(s1x, s1y, s2x, s2y);
	for(i=0;i<n;i++)
	{
		scanf("%d%d",&x,&y);
		dis[i].p = dist(s1x, s1y, x, y);
		dis[i].q = dist(s2x, s2y, x, y);
	}
	for(i=0;i<A;i++)
	{
		scanf("%d%d",&hate[i].p,&hate[i].q);
		hate[i].p--;
		hate[i].q--;
	}
	for(i=0;i<B;i++)
	{
		scanf("%d%d",&like[i].p,&like[i].q);
		like[i].p--;
		like[i].q--;
	}

	return true;
}

int main()
{
	while(input())
	{
		printf("%d\n",BiSearch());
	}
	return 0;
}
