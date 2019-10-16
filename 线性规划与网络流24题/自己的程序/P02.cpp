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

int GetInt(){
	static bool bShut = false;
	if(bShut){
		bShut = false;
		return 0;
	}
	int ret;
	char ch = getchar();
	while(ch == ' ')
		ch = getchar();
	ret = ch - '0';
	for(ch = getchar(); isdigit(ch); ch = getchar())
		ret = ret * 10 + ch - '0';
	if(ch == '\n')
		bShut = true;
	return ret;
}

int main(){
	int N, M, ans, i, p;

	scanf("%d%d", &M, &N);
	init(M + N + 2);
	ans = 0;
	for(i = 1; i <= M; i ++){
		scanf("%d", &p);
		ans += p;
		add_edge(0, i, p);
		for(p = GetInt(); p > 0; p = GetInt()){
			add_edge(i, M + p, inf);
		}
	}
	for(i = 1; i <= N; i ++){
		scanf("%d", &p);
		add_edge(M + i, M + N + 1, p);
	}

	ans -= max_flow(0, M + N + 1);
	for(i = 1; i <= M; i ++){
		if(level[i] >= 0)
			printf("%d ", i);
	}
	putchar('\n');
	for(i = 1; i <= N; i ++){
		if(level[M + i] >= 0)
			printf("%d ", i);
	}
	putchar('\n');
	printf("%d\n", ans);

	return 0;
}