/*
没有特别的技巧，暴搜即可。因为两种路径必须交错，减少了搜索的范围。
注意的是，使用路径的状态不同，到达同一点的情况也不同。DFS的时候不能统一记录距离。
*/
#include <cstdio>
#include <queue>
#include <cstdlib>
#include <algorithm>

#ifdef _DEBUG
#define BREAK __asm{INT 3}
#else
#define BREAK
#endif

typedef long long LL;
typedef unsigned long DWORD;
const int MAXN = 1005;
const int MAXM = 100005;
const int INF = 0x3fffffff;

struct NODE;
struct EDGE;
typedef NODE *PNODE;
typedef EDGE *PEDGE;
struct NODE{
	PEDGE head;
	int rdis[2];
	bool vis;
	int maxdis[2];
	int vis2[2];
}node[MAXN];
struct EDGE{
	PNODE a, b;
	PEDGE next;
	int dis;
	int type;
}edge[MAXM<<2];
int n, m, s, t;

void addedge(PNODE a, PNODE b, int dis, int type){
	edge[m].next = a->head;
	a->head = &edge[m];
	edge[m].a = a;
	edge[m].b = b;
	edge[m].dis = dis;
	edge[m].type = type;
	++ m;

	edge[m].next = b->head;
	b->head = &edge[m];
	edge[m].a = b;
	edge[m].b = a;
	edge[m].dis = dis;
	edge[m].type = type;
	++ m;
}

void input(){
	int p, a, b, d;
	scanf("%d%d%d", &n, &s, &t);
	m = 0;
	for(p = 0; p <= n; p ++){
		node[p].rdis[0] = node[p].rdis[1] = INF;
		node[p].maxdis[0] = node[p].maxdis[1] = 0;
		node[p].vis = false;
	}
	scanf("%d", &p);
	while(p --){
		scanf("%d%d%d", &a, &b, &d);
		addedge(node + a, node + b, d, 0);
	}
	scanf("%d", &p);
	while(p --){
		scanf("%d%d%d", &a, &b, &d);
		addedge(node + a, node + b, d, 1);
	}
}

void BFS(int type){
	PNODE cur = node + t;
	PEDGE e;
	std::queue<PNODE> *oplst = new std::queue<PNODE>();

	oplst->push(cur);
	cur->rdis[type] = 0;
	cur->vis = true;
	while(!oplst->empty()){
		cur = oplst->front();
		oplst->pop();
		cur->vis = false;
		for(e = cur->head; e != nullptr; e = e->next){
			if(e->type == type && e->b->rdis[type] > e->a->rdis[type] + e->dis){
				e->b->rdis[type] = e->a->rdis[type] + e->dis;
				if(!e->b->vis){
					e->b->vis = true;
					oplst->push(e->b);
				}
			}
		}
	}

	delete oplst;
}

int DFS(PNODE cur, int type){
	PEDGE e;
	int d;
	if(cur->vis2[type] == 1){
		return -1;
	}else if(cur->vis2[type] != 2){
		cur->vis2[type] = 1;
		for(e = cur->head; e != nullptr; e = e->next){
			if(e->type == type && e->b->rdis[type] < cur->rdis[type]){
				d = DFS(e->b, type ^ 1);
				if(d == -1){
					return -1;
				}else{
					cur->maxdis[type] = std::max(cur->maxdis[type], d + e->dis);
				}
			}
		}
		cur->vis2[type] = 2;
	}
	return cur->maxdis[type];
}

int main(){
#ifndef _DEBUG
	freopen("journey.in", "r", stdin);
	freopen("journey.out", "w", stdout);
#endif
	////////
	input();
	BFS(0);
	BFS(1);
	printf("%d\n", DFS(node + s, 0));
	////////
	return 0;
}
