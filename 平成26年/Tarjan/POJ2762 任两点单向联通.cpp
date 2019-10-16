#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
using namespace std;

#define MAXN 1005
#define MAXM 5000005

int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos = 0, now = 0;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt;
int ehead2[MAXN], edge2[MAXM], enext2[MAXM], ecnt2, n2, indeg[MAXN];
int n, m;

void addedge(int st, int ed)
{
	edge[ecnt] = ed;
	enext[ecnt] = ehead[st];
	ehead[st] = ecnt;
	ecnt++;
}

void addedge2(int st, int ed)
{
	edge2[ecnt2] = ed;
	enext2[ecnt2] = ehead2[st];
	ehead2[st] = ecnt2;
	ecnt2++;
}

void Tarjan(int cur)  //TarjanËõµã
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
		if(vis[j] == 2)  //·Ï³ý¶¥µã
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

bool domain()
{
	int i, j, a, b;
	bool flag = true;

	scanf("%d%d",&n,&m);
	if(n==0)
	{
		return false;
	}
	memset(vis, 0, sizeof(vis));
	memset(ehead, -1, sizeof(ehead));
	memset(ehead2, -1, sizeof(ehead2));
	memset(indeg, 0, sizeof(indeg));
	ecnt = 0;
	ecnt2 = 0;
	n2 = 0;
	now = 1;
	for(i=1;i<=m;i++)
	{
		scanf("%d%d",&a,&b);
		addedge(a, b);
	}
	for(i=1;i<=n;i++)
	{
		if(vis[i] == 0)
			Tarjan(i);
	}
	for(i=1;i<=n;i++)
	{
		for(a=ehead[i];a>=0;a=enext[a])
		{
			if(parset[i] == parset[edge[a]])
				continue;

			addedge2(parset[i], parset[edge[a]]);
			indeg[parset[edge[a]]]++;
		}
	}

	for(j=0;j<n2;j++)
	{
		b = 0;
		for(i=0;i<n2;i++)
		{
			if(indeg[i] == 0)
			{
				b++;
				indeg[i] = -1;
				if(b > 1)
					flag = false;
				for(a=ehead2[i];a>=0;a=enext2[a])
				{
					indeg[edge2[a]]--;
				}
			}
		}
	}
	return flag;
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		if(domain())
		{
			printf("Yes\n");
		}else{
			printf("No\n");
		}
	}
	return 0;
}
