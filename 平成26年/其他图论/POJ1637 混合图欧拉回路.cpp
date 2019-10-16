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

///////////////////////

int indeg[maxn], outdeg[maxn], n, s;

bool domain()
{
	int i, a, b, d, fl = 0;

	scanf("%d%d",&n,&s);
	memset(indeg, 0, sizeof(indeg));
	memset(outdeg, 0, sizeof(outdeg));
	init(n+2);
	for(i=0;i<s;i++)
	{
		scanf("%d%d%d",&a,&b,&d);
		outdeg[a]++;
		indeg[b]++;
		if(d==0)
		{
			add_edge(a, b, 1);
		}
	}

	for(i=1;i<=n;i++)
	{
		if((indeg[i] - outdeg[i]) & 1)
			return false;
		if(indeg[i] - outdeg[i] == 0)
		{
			continue;
		}
		if(indeg[i] - outdeg[i] > 0)
		{
			add_edge(i, n+1, (indeg[i] - outdeg[i]) >> 1);
			fl += (indeg[i] - outdeg[i]) >> 1;
		}else{
			add_edge(0, i, (outdeg[i] - indeg[i]) >> 1);
			fl += (outdeg[i] - indeg[i]) >> 1;
		}
	}

	if(max_flow(0, n+1) == (fl >> 1))
	{
		return true;
	}else{
		return false;
	}
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
	{
		if(domain())
		{
			printf("possible\n");
		}else{
			printf("impossible\n");
		}
	}
	return 0;
}
