//可以用LCA+树状数组搞，比HLD会快一些
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;
#pragma comment(linker, "/STACK:1024000000,1024000000") 

typedef long long LL;
const int MAXN = 100005;
const int INF = 0x3fffffff;

inline int log2(int x){
	int ret = 0;
	for(; x > 1; x >>= 1, ++ ret);
	return ret;
}

struct EDGE;
struct CHAIN;
struct NODE{
	int id;
	EDGE *head;
	NODE *par, *top; //父节点，链头
	int siz, dep; //子树尺寸,深度
	int w;	  //映射位置
	int plast;  //子树的末尾
	NODE *son;  //重儿子

	int pfst;

	int sum, dp;	//sum是所有孩子的dp值之和，不含自己
	CHAIN* ch;		//ch是链
	bool dfsvis;
}node[MAXN];
typedef void(*CBFUNC)(NODE *x, NODE *y, bool last);
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXN<<1];
struct CHAIN{
	int w;
	NODE *a, *b;
	CHAIN *next;
}chain[MAXN];
int N, M, ecnt, ccnt;

namespace LCA{
	int rmqarr[MAXN<<1][30];
	NODE* anc[MAXN<<1];
	int cnt;
	void DFS(NODE *cur){
		EDGE *j;

		cur->pfst = ++ cnt;
		anc[cnt] = cur;
		rmqarr[cnt][0] = cur->pfst;
		for(j = cur->head; j; j = j->next){
			if(j->b == cur->par)
				continue;
			j->b->par = cur;
			DFS(j->b);
			rmqarr[++ cnt][0] = cur->pfst;
			anc[cnt] = cur;
		}
		cur->plast = cnt;
	}
	NODE* LCA(NODE* a, NODE* b){
		int aa = a->pfst, bb = b->pfst, m;
		if(aa > bb)
			swap(aa, bb);
		m = log2(bb - aa + 1);
		return anc[min(rmqarr[aa][m], rmqarr[bb - (1<<m) + 1][m])];
	}
	void PreLCA(){
		int m, i, j;
		cnt = 0;
		DFS(node + 1);
		m = log2(cnt);
		for(j = 1; j <= m; j ++){
			for(i = 1;i <= cnt - (1<<(j-1)); i ++){
				rmqarr[i][j] = min(rmqarr[i][j-1], rmqarr[i + (1<<(j-1))][j-1]);
			}
		}
	}
};

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
	if(va->dep < vb->dep)
		swap(va, vb);
	cbf(va, vb, true);
}

class Segtree{
public:
	struct SN{
		LL dp, sum;
		SN *lchd, *rchd;
		inline void popup(){
			dp = lchd->dp + rchd->dp;
			sum = lchd->sum + rchd->sum;
		}
	}node[MAXN<<1];
	int ncnt;
	SN* build(int l, int r){
		SN* ret = &node[ncnt ++];
		ret->dp = ret->sum = 0;
		if(l == r){
			ret->lchd = ret->rchd = NULL;
		}else{
			int mid = (l + r) >> 1;
			ret->lchd = build(l, mid);
			ret->rchd = build(mid + 1, r);
		}
		return ret;
	}
	pair<LL, LL> query(SN* rt, int l, int r, int L, int R){
		if(L == l && r == R){
			return make_pair(rt->dp, rt->sum);
		}else{
			int mid = (L + R) >> 1;
			if(r <= mid){
				return query(rt->lchd, l, r, L, mid);
			}else if(l > mid){
				return query(rt->rchd, l, r, mid + 1, R);
			}else{
				pair<LL, LL> r1 = query(rt->lchd, l, mid, L, mid);
				pair<LL, LL> r2 = query(rt->rchd, mid + 1, r, mid + 1, R);
				return make_pair(r1.first + r2.first, r1.second + r2.second);
			}
		}
	}
	void update(SN* rt, int pos, const LL& dp, const LL& sum, int L, int R){
		if(L == R){
			rt->dp = dp;
			rt->sum = sum;
		}else{
			int mid = (L + R) >> 1;
			if(pos <= mid){
				update(rt->lchd, pos, dp, sum, L, mid);
			}else{
				update(rt->rchd, pos, dp, sum, mid + 1, R);
			}
			rt->popup();
		}
	}
public:
	inline void Build(){
		ncnt = 0;
		build(1, N);
	}
	inline pair<LL, LL> Query(int l, int r){
		return query(node, l, r, 1, N);
	}
	inline void Update(int pos, const LL& dp, const LL& sum){
		update(node, pos, dp, sum, 1, N);
	}
}segt;

LL gans;
void CalcOnTree(NODE *x, NODE *y, bool last){
	int pa = x->w, pb = y->w;
	pair<LL, LL> ans;
	if(last){
		pb ++;
	}
	if(pa >= pb)
		ans = segt.Query(pb, pa);
	else
		ans = make_pair(0, 0);
	gans -= ans.first;
	gans += ans.second;
}

void Deal(NODE* x){
	EDGE *j;
	CHAIN *ch;

	//预处理sum
	x->dp = x->sum = 0;
	for(j = x->head; j; j = j->next){
		if(j->b != x->par){
			Deal(j->b);
			x->sum += j->b->dp;
		}
	}

	//计算dp
	x->dp = x->sum;
	if(x->id == 1)
		x = x;
	for(ch = x->ch; ch; ch = ch->next){
		gans = x->sum;
		breakdown(ch->a, x, CalcOnTree);
		breakdown(ch->b, x, CalcOnTree);
		x->dp = max(x->dp, int(gans + ch->w));
	}

	//修改线段树
	segt.Update(x->w, x->dp, x->sum);
}

void addedge(int a, int b){
	edge[ecnt].a = &node[a];
	edge[ecnt].b = &node[b];
	edge[ecnt].next = node[a].head;
	node[a].head = &edge[ecnt ++];
}

void addchain(NODE* a, NODE* b, NODE* lca, int w){
	chain[ccnt].a = a;
	chain[ccnt].b = b;
	chain[ccnt].w = w;
	chain[ccnt].next = lca->ch;
	lca->ch = &chain[ccnt ++];
}

void domain(){
	int i, a, b;
	scanf("%d %d", &N, &M);
	memset(node, 0, sizeof(NODE) * (N + 1));
	for(i = 1; i <= N; i ++){
		node[i].id = i;
	}
	ecnt = ccnt = 0;
	for(i = 1; i < N; i ++){
		scanf("%d%d", &a, &b);
		addedge(a, b);
		addedge(b, a);
	}
	LCA::PreLCA();
	while(M --){
		scanf("%d %d %d", &a, &b, &i);
		addchain(node + a, node + b, LCA::LCA(node + a, node + b), i);
	}
	BTS::Split(node + 1);
	segt.Build();
	Deal(node + 1);
	printf("%d\n", node[1].dp);
}

int main(){
	int T;
	scanf("%d", &T);
	while(T --)
		domain();
	return 0;
}
