#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 2005
#define MAXM 5000005

int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos = 0, now = 0;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt;

int ehead2[MAXN], edge2[MAXM], enext2[MAXM], ecnt2, n2, indeg[MAXN];
int ops[MAXN], chose[MAXN];

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

//拓扑选择
void topsel()
{
	queue<int> oplst;
	int i, j;

	for(i=0;i<n2;i++)
	{
		if(indeg[i] == 0)
			oplst.push(i);
	}
	while(!oplst.empty())
	{
		i = oplst.front();
		oplst.pop();
		if(chose[i] == 0)
		{
			chose[i] = 1;
			chose[ops[i]] = -1; //选择一个点，那么对立点及其子孙将不能选择
			for(j=ehead2[i];j>=0;j=enext2[j])
			{
				indeg[edge2[j]]--;
				if(indeg[edge2[j]] == 0)
				{
					oplst.push(edge2[j]);
				}
			}
		}
	}
}

//选择和判定
bool judge()
{
	int i, j;

	//Tarjan缩点
	n2 = 0;
	for(i=0;i<2*n;i++)
	{
		if(vis[i] == 0)
			Tarjan(i);
	}
	for(i=0;i<n;i++)
	{
		if(parset[i] == parset[i+n])  //同一组的不能在同一个强连通分量里
			return false;
		ops[parset[i]] = parset[i+n];
		ops[parset[i+n]] = parset[i];
	}
	//逆向构图
	memset(ehead2, -1, sizeof(ehead2));
	ecnt2 = 0;
	for(i=0;i<2*n;i++)
	{
		for(j=ehead[i];j>=0;j=enext[j])
		{
			if(parset[i] == parset[edge[j]])
				continue;

			addedge2(parset[edge[j]], parset[i]);
			indeg[parset[i]]++;
		}
	}
	//拓扑选择
	topsel();
	return true;
}