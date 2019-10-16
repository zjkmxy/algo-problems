#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 10005
#define MAXM 50005

int n, m;
int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos = 0, tim = 0;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM];

bool input()
{
	int i, a, b;

	if(scanf("%d%d", &n, &m) == EOF)
		return false;
	if((n == 0) && (m == 0))
		return false;

	memset(ehead, -1, sizeof(ehead));
	for(i = 0; i < m; i++)
	{
		scanf("%d%d", &a, &b);
		edge[i] = b;
		enext[i] = ehead[a];
		ehead[a] = i;
	}

	return true;
}

void Tarjan(int cur)  //Tarjan查强连通分量
{
	int i, j;

	DFN[cur] = low[cur] = ++ tim;
	oplst[pos] = cur;
	pos++;
	for(i = ehead[cur]; i >= 0; i = enext[i])
	{
		j = edge[i];
		if(DFN[j] == 0)
		{
			Tarjan(j);
		}
		low[cur] = min(low[cur], low[j]);
	}
	if(DFN[cur] == low[cur])
	{
		do{
			pos--;
			j = oplst[pos];
			parset[j] = cur;
			setcnt[cur]++;
		} while(cur != j);
	}
}

int main()
{
	int i, j, ret;

	input();
	for(i = 1; i <= n; i++)
	{
		if(DFN[i] == 0)
			Tarjan(i);
	}
	for(i = 1; i <= n; i++)
	{
		for(j = ehead[i]; j >= 0; j = enext[j])
		{
			if(parset[i] != parset[edge[j]])
			{
				vis[parset[i]] = 1;
				break;
			}
		}
	}
	ret = 0;
	for(i = 1; i <= n; i++)
	{
		if(vis[i] == 0 && i == parset[i])
		{
			if(ret == 0)
			{
				ret = setcnt[i];
			} else
			{
				ret = -1;
				break;
			}
		}
	}
	if(ret > 0)
		printf("%d\n", ret);
	else
		printf("0\n");
	return 0;
}
