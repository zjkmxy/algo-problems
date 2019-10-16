#include <cstdio>
#include <cstring>
#include <vector>
#include <cctype>
#include <queue>
using namespace std;

//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////

const int MAXN = 1000;
int arr[MAXN], f[MAXN], dp[MAXN], N;
int main(){
	int i, j, ans = 0, flow = 0;

	//特判
	if(N == 1){
		printf("1\n1\n1\n");
		return 0;
	}

	//LIS
	scanf("%d", &N);
	fill(dp + 1, dp + N + 1, 0x3fffffff);
	for(i = 1; i <= N; i ++){
		scanf("%d", &arr[i]);
		f[i] = lower_bound(dp, dp + N, arr[i]) - dp;
		ans = max(ans, f[i]);
		dp[f[i]] = arr[i];
	}
	printf("%d\n", ans);

	//拆点独立构图，注意利用最大的性质分层搞
	init(N * 2 + 2);
	for(i = 1; i <= N; i ++){
		add_edge(i, N + i, 1);
		if(f[i] == 1)
			add_edge(0, i, 1);
		if(f[i] == ans)
			add_edge(N + i, N * 2 + 1, 1);
		for(j = i + 1; j <= N; j ++){
			if(arr[j] > arr[i] && f[j] == f[i] + 1){
				add_edge(N + i, j, 1);
			}
		}
	}
	flow = max_flow(0, N * 2 + 1);
	printf("%d\n", flow);

	//修正构图，继续计算
	g[0].front().cap = inf;	//S->1.a
	g[N * 2].back().cap = inf;	//N.b->T
	g[1].front().cap = inf;	//1.a->1.b
	g[N].back().cap = inf;	//N.a->N.b
	flow += max_flow(0, N * 2 + 1);
	printf("%d\n", flow);

	return 0;
}