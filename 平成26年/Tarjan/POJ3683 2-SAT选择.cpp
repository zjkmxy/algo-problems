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

int n;
struct CERE{
	int beg;
	int end;
	int las;
}cere[MAXN];

int chgtime(int hh, int mm)
{
	return (hh * 60 + mm);
}

bool paradox(int st1, int ed1, int st2, int ed2)
{
	return (st1 < ed2 && st2 < ed1);
}

void input()
{
	int i, a, b, c, d, e;

	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d:%d %d:%d %d", &a, &b, &c, &d, &e);
		cere[i].beg = chgtime(a, b);
		cere[i].end = chgtime(c, d);
		cere[i].las = e;
	}
	memset(ehead, -1, sizeof(ehead));
	ecnt = 0;
	for(a=0;a<n;a++)
	{
		for(b=0;b<n;b++)  //双向选择，但是addedge是单向的
				  //原因是：a与b矛盾，那么选择a一定不能选择b，相应的选择b一定不能选择a，但是不选择b不一定非要选择a
		{
			if(a == b)
				continue;
			//对每一组矛盾条件，原件与其对立面必定同时选择
			if(paradox(cere[a].beg, cere[a].beg+cere[a].las, cere[b].beg, cere[b].beg+cere[b].las))
			{
				addedge(a, b+n);
			}
			if(paradox(cere[a].beg, cere[a].beg+cere[a].las, cere[b].end-cere[b].las, cere[b].end))
			{
				addedge(a, b);
			}
			if(paradox(cere[a].end-cere[a].las, cere[a].end, cere[b].end-cere[b].las, cere[b].end))
			{
				addedge(a+n, b);
			}
			if(paradox(cere[a].end-cere[a].las, cere[a].end, cere[b].beg, cere[b].beg+cere[b].las))
			{
				addedge(a+n, b+n);
			}
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

int main()
{
	int i, a, b;

	input();
	if(!judge())
	{
		printf("NO\n");
	}else{
		printf("YES\n");
		for(i=0;i<n;i++)
		{
			if(chose[parset[i]]==1)
			{
				a = cere[i].beg;
				b = cere[i].beg + cere[i].las;
			}else{
				a = cere[i].end - cere[i].las;
				b = cere[i].end;
			}
			printf("%02d:%02d %02d:%02d\n",a/60,a%60,b/60,b%60);
		}
	}

	return 0;
}
