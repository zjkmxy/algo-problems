#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;
#pragma comment(linker, "/STACK:102400000,102400000")  

const int MAXN = 100005;
const int MAXV = 100;
const int INF = 0x3fffffff;

struct NODE;
struct EDGE;
struct NODE{
	int id;
	EDGE *head;
	NODE* par;
	int w;
	//f是已经破坏掉cur，且还能向上破坏至少j的情况
	//g是没有破坏cur，子树未破坏的仅限j步以内的情况
	int f[MAXV+1], g[MAXV+1];
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	EDGE* next;
}edge[MAXN<<1];
int N, ecnt;

void DFS(NODE* rt){
	int i;
	EDGE *j;
	for(i = 0; i <= rt->w; i ++){
		rt->f[i] = 1;
		rt->g[i] = 0;
	}
	for(; i <= MAXV; i ++){
		rt->f[i] = INF;
		rt->g[i] = 0;
	}
	for(j = rt->head; j; j = j->next){
		if(j->b == rt->par)
			continue;
		j->b->par = rt;
		DFS(j->b);
		rt->f[0] = min(rt->f[0] + j->b->f[0], rt->g[0] + j->b->f[1]);
		rt->g[0] = rt->g[0] + j->b->f[0];
		for(i = 1; i <= MAXV; i ++){
			rt->f[i] = rt->f[i] + j->b->g[i - 1];
			if(i < MAXV)
				rt->f[i] = min(rt->f[i], rt->g[i] +  + j->b->f[i + 1]);
			rt->g[i] = rt->g[i] + j->b->g[i - 1];
		}

		for(i = MAXV - 1; i >= 0; i --){
			rt->f[i] = min(rt->f[i], rt->f[i + 1]);
		}
		rt->g[0] = min(rt->g[0], rt->f[0]);
		for(i = 1; i <= MAXV; i ++){
			rt->g[i] = min(rt->g[i], rt->g[i - 1]);
		}
	}
}

void addedge(int a, int b){
	edge[ecnt].a = node + a;
	edge[ecnt].b = node + b;
	edge[ecnt].next = node[a].head;
	node[a].head = &edge[ecnt ++];
}

bool domain(){
	int i, a, b;
	if(scanf("%d", &N) == EOF)
		return false;
	for(i = 1; i <= N; i ++){
		node[i].id = i;
		scanf("%d", &node[i].w);
		node[i].head = NULL;
		node[i].par = NULL;
	}
	ecnt = 0;
	for(i = 1; i < N; i ++){
		scanf("%d%d", &a, &b);
		addedge(a, b);
		addedge(b, a);
	}
	DFS(node + 1);
	printf("%d\n", node[1].f[0]);
	return true;
}

int main(){
	while(domain());
	return 0;
}