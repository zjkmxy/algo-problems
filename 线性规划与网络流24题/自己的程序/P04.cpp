#include <cstdio>
#include <cstring>
#include <vector>
#include <cctype>
#include <queue>
using namespace std;

//////////////////////////////////////////////////////////////////////////
const int maxn = 0x2fff;        //��ĸ���
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
//////////////////////////////////////////////////////////////////////////
const int MAXN = 4000;
int M;
bool vis[MAXN + 1];
int sqrn[60];
void DFS(int p){
	int next = -1;
	vis[p] = true;
	for(auto e : g[p]){
		if(e.cap == 0){
			next = e.to - MAXN;
			break;
		}
	}
	if(next > 0){
		printf(" %d", next);
		DFS(next);
	}
}

int main(){
	int N, M, flow = 0, i, k;
	const int ST = 0, ED = MAXN * 2 + 1;

	scanf("%d", &M);
	init(ED + 1);
	for(i = 1; i < 60; i ++){
		sqrn[i] = i * i;
	}
	for(k = 1; true; k ++){
		add_edge(ST, k, 1);
		add_edge(MAXN + k, ED, 1);
		for(i = 1; sqrn[i] <= k; i ++);
		for(; sqrn[i] < 2 * k; i ++)
			add_edge(sqrn[i] - k, MAXN + k, 1);
		flow += max_flow(ST, ED);
		if(k - flow > M)
			break;
	}
	N = k - 1;
	printf("%d\n", N);

	init(ED + 1);
	for(k = 1; k <= N; k ++){
		add_edge(ST, k, 1);
		add_edge(MAXN + k, ED, 1);
		for(i = 1; sqrn[i] <= k; i ++);
		for(; sqrn[i] < 2 * k; i ++)
			add_edge(sqrn[i] - k, MAXN + k, 1);
	}
	max_flow(ST, ED);

	for(auto e : g[0]){
		if(!vis[e.to]){
			printf("%d", e.to);
			DFS(e.to);
			printf("\n");
		}
	}
	return 0;
}
