#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
struct NODE;
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXM];
struct NODE{
	EDGE *head;
	NODE *par;
	int DFN, low;
	int bbc_lab;
	int bbc_lab2;
}node[MAXN];
int ecnt, bbc_cnt, bbc_cnt2, pos, pos2, degs[MAXN], dep;
NODE *oplst[MAXN], *oplst2[MAXN];
int n, m;

//这里1和2等效
void Tarjan(NODE *x){
	x->DFN = x->low = ++ dep;
	oplst[pos ++] = x;
	oplst2[pos2 ++] = x;
	for(EDGE *j = x->head; j; j = j->next){
		NODE *p = j->b;
		if(p == x->par)
			continue;
		if(!p->DFN){
			p->par = x;
			Tarjan(p);
		}
		x->low = min(x->low, p->low);	//这里是等效的，无向图没有横叉边
		//发现桥
		if(p->low > x->DFN){
			p->bbc_lab2 = ++ bbc_cnt2;
			for(pos2 --; oplst2[pos2] != p; pos2 --)
				oplst2[pos2]->bbc_lab2 = bbc_cnt2;
		}
	}
	//发现割点
	if(x->DFN == x->low){
		x->bbc_lab = ++ bbc_cnt;
		for(pos --; oplst[pos] != x; pos --)
			oplst[pos]->bbc_lab = bbc_cnt;
	}
}

void addedge(NODE *a, NODE *b){
	EDGE *j = &edge[ecnt ++];
	j->a = a;
	j->b = b;
	j->next = a->head;
	a->head = j;
}

bool input(){
	int i, a, b;

	scanf("%d%d",&n,&m);
	memset(degs, 0, sizeof(degs));
	for(i = 0; i <= n; i ++){
		node[i].par = NULL;
		node[i].bbc_lab = 0;
		node[i].DFN = 0;
		node[i].head = NULL;
	}
	for(i = 0; i < m; i ++){
		scanf("%d%d",&a,&b);
		addedge(node + a, node + b);
		addedge(node + b, node + a);
	}

	return true;
}

int domain(){
	int i, ret = 0;

	dep = 0;
	for(i = 1; i <= n; i ++)
		if(!node[i].DFN)
			Tarjan(node + i);

	for(i = 1; i <= n; i ++){
		for(EDGE *j = node[i].head; j; j = j->next){
			NODE* ed = j->b;
			if(node[i].bbc_lab2 != ed->bbc_lab2){
				degs[node[i].bbc_lab2] ++;
				degs[ed->bbc_lab2] ++;
			}
		}
	}

	for(i = 1; i <= bbc_cnt2; i ++){
		if(degs[i] / 2 == 1)
			ret++;
	}
	return (ret + 1) / 2;
}

int main(){
	input();
	printf("%d\n",domain());
	return 0;
}
