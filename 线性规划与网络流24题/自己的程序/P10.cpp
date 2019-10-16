#include <cstdio>
#include <cstring>
#include <vector>
#include <cctype>
#include <queue>
using namespace std;

//////////////////////////////////////////////////////////////////////////
const int MAXN = 10005;
const int MAXM = 100005;
const int INF = 0x3fffffff;
struct EDGE;
struct NODE{
	EDGE *head, *prev;
	int cost, cap;
	bool inq;
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	int cap, cost;
	EDGE *next, *rev;
}edge[MAXM * 2];
int np, ecnt;

int SPFA(NODE* s, NODE* t){
	int ret, qh = 0, qt = 1;
	static NODE* q[MAXN];
	NODE* cur;
	EDGE *j;

	for(ret = 0; ret < np; ret ++)
		node[ret].cost = INF;
	s->cost = 0, s->cap = INF, s->inq = true, s->prev = nullptr;
	q[0] = s;
	while(qh != qt){
		cur = q[qh ++];
		if(qh >= MAXN) qh -= MAXN;
		cur->inq = false;
		for(j = cur->head; j; j = j->next){
			if(j->cap == 0 || j->b->cost <= cur->cost + j->cost)
				continue;
			j->b->cost = cur->cost + j->cost;
			j->b->prev = j;
			j->b->cap = min(cur->cap, j->cap);
			if(!j->b->inq){
				j->b->inq = true;
				q[qt ++] = j->b;
				if(qt >= MAXN) qt -= MAXN;
			}
		}
	}
	if(t->cost >= INF) return 0;
	ret = t->cap;
	for(j = t->prev; j; j = j->a->prev){
		j->cap -= ret;
		j->rev->cap += ret;
	}
	return ret;
}

void addedge(int a, int b, int cap, int cost){
	EDGE *j = &edge[ecnt ++];
	EDGE *k = &edge[ecnt ++];
	j->a = node + a, j->b = node + b;
	k->a = node + b, k->b = node + a;
	j->cap = cap, j->cost = cost;
	k->cap = 0, k->cost = - cost;
	j->rev = k, k->rev = j;
	j->next = node[a].head, node[a].head = j;
	k->next = node[b].head, node[b].head = k;
}

void init(int N){
	ecnt = 0;
	for(np = 0; np < N; np ++)
		node[np].head = nullptr;
}

int MCMF(int s, int t, int& cost){
	int f, flow = cost = 0;
	do{
		f = SPFA(node + s, node + t);
		flow += f, cost += f * node[t].cost;
	} while(f > 0);
	return flow;
}
//////////////////////////////////////////////////////////////////////////
int main(){
	int N, p, m, f, n, s;
	int i, r, cost;

	scanf("%d%d%d%d%d%d", &N, &p, &m, &f, &n, &s);
	init(2 * N + 2);
	for(i = 1; i <= N; i ++){
		scanf("%d", &r);
		//每天用过的
		addedge(0, i, r, 0);
		//新购入
		addedge(0, N + i, r, p);
		//每日需求
		addedge(N + i, N * 2 + 1, r, 0);
		//快洗
		if(i + m <= N)
			addedge(i, N + i + m, r, f);
		//慢洗
		if(i + n <= N)
			addedge(i, N + i + n, r, s);
		//让渡
		if(i + 1 <= N)
			addedge(N + i, N + i + 1, INF, 0);
	}
	MCMF(0, N * 2 + 1, cost);
	printf("%d\n", cost);
	return 0;
}