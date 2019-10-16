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
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt;
int n2, n, m;
int keys[MAXN], fk[MAXN][2];

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

bool judge(int flr)
{
	int i;

	ecnt = 0;
	for(i=0;i<n;i++)
	{
		ehead[i] = -1;
	}
	for(i=0;i<flr;i++)
	{
		addedge(keys[fk[i][0]], fk[i][1]);
		addedge(keys[fk[i][1]], fk[i][0]);
	}
	//Tarjan缩点
	n2 = 0;
	memset(vis, 0, sizeof(vis));
	now = 0;
	pos = 0;
	for(i=0;i<n;i++)
	{
		if(vis[i] == 0)
			Tarjan(i);
	}
	for(i=0;i<n;i++)
	{
		if(parset[i] == parset[keys[i]])  //同一组的不能在同一个强连通分量里
			return false;
	}
	return true;
}


int BiSearch()
{
	int begin = 0, end = m, mid;

	while(end > begin + 1)
	{
		mid = (begin + end) / 2;
		if(judge(mid))
		{
			begin = mid;
		}else{
			end = mid;
		}
	}

	if(judge(end))
	{
		return end;
	}else{
		return begin;
	}
}

int input()
{
	int i, a, b;
	scanf("%d%d",&n,&m);
	if(n == 0)
		return false;
	for(i=0;i<n;i++)
	{
		scanf("%d%d",&a,&b);
		keys[a] = b;
		keys[b] = a;
	}
	n <<= 1;
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&fk[i][0],&fk[i][1]);
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
