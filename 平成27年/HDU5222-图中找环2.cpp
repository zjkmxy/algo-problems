//单双向边混合图中找环，用并查集先对双向边缩点再处理
#include <cstdio>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <string>
#include <map>
using namespace std;
#pragma comment(linker, "/STACK:102400000,102400000")

const int MAXN = 1000005;
#define nullptr NULL

struct EDGE;
struct NODE;
typedef struct NODE{
	int id;
	int vis;
	EDGE *head;
}*PNODE;  //双向边缩点的结果
int parset[MAXN];
typedef struct EDGE{
	PNODE a, b;
	EDGE *next;
}*PEDGE;
NODE node[MAXN];
EDGE edge[MAXN<<1];
int N, M1, M2, ecnt;
bool answer;

int getpar(int id){
	if(parset[id] != id){
		parset[id] = getpar(parset[id]);
	}
	return parset[id];
}

bool merge(int a, int b){
	a = getpar(a);
	b = getpar(b);
	if(a != b){
		parset[a] = b;
		return true;
	}else{
		return false;
	}
}

void addedge(PNODE a, PNODE b){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = a->head;
	a->head = edge + ecnt;
	ecnt ++;
}

void dfs(PNODE st)
{
	PEDGE j;
	PNODE p;
	st->vis = -1;
	for(j = st->head; j; j = j ->next)
	{
		p = j->b;
		if(p->vis == 0)
		{
			dfs(p);
			p->vis = 1;
		}
		if(p->vis == -1)
		{
			answer = true;
			return;
		}
	}
	st->vis = 1;
}

void domain(){
	int i, a, b;

	answer = false;
	ecnt = 0;
	scanf("%d%d%d", &N, &M1, &M2);
	for(i = 0; i <= N; i ++){
		parset[i] = i;
		node[i].id = i;  //调试用识别代号
		node[i].vis = 0;
		node[i].head = nullptr;
	}
	for(i = 0; i < M1; i ++){
		scanf("%d%d", &a, &b);
		if(!merge(a, b))
			answer = true;
	}
	for(i = 0; i < M2; i ++){
		scanf("%d%d", &a, &b);
		addedge(node + getpar(a), node + getpar(b));
	}
	for(i = 1; i <= N; i ++){
		if(node[i].vis == 0)
			dfs(node + i);
	}
	if(answer){
		printf("YES\n");
	}else{
		printf("NO\n");
	}
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		domain();
	}
	return 0;
}

