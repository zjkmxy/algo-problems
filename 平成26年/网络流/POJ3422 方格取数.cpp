#include <cstdio>
#include <algorithm>
using namespace std;

#define nullptr NULL
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
#define MAXN 60
int n, k;
int tab[MAXN][MAXN];

#define START(x, y) ((x)*n+(y)+1)
#define END(x, y) ((x)*n+(y)+1+n*n)

bool input(){
	int i, j;
	if(scanf("%d%d", &n, &k) == EOF)
		return false;
	init(n * n * 2 + 2);
	for(i = 0; i < n; i++)
	for(j = 0; j < n; j++)
		scanf("%d", &tab[i][j]);
	for(i = 0; i < n; i++){
		for(j = 0; j<n; j++){
			addedge(START(i, j), END(i, j), k, 0);
			if(tab[i][j] > 0)
				addedge(START(i, j), END(i, j), 1, -tab[i][j]);
			if(j < n - 1)
				addedge(END(i, j), START(i, j + 1), k, 0);
			if(i < n - 1)
				addedge(END(i, j), START(i + 1, j), k, 0);
		}
	}
	addedge(0, START(0, 0), k, 0);
	addedge(END(n - 1, n - 1), np - 1, k, 0);
	return true;
}

int main(){
	int ret;
	while(input()){
		MCMF(0, np - 1, ret);
		printf("%d\n", - ret);
	}
	return 0;
}