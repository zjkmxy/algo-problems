#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 502
#define MAXM 40002

int n, m;
int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos, root, rtcnt;
bool cut;
int ehead[MAXN], edge[MAXM], enext[MAXM];

bool input()
{
	int i, a, b;

	if(scanf("%d%d",&n,&m)==EOF)
		return false;
	if((n==0)&&(m==0))
		return false;

	memset(ehead, -1, sizeof(ehead));
	m <<= 1;
	for(i=0;i<m;i+=2)
	{
		scanf("%d%d",&a,&b);
		edge[i] = b;
		edge[i+1] = a;
		enext[i] = ehead[a];
		enext[i+1] = ehead[b];
		ehead[a] = i;
		ehead[b] = i + 1;
	}

	return true;
}

void Tarjan(int cur, int dep, int par)  //Tarjan算法查割点
{
	int i, j;

	DFN[cur] = low[cur] = dep;
	//oplst[pos] = cur;
	//pos++;
	vis[cur] = 1;
	for(i=ehead[cur];i>=0;i=enext[i])
	{
		j = edge[i];
		if(vis[j] == 2)  //废除顶点
			continue;
		if(vis[j] == 0)
		{
			Tarjan(j, dep+1, cur);
			if(cur == root)
			{
				rtcnt++;
			}else{
				low[cur] = min(low[cur], low[j]);
				if(low[j] >= DFN[cur])
					cut = true;
			}
		}else if(j != par)
		{
			low[cur] = min(low[cur], DFN[j]);
		}
	}
}

bool judge()
{
	int i, j;

	for(i=0;i<n;i++)
	{
		memset(vis, 0, sizeof(vis));
		rtcnt = 0;
		root = (i==0)?1:0;
		vis[i] = 2;
		cut = false;
		Tarjan(root, 1, -1);
		if(cut)
			return false;
		if(rtcnt > 1)
			return false;
		for(j=0;j<n;j++)
		{
			if(vis[j]==0)
				return false;
		}
	}
	return true;
}

int main()
{
	while(input())
	{
		if(judge())
		{
			printf("YES\n");
		}else{
			printf("NO\n");
		}
	}

	return 0;
}