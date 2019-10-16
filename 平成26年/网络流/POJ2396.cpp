#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

const int maxn = 0xfff;		//点的个数
const int inf = 0x3fffffff; 

struct edge
{
	int to,cap;
	int rev;
	edge(int to,int cap,int rev)
	{
		this->to = to;
		this->cap = cap;
		this->rev = rev;
	}
};

vector <edge> g[maxn];
int level[maxn];
int iter[maxn];

void add_edge(int from,int to,int cap)
{
	g[from].push_back(edge(to,cap,(int)g[to].size()));
	g[to].push_back(edge(from,0,(int)g[from].size()-1));
}

void mk_dist(int s)
{
	queue<int> que;
	int v, i;
	edge *e;

	memset(level, -1, sizeof(level));
	level[s] = 0;
	que.push(s);
	while(!que.empty())
	{
		v = que.front();
		que.pop();
		for(i = 0;i < g[v].size();i++)
		{
			e = &g[v][i];
			if(e->cap>0&&level[e->to]<0)
			{
				level[e->to]=level[v]+1;
				que.push(e->to);
			}
		}
	}
}

int augment(int v,int t,int f)
{
	int i, d, rest = f;
	edge *e;

	if(v==t)
		return f;
	for(i=iter[v];i<g[v].size();i++)
	{
		e = &g[v][i];
		if(e->cap > 0 && level[v] < level[e->to])
		{
			d = augment(e->to, t, min(rest, e->cap));
			if(d > 0)
			{
				e->cap -= d;
				g[e->to][e->rev].cap += d;
				rest -= d;
				iter[v] = i;
			}
			if(rest <= 0)
				break;
		}
	}
	iter[v] = i;
	return f - rest;
}

int max_flow(int s,int t)
{
	int flow = 0, f;
	while(true)
	{
		mk_dist(s);
		if(level[t]<0)
			return flow;
		memset(iter, 0, sizeof(iter));
		f = augment(s, t, inf);
		flow += f;
	}
}

void init(int n)
{
	int i;

	for(i=0;i<n;i++)
	{
		g[i].clear();
	}
}

//////////////////////////////

#define MAXM 205
#define MAXN 55
#define MAXP 400

int nmlow[MAXM][MAXN], nmhigh[MAXM][MAXN], nmmat[MAXM][MAXN];
int smrow[MAXM], smcol[MAXN];
bool vis1[MAXP], vis2[MAXP];
int n, m, qr, qc;

inline void deal(int a, int b, int c, char op)
{
	if(op != '<')
	{
		nmlow[a][b] = max(nmlow[a][b], op=='='?c:c+1);
	}
	if(op != '>')
	{
		nmhigh[a][b] = min(nmhigh[a][b], op=='='?c:c-1);
	}
}

bool input()
{
	int i, j, c;
	int a, b, d;
	char op[3];

	qr = qc = 0;
	scanf("%d%d",&m,&n);
	for(i=1;i<=m;i++)
	{
		scanf("%d",&smrow[i]);
		qr += smrow[i];
	}
	for(j=1;j<=n;j++)
	{
		scanf("%d",&smcol[j]);
		qc += smcol[j];
	}
	scanf("%d",&c);
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			nmlow[i][j] = 0;
			nmhigh[i][j] = inf;
		}
	}
	while(c--)
	{
		scanf("%d%d%s%d",&a,&b,&op,&d);
		if(a == 0 && b == 0)
		{
			for(i=1;i<=m;i++)
			{
				for(j=1;j<=n;j++)
				{
					deal(i, j, d, op[0]);
				}
			}
		}else if(a==0)
		{
			for(i=1;i<=m;i++)
			{
				deal(i, b, d, op[0]);
			}
		}else if(b==0)
		{
			for(j=1;j<=n;j++)
			{
				deal(a, j, d, op[0]);
			}
		}else{
			deal(a, b, d, op[0]);
		}
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(nmlow[i][j] > nmhigh[i][j])
				return false;
		}
	}
	return (qr==qc);
}

bool judge()
{
	int i, j;

	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			nmhigh[i][j] -= nmlow[i][j];
			smrow[i] -= nmlow[i][j];
			smcol[j] -= nmlow[i][j];
			qr -= nmlow[i][j];
			if(smrow[i]<0 || smcol[j]<0)
				return false;
		}
	}
	init(n+m+2);
	for(i=1;i<=m;i++)
	{
		add_edge(0, i, smrow[i]);
	}
	for(j=1;j<=n;j++)
	{
		add_edge(m+j, n+m+1, smcol[j]);
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(nmhigh[i][j] > 0)
				add_edge(i, m+j, nmhigh[i][j]);
		}
	}
	return max_flow(0, n+m+1)==qr;
}

bool DFS(int u, int p)
{
	int j, k;

	vis1[u] = vis2[u] = true;
	for(j=0;j<g[u].size();j++)
	{
		k = g[u][j].to;
		if(k==p)
			continue;
		if(vis1[k])
			return false;
		if(!DFS(k, u))
			return false;
	}
	return true;
}

bool uniq()
{
	int i;
	memset(vis2, 0, sizeof(vis2));
	memset(vis1, 0, sizeof(vis1));
	for(i=1;i<=m+n;i++)
	{
		if(vis2[i])
			continue;
		if(!DFS(i, -1))
			return false;
	}
	return true;
}

void print_mat()
{
	int i, j, p;

	memset(nmmat, 0, sizeof(nmmat));
	for(i=1;i<=m;i++)
	{
		for(j=0;j<g[i].size();j++)
		{
			p = g[i][j].to;
			if(p <= m || p >= m+n+1)
				continue;
			p -= m;
			nmmat[i][p] = nmhigh[i][p] - g[i][j].cap;
		}
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			printf(j==1?"%d":" %d",nmmat[i][j] + nmlow[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	int t, i;

	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		if(i > 0)
			printf("\n");
		if(!input() || !judge())
		{
			printf("IMPOSSIBLE\n");
			continue;
		}else
			print_mat();
	}
	return 0;
}
