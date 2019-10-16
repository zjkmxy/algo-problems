/*
给定一个图，问是否对所有点子集S和其补集T，满足S到T所有正向边的A和减去反向边的B和小于0
做法：
若存在一个可行流使得每条边的流量大于A而小于B，则显然对每一子集，其A值和小于B值和。
*/
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

const int maxn = 0xfff;        //点的个数
const int inf = 0x3fffffff;

struct edge{
	int to, cap, rev;
	bool ori;
	edge(int to, int cap, int rev, bool ori) :
		to(to), cap(cap), rev(rev), ori(ori){}
};

vector <edge> g[maxn];
int level[maxn], iter[maxn], np;

inline void add_edge(int from, int to, int cap){
	g[from].push_back(edge(to, cap, (int)g[to].size(), true));
	g[to].push_back(edge(from, 0, (int)g[from].size() - 1, false));
}

bool mk_dist(int s, int t){
	queue<int> que;
	int v, i, siz;
	edge *e;

	fill(level, level + np + 1, -1);
	level[s] = 0;
	que.push(s);
	while(!que.empty()){
		v = que.front();
		que.pop();
		for(i = 0, siz = g[v].size(); i < siz; i ++){
			e = &g[v][i];
			if(e->cap > 0 && level[e->to] < 0){
				level[e->to] = level[v] + 1;
				que.push(e->to);
			}
		}
	}
	return level[t] >= 0;
}

int augment(int v, int t, int f){
	int d, rest = f;
	edge *e;

	if(v == t)
		return f;
	for(int &i = iter[v], siz = g[v].size(); i < siz; i ++){
		e = &g[v][i];
		if(e->cap > 0 && level[v] < level[e->to]){
			d = augment(e->to, t, min(rest, e->cap));
			if(d > 0){
				e->cap -= d;
				g[e->to][e->rev].cap += d;
				rest -= d;
			}
			if(rest <= 0)
				break;
		}
	}
	return f - rest;
}

int max_flow(int s, int t){
	int flow = 0;
	while(mk_dist(s, t)){
		fill(iter, iter + np + 1, 0);
		flow += augment(s, t, inf);
	}
	return flow;
}

void init(int n){
	np = n;
	for(int i = 0; i < n; i++)
		g[i].clear();
}
//////////

int n, m, f;
bool domain()
{
	int i, u, v, d, b;

	scanf("%d%d", &n, &m);
	init(n + 2);
	f = 0;
	for(i = 0; i < m; i++)
	{
		scanf("%d%d%d%d", &u, &v, &d, &b);
		add_edge(0, v, d);
		add_edge(u, n + 1, d);
		add_edge(u, v, b);
		f += d;
	}
	if(max_flow(0, n + 1) == f)
		return true;
	else
		return false;
}

int main()
{
	int i, T;

	scanf("%d", &T);
	for(i = 1; i <= T; i++)
	{
		printf("Case #%d: ", i);
		if(domain())
			printf("happy\n");
		else
			printf("unhappy\n");
	}
	return 0;
}
