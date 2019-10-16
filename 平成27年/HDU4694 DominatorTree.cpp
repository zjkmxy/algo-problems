#include<cstdio>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;

const int MAXN = 200005;
struct EDGE;
struct NODE{
	int id;
	NODE *par;	//���׽ڵ�
	int dfn;	//ʱ����
	NODE *semi, *idom;	//��ؾ��㣬����ؾ���
	NODE *anc;	//���鼯�ĸ�
	NODE *best;	//���鼯����Сʱ���
	EDGE *head, *pre;
	vector<NODE*> dom;	//����ֱ��֧��Ľڵ�
}node[MAXN];
NODE *vid[MAXN];	//dfn��Ӧ�Ķ���
struct EDGE{
	NODE *a, *b;
	EDGE *nxt, *pnxt;
}edge[MAXN];
int timev, ecnt;

//��ʼ��DFN
void DFS(NODE* cur){
	EDGE *j;

	cur->dfn = ++ timev;
	vid[timev] = cur;
	for(j = cur->head; j; j = j->nxt){
		if(j->b->dfn == 0){
			DFS(j->b);
			j->b->par = cur;
		}
	}
}
//��ò��鼯�ĸ�
NODE* get(NODE* x){
	if(x == x->anc)
		return x;
	NODE* y = get(x->anc);
	if(x->anc->best->semi->dfn < x->best->semi->dfn)
		x->best = x->anc->best;
	x->anc = y;
	return y;
}
//����idom
void Tarjan(){
	int tm;
	NODE *y, *x;
	EDGE *j;

	for(tm = timev; tm > 1; tm --){
		y = vid[tm];
		x = y->par;
		//�����ؾ���
		for(j = y->pre; j; j = j->pnxt){
			NODE *z = j->a;
			if(z->dfn == 0)
				continue;
			get(z);
			if(z->best->semi->dfn < y->semi->dfn)
				y->semi = z->best->semi;
		}
		y->semi->dom.push_back(y);
		y->anc = x;
		for(NODE *z : x->dom){
			get(z);
			if(z->best->semi->dfn < x->dfn)
				z->idom = z->best;
			else
				z->idom = x;
		}
		x->dom.clear();
	}
	for(tm = 2; tm <= timev; tm ++){
		x = vid[tm];
		if(x->idom != x->semi)
			x->idom = x->idom->idom;
		x->idom->dom.push_back(x);
	}
}

void Init(int N){
	ecnt = timev = 0;
	for(int i = 1; i <= N; i ++){
		node[i].id = i;
		node[i].par = node[i].idom = nullptr;
		node[i].semi = node[i].best = node[i].anc = &node[i];
		node[i].dfn = 0;
		node[i].dom.clear();
		node[i].head = node[i].pre = nullptr;
	}
}

void addedge(NODE *a, NODE *b){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].nxt = a->head;
	edge[ecnt].pnxt = b->pre;
	a->head = &edge[ecnt];
	b->pre = &edge[ecnt];
	ecnt ++;
}

long long ans[50010];

void get_ans(NODE* x){  
	ans[x->id] += x->id;
	int sz = x->dom.size();  
	for(int i = 0; i < sz; ++i)  
	{  
		ans[x->dom[i]->id] += ans[x->id];  
		get_ans(x->dom[i]);  
	}  
}  

int N, M;
int main(){
	int a, b;
	while(scanf("%d%d", &N, &M) != EOF){
		Init(N);
		while(M --){
			scanf("%d%d", &a, &b);
			addedge(node + a, node + b);
		}
		DFS(node + N);
		Tarjan();
		memset(ans, 0, sizeof(ans));
		get_ans(&node[N]);
		for(int i = 1; i <= N; ++i)  
			printf("%I64d%c", ans[i], i == N ? '\n' :' ');  
	}
	return 0;
}