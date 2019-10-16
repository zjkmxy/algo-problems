#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
using namespace std;

#define MAXN 5005
#define MAXM 5000005

int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos = 0, now = 0;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt;

int n2, odeg[MAXN];
int n, m;

void addedge(int st, int ed)
{
	edge[ecnt] = ed;
	enext[ecnt] = ehead[st];
	ehead[st] = ecnt;
	ecnt++;
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
	int i, a, b;

	scanf("%d%d",&n,&m);
	if(n==0)
	{
		return false;
	}
	memset(vis, 0, sizeof(vis));
	memset(ehead, -1, sizeof(ehead));
	memset(odeg, 0, sizeof(odeg));
	ecnt = 0;
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

			odeg[parset[i]]++;
		}
	}
	a = 0;
	for(i=1;i<=n;i++)
	{
		if(odeg[parset[i]] == 0)
		{
			a++;
			printf(a==1?"%d":" %d",i);
		}
	}
	printf("\n");
	return true;
}

int main()
{
	while(domain());
	return 0;
}
