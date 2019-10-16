#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
#pragma comment(linker, "/STACK:25600000,25600000")

const int MAXN = 100005;
struct EDGE;
struct NODE{
	int id;
	EDGE *head;
	NODE *par, *top; //���ڵ㣬��ͷ
	int siz, dep; //�����ߴ�,���
	int w;	  //ӳ��λ��
	int plast;  //������ĩβ
	NODE *son;  //�ض���
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	EDGE* next;
}edge[MAXN];
NODE* map[MAXN];
int N, ecnt;

namespace BTS{
	void DFS(NODE *x){
		x->siz = 1;
		x->son = nullptr;
		for(EDGE *j = x->head; j; j = j->next){
			NODE *u = j->b;
			if(u == x->par)
				continue;
			u->par = x;
			u->dep = x->dep + 1;
			DFS(u);
			if(!x->son || u->siz > x->son->siz)
				x->son = u;
			x->siz += u->siz;
		}
	}
	int tz; NODE *tp;
	void DFS2(NODE *x){
		x->w = ++tz;    //������
		map[x->w] = x;
		x->top = tp;
		if(x->son) DFS2(x->son);
		for(EDGE *j = x->head; j; j = j->next){
			if(j->b == x->son || j->b == x->par)
				continue;
			tp = j->b;
			DFS2(j->b);
		}
		x->plast = tz;
	}
	void Split(NODE *root){
		tz = 0, tp = root;
		DFS(root);
		DFS2(root);
	}
};

class Segtree{
public:
	struct SN{
		SN *l, *r;
		int L, R;
		int add[2];	//����ӷ����
		int mx[2];	//�������ֵ
		bool rev;	//��ɫ��ת���
		int sum;	//��������

		inline void doadd(int d, int v){
			add[d] += v;
			if(mx[d] != -1)
				mx[d] += v;
		}
		inline void dorev(){
			rev = !rev;
			sum = (R - L + 1) - sum;
		}
		inline void popup(){
			mx[0] = max(l->mx[0] , r->mx[0]);
			mx[1] = max(l->mx[1] , r->mx[1]);
			sum = l->sum + r->sum;
		}
		inline void pushdown(){
			if(add[0] != 0){
				l->doadd(0, add[0]);
				r->doadd(0, add[0]);
				add[0] = 0;
			}
			if(add[1] != 0){
				l->doadd(1, add[1]);
				r->doadd(1, add[1]);
				add[1] = 0;
			}
			if(rev){
				l->dorev();
				r->dorev();
				rev = false;
			}
		}
	}sn[MAXN<<1];
	int ncnt;

	//����
	SN* build(int L, int R){
		SN* ret = &sn[ncnt ++];
		ret->add[0] = ret->add[1] = ret->mx[0] = ret->mx[1] = 0;
		ret->rev = false;
		ret->sum = 0;
		ret->L = L;
		ret->R = R;
		if(L == R){
			int p = map[L]->dep & 1;
			ret->mx[p ^ 1] = -1;
			ret->l = ret->r = NULL;
		}else{
			int mid = (L + R) >> 1;
			ret->l = build(L, mid);
			ret->r = build(mid + 1, R);
			ret->popup();
		}
		return ret;
	}
	//��ѯ��С��ת��
	int findir(SN* rt, int l, int r, int d){
		if(abs(rt->mx[d]) == 1 && rt->mx[d^1] <= 0)
			return l;
		if(rt->L == rt->R)
			return 0;
		rt->pushdown();
		int mid = (rt->L + rt->R) >> 1;
		int ansr = 0, ansl = 0;
		if(r > mid)
			ansr = findir(rt->r, max(l, mid + 1), r, d);
		if(ansr == mid + 1 || r <= mid)
			ansl = findir(rt->l, l, min(r, mid), d);
		return ansl ? ansl : ansr;
	}
	//��ת
	void revers(SN* rt, int l, int r){
		if(l == rt->L && rt->R == r){
			rt->dorev();
			return ;
		}
		int mid = (rt->L + rt->R) >> 1;
		rt->pushdown();
		if(l > mid){
			revers(rt->r, l, r);
		}else if(r <= mid){
			revers(rt->l, l, r);
		}else{
			revers(rt->l, l, mid);
			revers(rt->r, mid + 1, r);
		}
		rt->popup();
	}
	//�Ӽ���
	void add(SN* rt, int l, int r, int v, int d){
		if(l == rt->L && rt->R == r){
			rt->doadd(d, v);
			return ;
		}
		int mid = (rt->L + rt->R) >> 1;
		rt->pushdown();
		if(l > mid){
			add(rt->r, l, r, v, d);
		}else if(r <= mid){
			add(rt->l, l, r, v, d);
		}else{
			add(rt->l, l, mid, v, d);
			add(rt->r, mid + 1, r, v, d);
		}
		rt->popup();
	}
public:
	inline void Build(int N){
		ncnt = 0;
		build(1, N);
	}
	inline void Reverse(int l, int r){
		revers(sn, l, r);
	}
	inline void Add(int l, int r, int v, int d){
		add(sn, l, r, v, d);
	}
	inline int Find(int l, int r, int d){
		return findir(sn, l, r, d);
	}
	inline int GetSum(){
		return sn->sum;
	}
}segt;

//����Ҷ�ӽڵ�x������x�Ǻڣ��ոշ��ɰ�
void Change(NODE* x){
	int d = x->dep & 1;
	bool flag = false;
	int p;

	x = x->par;
	while(x && !flag){
		p = segt.Find(x->top->w, x->w, d);
		//��ת�Ǿ͵ط�ת
		if(p)
			segt.Reverse(p, x->w);
		//�Ӽ�1Ҫ�鸸��
		if(p == 0){
			p = x->w;
			flag = true;
		}else if(p != x->top->w){
			p --;
			flag = true;
		}
		segt.Add(p, x->w, 1, d ^ 1);
		segt.Add(p, x->w, -1, d);
		x = map[p]->top->par;
	}
}

void addedge(NODE* a, NODE* b){
	EDGE *cur = &edge[ecnt ++];
	cur->a = a;
	cur->b = b;
	cur->next = a->head;
	a->head = cur;
}

//��ʼ���ڵ�
void InitNode(int id){
    node[id].id = id;
    node[id].par = node[id].son = node[id].top = nullptr;
    node[id].head = nullptr;
	node[id].dep = 0;
}

bool domain(){
	int i, a;
	ecnt = 0;
	if(scanf("%d", &N) == EOF)
		return false;
	for(i = 0; i < N; i ++)
		InitNode(i);
	for(i = 1; i < N; i ++){
		scanf("%d", &a);
		addedge(node + a, node + i);
	}
	BTS::Split(node);
	segt.Build(BTS::tz);
	for(i = 1; i < N; i ++){
		Change(node + i);
		printf("%d\n", i - segt.GetSum() + 1);
	}
	return true;
}

int main(){
	while(domain()) fflush(stdout);
	return 0l;
}