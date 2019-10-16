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

int add_edge(int from,int to,int cap)
{
	g[from].push_back(edge(to,cap,(int)g[to].size()));
	g[to].push_back(edge(from,0,(int)g[from].size()-1));
	return (g[to].size() - 1);
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

int mat[maxn][maxn];
int vtolst[maxn];
int n, s, t;

void input()
{
	int i, j;

	scanf("%d%d%d",&n,&s,&t);
	s--;
	t--;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			scanf("%d",&mat[i][j]);
		}
	}
}

int get_flow(int cur)
{
	int i, j;

	init(n * 2);
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(i == cur || j == cur)
				continue;
			if(!mat[i][j])
				continue;
			if(i == j)
				vtolst[i] = add_edge(i, i+n, 1);
			else
				add_edge(i+n, j, maxn);
		}
	}

	return max_flow(s+n, t);
}

bool vis[maxn];
bool judge(int cur)
{
	queue<int> oplst;
	int c, j;
	edge *e;

	memset(vis, 0, sizeof(vis));
	oplst.push(cur);
	vis[cur] = true;
	while(!oplst.empty())
	{
		c = oplst.front();
		oplst.pop();
		for(j=0;j<g[c].size();j++)
		{
			e = &g[c][j];
			if(e->cap <= 0 || vis[e->to])
				continue;
			vis[e->to] = true;
			oplst.push(e->to);
			if(e->to == cur + n)
				return true;
		}
	}
	return false;
}

bool reduce(int sp, int tp, int f)
{
	int j;
	edge *e;
	if(sp == tp)
		return true;
	vis[sp] = true;
	for(j=0;j<g[sp].size();j++)
	{
		e = &g[sp][j];
		if(e->cap > 0 && !vis[e->to] && reduce(e->to, tp, f))
		{
			e->cap -= f;
			g[e->to][e->rev].cap += f;
			return true;
		}
	}
	return false;
}

int main()
{
	int flow, cur, i, j;
	bool flag = false;

	input();
	if(mat[s][t])
	{
		printf("NO ANSWER!\n");
		return 0;
	}
	flow = get_flow(maxn);
	printf("%d\n",flow);
	//枚举割点
	for(i=0;i<n;i++)
	{
		if(i==s || i==t)
			continue;
		/*
		//每次做最大流，虽然能得出正解但是代价太高，不建议
		cur = get_flow(i);
		if(cur < flow)
		{
			flow = cur;
			printf(flag?" %d":"%d",i+1);
			flag = true;
			for(j=0;j<n;j++)
				mat[i][j] = mat[j][i] = 0;
		}
		*/
		//这里的做法是：如果残余网络中不存在i->i'的路径，那么i是割点，退掉s->i->i'->t的流量，删边
		if(!judge(i))
		{
			flow--;
			printf(flag?" %d":"%d",i+1);
			flag = true;
			memset(vis, 0, sizeof(vis));
			reduce(t, i+n, 1);
			memset(vis, 0, sizeof(vis));
			reduce(i, s+n, 1);
			j = vtolst[i];
			g[i][j].cap = 0;
			g[i+n][g[i][j].rev].cap = 0;
		}
		if(flow <= 0)
			break;
	}
	printf("\n");

	return 0;
}

