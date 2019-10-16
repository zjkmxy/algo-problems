#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<algorithm>
using namespace std;

#define MAXN 100005
#define MAXM 200005
#define MID(l, r)	(((l)+(r))>>1)
#define LCHD(x)		((x)<<1)
#define RCHD(x)		(((x)<<1)+1)
struct EDGE;
struct NODE{
	int id;
	EDGE *head;
	NODE *par, *top; //父节点，链头
	int siz, dep; //子树尺寸,深度
	int w;	  //映射位置
	int col;
}node[MAXN];
typedef void(*CBFUNC)(NODE *x, NODE *y, bool last);
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXN<<1];
int N, Q, ecnt;

//线段树
class TREE
{
private:
	struct STNODE
	{
		int l, r;
		int sum;
		bool rev;
	}tree[MAXN<<3];
public:
	void init()
	{
		int i;
		build(1, 1, N);
		ecnt = 0;
		for(i=0;i<=N;i++)
		{
			node[i].dep = node[i].w = node[i].siz = 0;
			node[i].par = node[i].top = nullptr;
			node[i].head = nullptr;
			node[i].id = i;
			node[i].col = 0;
		}
	}
	void build(int id, int l, int r)
	{
		int mid = MID(l, r);
		tree[id].sum = 0;
		tree[id].rev = false;
		tree[id].l = l;
		tree[id].r = r;
		if(l != r)
		{
			build(LCHD(id), l, mid);
			build(RCHD(id), mid+1, r);
		}
	}
	void pushdown(int id)
	{
		if(tree[id].rev)
		{
			tree[LCHD(id)].rev = !tree[LCHD(id)].rev;
			tree[LCHD(id)].sum = tree[LCHD(id)].r - tree[LCHD(id)].l - tree[LCHD(id)].sum + 1;
			tree[RCHD(id)].rev = !tree[RCHD(id)].rev;
			tree[RCHD(id)].sum = tree[RCHD(id)].r - tree[RCHD(id)].l - tree[RCHD(id)].sum + 1;
			tree[id].rev = false;
		}
	}
	inline void popup(int id)
	{
		tree[id].sum = tree[LCHD(id)].sum + tree[RCHD(id)].sum;
	}
	void reverse(int id, int l, int r)
	{
		int mid = MID(tree[id].l, tree[id].r);
		if(tree[id].l == l && tree[id].r == r)
		{
			tree[id].rev = !tree[id].rev;
			tree[id].sum = tree[id].r - tree[id].l - tree[id].sum + 1;
			return ;
		}
		pushdown(id);
		if(r <= mid)
			reverse(LCHD(id), l, r);
		else if(l > mid)
			reverse(RCHD(id), l, r);
		else{
			reverse(LCHD(id), l, mid);
			reverse(RCHD(id), mid+1, r);
		}
		popup(id);
	}
	int query(int id, int l, int r)
	{
		int mid = MID(tree[id].l, tree[id].r);
		if(l > tree[id].r || r < tree[id].l)
			id = id;
		if(tree[id].l == l && tree[id].r == r)
		{
			return tree[id].sum;
		}
		pushdown(id);
		if(r <= mid)
			return query(LCHD(id), l, r);
		else if(l > mid)
			return query(RCHD(id), l, r);
		else{
			return (query(LCHD(id), l, mid) + query(RCHD(id), mid+1, r));
		}
	}
} segtree, marktree;

void addedge(NODE *x, NODE *y)
{
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
//进行BFS剖分(返回树链大小)
int Split(NODE *root){
	int i, qt, qh, tz = 0;
	NODE *u, *top, *nxt;
	EDGE *j;
	static NODE* que[MAXN];
	qh = qt = 0;
	que[qt++] = root, root->par = nullptr, root->dep = 0;
	while (qh < qt){
		for(u = que[qh++], j = u->head; j; j = j->next){
			if(j->b != u->par){
				j->b->par = u;
				que[qt++] = j->b;
				j->b->dep = u->dep + 1;
			}
		}
	}
	for (i = N - 1; i >= 0; --i){
		u = que[i];
		u->siz = 1;
		for(j = u->head; j; j = j->next)
			if(j->b->par == u)
				u->siz += j->b->siz;
	}
	for (i = 0; i < N; ++i) {
		u = que[i];
		if(u->top)
			continue;
		top = u;
		while(u){
			u->top = top;
			u->w = ++tz;
			nxt = nullptr;
			for(j = u->head; j; j = j->next)
				if (j->b->par == u)
					if (!nxt || j->b->siz > nxt->siz)
						nxt = j->b;
			u = nxt;
		}
	}
	return tz;
}
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

//轻边由父亲节点在线段树中的值和子节点的值共同决定。
void revpath(NODE *x, NODE *y, bool last)
{
	if(y != x)
		segtree.reverse(1, y->w+1, x->w);
	if(!last)
		y->col = !y->col;
}
void revadj(NODE *x, NODE *y, bool last)
{
	marktree.reverse(1, y->w, x->w);
	if(last)
	{
		if(y->top == y)
			y->col = !y->col;
		else
			segtree.reverse(1, y->w, y->w);
	}else{
		y->col = !y->col;
	}
	if(x->w != N)
		segtree.reverse(1, x->w+1, x->w+1);
}

int ans;
void getsum(NODE *x, NODE *y, bool last)
{
	NODE *p;
	if(y != x)
		ans += segtree.query(1, y->w+1, x->w);
	if(!last)
	{
		p = y->par;
		ans += y->col ^ marktree.query(1, p->w, p->w);
	}
}

int main()
{
	int T, i, a, b, cas=0;
	scanf("%d",&T);
	while(T--)
	{
		scanf("%d",&N);
		segtree.init();
		marktree.init();
		for(i=0;i<N-1;i++)
		{
			scanf("%d%d",&a,&b);
			addedge(node + a, node + b);
		}
		Split(node + 1);
		scanf("%d",&Q);
		while(Q--)
		{
			scanf("%d%d%d",&i,&a,&b);
			if(i == 1)
				breakdown(node + a, node + b, revpath);
			else if(i == 2)
				breakdown(node + a, node + b, revadj);
			else{
				ans = 0;
				breakdown(node + a, node + b, getsum);
				printf("%d\n",ans);
			}
		}
	}
	return 0;
}
