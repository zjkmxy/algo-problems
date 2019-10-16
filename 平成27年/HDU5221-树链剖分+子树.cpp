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

#define nullptr NULL
const int MAXN = 100005;
//////START
struct EDGE;
struct NODE{
	int id;
	EDGE *head;
	NODE *par, *top; //父节点，链头
	int siz, dep; //子树尺寸,深度
	int w;	  //映射位置
	//限DFS
	int plast;  //子树的末尾
	NODE *son;  //重儿子
	//
	int val;
}node[MAXN];
typedef void(*CBFUNC)(NODE *x, NODE *y, bool last);
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXN<<1];
int N, Q, ecnt;
int varr[MAXN];
//子树版：可以利用子树的整体操作
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
		x->w = ++tz;    //分配编号
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
//剖分路径
void breakdown(NODE *va, NODE *vb, CBFUNC cbf){
	NODE *f1 = va->top, *f2 = vb->top;
	while(f1 != f2){
		if(f1->dep < f2->dep){
			swap(f1, f2);
			swap(va, vb);
		}
		cbf(va, f1, false);
		va = f1->par;
		f1 = va->top;
	}
	if(va->dep > vb->dep)
		swap(va, vb);
	cbf(vb, va, true);
}
//////END
class TREE{
private:
	struct STNODE{
		int l, r;
		STNODE *lchd, *rchd;
		int sum, val;
		bool set;
	}node[MAXN<<2];
	int cnt;
	STNODE *root;
	inline void popup(STNODE *cur){
		cur->val = cur->lchd->val + cur->rchd->val;
	}
	inline void pushdown(STNODE *cur){
		if(cur->set){
			cur->lchd->set = true;
			cur->lchd->val = cur->lchd->sum;
			cur->rchd->set = true;
			cur->rchd->val = cur->rchd->sum;
			cur->set = false;
		}
	}
	STNODE* build(int l, int r){
		STNODE *ret = node + cnt;
		cnt ++;
		ret->l = l;
		ret->r = r;
		ret->set = false;
		if(l != r){
			int mid = (l + r) >> 1;
			ret->lchd = build(l, mid);
			ret->rchd = build(mid + 1, r);
			ret->sum = ret->lchd->sum + ret->rchd->sum;
			popup(ret);
		}else{
			ret->lchd = ret->rchd = nullptr;
			ret->val = 0;
			ret->sum = varr[l];
		}
		return ret;
	}
	void set(STNODE* cur, int l, int r){
		if(cur->l == l && cur->r == r){
			cur->val = cur->sum;
			cur->set = true;
		}else{
			int mid = (cur->l + cur->r) >> 1;
			pushdown(cur);
			if(r <= mid){
				set(cur->lchd, l, r);
			}else if(l > mid){
				set(cur->rchd, l, r);
			}else{
				set(cur->lchd, l, mid);
				set(cur->rchd, mid + 1, r);
			}
			popup(cur);
		}
	}
	void reset(STNODE* cur, int pos){
		if(cur->l == cur->r){
			cur->val = 0;
			cur->set = false;
		}else{
			int mid = (cur->l + cur->r) >> 1;
			pushdown(cur);
			if(pos <= mid){
				reset(cur->lchd, pos);
			}else{
				reset(cur->rchd, pos);
			}
			popup(cur);
		}
	}
public:
	inline void Init(){
		cnt = 0;
		root = build(1, N);
	}
	inline void Set(int l, int r){
		if(l > r)
			swap(l, r);
		set(root, l, r);
	}
	inline void Reset(int pos){
		reset(root, pos);
	}
	inline int Query(){
		return root->val;
	}
}tree;

void init(){
	ecnt = 0;
	for(int i = 0; i <= N; i ++){
		node[i].dep = node[i].siz = node[i].w = 0;
		node[i].par = node[i].top = nullptr;
		node[i].head = nullptr;
		node[i].id = i;
	}
}
void addedge(NODE *x, NODE *y){
	edge[ecnt].a = x;
	edge[ecnt].b = y;
	edge[ecnt].next = x->head;
	x->head = edge + ecnt;
	ecnt++;
	edge[ecnt].a = y;
	edge[ecnt].b = x;
	edge[ecnt].next = y->head;
	y->head = edge + ecnt;
	ecnt++;
}
void doset(NODE *x, NODE *y, bool last){
	tree.Set(x->w, y->w);
}
void domain(){
	int i, a, b, c;
	scanf("%d", &N);
	init();
	for(i = 1; i <= N; i ++){
		scanf("%d", &node[i].val);
	}
	for(i = 1; i < N; i ++){
		scanf("%d%d", &a, &b);
		addedge(node + a, node + b);
	}
	BTS::Split(node + 1);
	for(i = 1; i <= N; i ++){
		varr[node[i].w] = node[i].val;
	}
	tree.Init();
	scanf("%d", &Q);
	while(Q--){
		scanf("%d%d", &a, &b);
		if(a == 1){
			scanf("%d", &c);
			breakdown(node + b, node + c, doset);
		}else if(a == 2){
			tree.Reset(node[b].w);
		}else{
			tree.Set(node[b].w, node[b].plast);
		}
		printf("%d\n", tree.Query());
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