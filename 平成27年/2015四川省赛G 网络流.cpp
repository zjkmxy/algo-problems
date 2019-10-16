#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

//////////////////////////////////////////////////////////////////////////
namespace nwf{
	const int maxn = 0xfff;
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
		for(int &i = iter[v], siz = g[v].size(); i < siz; i++){
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
		for(int i = 1; i <= n; i++)
			g[i].clear();
	}
};
//////////////////////////////////////////////////////////////////////////
const int MAXN = 1005;
const int MAXM = 20005;
struct EDGE;
struct NODE{
	int id;
	int w, p;
	int col;
	EDGE *head;
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXM];
int N, M, ecnt;

void addedge(NODE *a, NODE *b){
	EDGE *j = &edge[ecnt ++];
	j->a = a;
	j->b = b;
	j->next = a->head;
	a->head = j;
}

void Color(NODE* x, int col){
	EDGE *j;

	x->col = col;
	for(j = x->head; j; j = j->next){
		if(j->b->col > 0)
			continue;
		Color(j->b, col ^ 1);
	}
}

bool domain(){
	int i, a, b, ret;

	//Input
	if(scanf("%d%d", &N, &M) == EOF)
		return false;
	ecnt = 0;
	for(i = 1; i <= N; i ++){
		node[i].id = i;
		node[i].head = NULL;
		node[i].col = -1;
	}
	for(i = 1; i <= N; i ++)
		scanf("%d", &node[i].w);
	for(i = 1; i <= N; i ++){
		scanf("%d", &node[i].p);
		node[i].p --;
	}
	while(M --){
		scanf("%d%d", &a, &b);
		addedge(node + a, node + b);
		addedge(node + b, node + a);
	}

	//Color
	for(i = 1; i <= N; i ++){
		if(node[i].col == -1)
			Color(node + i, 0);
	}

	//Build Map
	nwf::init(N * 2 + 2);
	for(i = 1; i <= N; i ++){
		if(node[i].p == 2){
			nwf::add_edge(1, i + 1, node[i].w);
			nwf::add_edge(N + i + 1, N * 2 + 2, node[i].w);
			nwf::add_edge(i + 1, N + i + 1, nwf::inf);
		} else if(node[i].col == node[i].p){
			nwf::add_edge(1, i + 1, node[i].w);
		} else{
			nwf::add_edge(N + i + 1, N * 2 + 2, node[i].w);
		}
	}
	for(i = 0; i < ecnt; i += 2){
		nwf::add_edge(edge[i].a->id + 1, edge[i].b->id + N + 1, nwf::inf);
		nwf::add_edge(edge[i].b->id + 1, edge[i].a->id + N + 1, nwf::inf);
	}

	//Calc
	ret = nwf::max_flow(1, N * 2 + 2);
	for(i = 1; i <= N; i ++){
		if(node[i].p == 2)
			ret -= node[i].w;
	}
	printf("%d\n", ret);

	return true;
}

int main(){
	while(domain());
	return 0;
}