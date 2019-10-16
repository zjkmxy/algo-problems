#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 10005
#define MAXM 1000005

int n, m;
int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos;

int bbc_cnt;
struct EDGE
{
	int a, b;
	int next;
	int btag;
}edge[MAXM];
int ehead[MAXN], bbc_rt[MAXN], ecnt;
int color[MAXN], valid[MAXN];

int map[MAXN][MAXN];

void Tarjan(int cur, int dep, int par)  //Tarjan算法查点双连通分量
{
	int i, j, tmp;

	DFN[cur] = low[cur] = dep;
	vis[cur] = 1;
	for(i=ehead[cur];i>=0;i=edge[i].next)
	{
		j = edge[i].b;
		if(vis[j] == 2)  //废除顶点
			continue;
		if(vis[j] == 0)
		{
			oplst[pos++] = i;
			Tarjan(j, dep+1, cur);
			low[cur] = min(low[cur], low[j]);
			if(low[j] >= DFN[cur])
			{
				//出栈，标记双联通
				while(pos > 0)
				{
					pos--;
					tmp = oplst[pos];
					edge[tmp].btag = bbc_cnt;
					if((edge[tmp].a==cur&&edge[tmp].b==j) || (edge[tmp].a==j&&edge[tmp].b==cur))
						break;
				}
				bbc_rt[bbc_cnt] = cur;
				bbc_cnt++;
			}
		}else if(j != par)
		{
			low[cur] = min(low[cur], DFN[j]);
			if(DFN[j] < DFN[cur])
				oplst[pos++] = i;
		}
	}
}

bool dfs_col(int cur, int col, int tag)
{
	int i, j;
	color[cur] = col;
	col ^= 1;
	for(i=ehead[cur];i>=0;i=edge[i].next)
	{
		j = edge[i].b;
		if(edge[i].btag != tag)
			continue;
		if(color[j] == col)
			continue;
		if(color[j] == -1)
		{
			if(!dfs_col(j, col, tag))
				return false;
		}else{
			return false;
		}
	}
	return true;
}

void dfs_flag(int cur, int tag)
{
	int i, j;
	vis[cur] = 1;
	valid[cur] = 1;
	for(i=ehead[cur];i>=0;i=edge[i].next)
	{
		j = edge[i].b;
		if(edge[i].btag != tag)
			continue;
		if(vis[j])
			continue;
		dfs_flag(j, tag);
	}
}

int judge()
{
	int i, j, ret = 0;

	memset(vis, 0, sizeof(vis));
	memset(valid, 0, sizeof(valid));
	pos = 0;
	bbc_cnt = 0;
	for(i=0;i<n;i++)
	{
		if(vis[i] == 0)
			Tarjan(i, 1, -1);
	}
	for(i=0;i<bbc_cnt;i++)
	{
		memset(vis, 0, sizeof(vis));
		memset(color, -1, sizeof(color));
		if(!dfs_col(bbc_rt[i], 0, i))
		{
			//存在奇数环
			dfs_flag(bbc_rt[i], i);
		}
	}
	for(i=0;i<n;i++)
	{
		if(valid[i] == 1)
			ret++;
	}
	return ret;
}

void addedge(int a, int b)
{
	edge[ecnt].btag = -1;
	edge[ecnt].next = ehead[a];
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	ehead[a] = ecnt++;
	edge[ecnt].btag = -1;
	edge[ecnt].next = ehead[b];
	edge[ecnt].a = b;
	edge[ecnt].b = a;
	ehead[b] = ecnt++;
}

bool input()
{
	int i, j, a, b;
	if(scanf("%d%d",&n,&m)==EOF)
		return false;
	if(n==0)
		return false;

	ecnt = 0;
	for(i=0;i<n;i++)
	{
		ehead[i] = -1;
		for(j=0;j<n;j++)
		{
			map[i][j] = 0;
		}
	}
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&a,&b);
		a--;
		b--;
		map[a][b] = map[b][a] = 1;
	}
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(map[i][j] == 0)
				addedge(i, j);
		}
	}

	return true;
}

int main()
{
	int ret;
	while(input())
	{
		ret  = judge();
		printf("%d\n",n-ret);
	}
	return 0;
}
