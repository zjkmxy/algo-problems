#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 2005
#define MAXM 6000005

int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos = 0, now = 0;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt;
int n2, n, m;

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

void input()
{
	int i, a, b, c;
	char op[10];

	scanf("%d%d",&n,&m);
	memset(ehead, -1, sizeof(ehead));
	ecnt = 0;
	for(i=0;i<m;i++)
	{
		scanf("%d %d %d %s",&a,&b,&c,op);
		switch(op[0])
		{
		case 'A':
			if(c == 1)
			{
				addedge(a, b);
				addedge(b, a);
				addedge(a+n, a);
				addedge(b+n, b);
			}else{
				addedge(a, b+n);
				addedge(b, a+n);
			}
			break;
		case 'O':
			if(c == 1)
			{
				addedge(a+n, b);
				addedge(b+n, a);
			}else{
				addedge(a+n, b+n);
				addedge(b+n, a+n);
				addedge(a, a+n);
				addedge(b, b+n);
			}
			break;
		case 'X':
			if(c == 1)
			{
				addedge(a, b+n);
				addedge(b, a+n);
				addedge(a+n, b);
				addedge(b+n, a);
			}else{
				addedge(a, b);
				addedge(b, a);
				addedge(a+n, b+n);
				addedge(b+n, a+n);
			}
			break;
		}
	}
}

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
	}
	return true;
}

int main()
{
	int i, a, b;

	input();
	if(!judge())
	{
		printf("NO\n");
	}else{
		printf("YES\n");
	}

	return 0;
}
