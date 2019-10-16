#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 100005
#define MAXM 1000005

int n, m, q, T = 0;
int DFN[MAXN], low[MAXN], vis[MAXN], revs[MAXM];
int oplst[MAXN], pos;

struct EDGE
{
	int a, b;
	int next;
	bool brg;
}edge[MAXM];
int ehead[MAXN], ecnt, parent[MAXN];
int bbc_lab[MAXN], bbc_cnt, brg_cnt;

void Tarjan(int cur, int dep, int par)  //Tarjan算法查边双连通分量
{
	int i, j, tmp;

	DFN[cur] = low[cur] = dep;
	parent[cur] = par;
	vis[cur] = 1;
	oplst[pos++] = cur;
	for(i=ehead[cur];i>=0;i=edge[i].next)
	{
		j = edge[i].b;
		if(vis[j] == 2)  //废除顶点
			continue;
		if(vis[j] == 0)
		{
			Tarjan(j, dep+1, i);
			low[cur] = min(low[cur], low[j]);
			
			if(low[j] > DFN[cur])
			{
				brg_cnt++;
				edge[i].brg = true;
			}
		}else if(i != par && i!=revs[par])  //判重边用
		{
			low[cur] = min(low[cur], DFN[j]);
		}
	}

	if(DFN[cur] == low[cur])
	{
		bbc_lab[cur] = cur;
		pos--;
		while(oplst[pos] != cur)
		{
			bbc_lab[oplst[pos]] = cur;
			pos--;
		}
		bbc_cnt++;
	}
}


void addedge(int a, int b)
{
	edge[ecnt].next = ehead[a];
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	revs[ecnt] = ecnt + 1;
	ehead[a] = ecnt++;
	edge[ecnt].next = ehead[b];
	edge[ecnt].a = b;
	edge[ecnt].b = a;
	revs[ecnt] = ecnt - 1;
	ehead[b] = ecnt++;
}

void query2(int a, int b)
{
	int i;

	a = bbc_lab[a];
	b = bbc_lab[b];
	while(a != b)
	{
		if(DFN[a] >= DFN[b])
		{
			i = parent[a];
			if(edge[i].brg)
			{
				edge[i].brg = false;
				brg_cnt--;
			}
			a = bbc_lab[edge[i].a];
		}
		if(DFN[b] > DFN[a])
		{
			i = parent[b];
			if(edge[i].brg)
			{
				edge[i].brg = false;
				brg_cnt--;
			}
			b = bbc_lab[edge[i].a];
		}
	}

	printf("%d\n",brg_cnt);
}

bool domain()
{
	int i, a, b;

	scanf("%d%d",&n,&m);
	if(n == 0)
		return false;
	T++;
	if(T > 1)
		printf("\nCase %d:\n",T);
	else
		printf("Case 1:\n");

	ecnt = 0;
	bbc_cnt = brg_cnt = pos = 0;
	memset(vis, 0, sizeof(vis));
	memset(ehead, -1, sizeof(ehead));
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&a,&b);
		addedge(a, b);
	}
	Tarjan(1, 1, -1);
	scanf("%d",&q);
	for(i=0;i<q;i++)
	{
		scanf("%d%d",&a,&b);
		query2(a, b);
	}

	return true;
}

int main()
{
	while(domain());
	return 0;
}
