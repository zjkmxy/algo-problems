#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 500005;
const int INF = 0x3fffffff;

struct EDGE;
struct NODE{
	int id;
	EDGE *head;
	NODE *par, *top;//父节点，链头
	int siz, dep;	//子树尺寸,深度
	int w;			//映射位置
	//限DFS
	int plast;  //子树的末尾
	NODE *son;  //重儿子
	//
	int val;	//父向边距离，根的话永远是INF
	bool vis;	//是否用到过
	bool root;	//是否是根（重要）
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXN];
int M, Q, ecnt;
int varr[MAXN];

//Big Cactus Saw......
namespace BCS{
	void DFS(NODE *x){
		x->siz = 1;
		x->son = nullptr;
		for(EDGE *j = x->head; j; j = j->next){
			NODE *u = j->b;
			//if(u == x->par)
			//	continue;
			u->par = x;
			if(u->root)
				continue;
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
			if(j->b->root || j->b == x->son)
				continue;
			tp = j->b;
			DFS2(j->b);
		}
		x->plast = tz;
	}
	int Split(NODE *root){
		tp = root;
		root->root = true;
		DFS(root);
		DFS2(root);
		return tz;
	}
};

//线段树
class TREE{
private:
	struct STNODE{
		int l, r;
		STNODE *lchd, *rchd;

		int val;	//最大的数值
		int pos;	//最右的TRUE节点位置
	}node[MAXN<<2];
	int cnt, N;
	STNODE *root;
	inline void popup(STNODE *cur){
		cur->val = max(cur->lchd->val, cur->rchd->val);
		cur->pos = max(cur->lchd->pos, cur->rchd->pos);
	}
	STNODE* build(int l, int r){
		STNODE *ret = node + cnt;
		cnt ++;
		ret->l = l;
		ret->r = r;
		ret->pos = -1;
		if(l != r){
			int mid = (l + r) >> 1;
			ret->lchd = build(l, mid);
			ret->rchd = build(mid + 1, r);
			popup(ret);
		}else{
			ret->lchd = ret->rchd = nullptr;
			ret->val = varr[l];
		}
		return ret;
	}
	//设置某一个点是TRUE或者清除TRUE
	void set(STNODE* cur, int pos, bool val){
		if(cur->l == cur->r){
			cur->pos = val ? cur->l : -1;
		}else{
			int mid = (cur->l + cur->r) >> 1;
			if(pos <= mid){
				set(cur->lchd, pos, val);
			}else{
				set(cur->rchd, pos, val);
			}
			popup(cur);
		}
	}
	//取得到最右TRUE点的值
	int query(STNODE* cur, int l, int r){
		if(cur->l == l && cur->r == r){
			return cur->val;
		}else{
			int mid = (cur->l + cur->r) >> 1;
			if(r <= mid){
				return query(cur->lchd, l, r);
			}else if(l > mid){
				return query(cur->rchd, l, r);
			}else{
				return max(query(cur->lchd, l, mid), query(cur->rchd, mid + 1, r));
			}
		}
	}
	//取得区间最右TRUE点的位置
	int getpos(STNODE* cur, int l, int r){
		if(cur->l == l && cur->r == r){
			return cur->pos;
		}else{
			int mid = (cur->l + cur->r) >> 1;
			if(r <= mid){
				return getpos(cur->lchd, l, r);
			}else if(l > mid){
				return getpos(cur->rchd, l, r);
			}else{
				return max(getpos(cur->lchd, l, mid), getpos(cur->rchd, mid + 1, r));
			}
		}
	}
public:
	inline void Init(int N){
		cnt = 0;
		this->N = N;
		root = build(1, N);
	}
	inline void Set(int pos, bool val){
		set(root, pos, val);
	}
	inline int GetPos(int l, int r){
		if(l > r)
			return -1;
		return getpos(root, l, r);
	}
	inline int Query(int l, int r){
		if(l > r)
			return -1;
		return query(root, l, r);
	}
}tree;

//并查集
class UnionSet{
public:
	int p[MAXN];
	void Init(){
		for(int i = 0; i < MAXN; i ++)
			p[i] = i;
	}
	int GetPar(int x){
		if(p[x] != x)
			p[x] = GetPar(p[x]);
		return p[x];
	}
	bool Merge(int x, int y){
		x = GetPar(x);
		y = GetPar(y);
		if(x == y)
			return false;
		p[x] = y;
		return true;
	}
}us;

void init(){
	ecnt = 0;
	for(int i = 0; i < MAXN; i ++){
		node[i].dep = node[i].siz = node[i].w = 0;
		node[i].par = node[i].top = nullptr;
		node[i].head = nullptr;
		node[i].id = i;

		node[i].val = INF;
		node[i].vis = false;
		node[i].root = false;
	}
	us.Init();
}

void addedge(NODE *x, NODE *y, int val){
	edge[ecnt].a = x;
	edge[ecnt].b = y;
	edge[ecnt].next = x->head;
	x->head = edge + ecnt;
	ecnt++;

	y->val = val;
	x->vis = y->vis = true;
	if(!us.Merge(x->id, y->id)){
		x->root = true;
	}
}

int query(NODE* cur){
	int pos = -1, ans = 0;
	//向树根处理
	while(pos == -1){
		pos = tree.GetPos(cur->top->w, cur->w);
		if(pos > 0){
			ans = max(ans, tree.Query(pos + 1, cur->w));
		}else{
			ans = max(ans, tree.Query(cur->top->w, cur->w));
		}
		cur = cur->top;
		if(cur->root)
			break;
		else
			cur = cur->par;
	}
	if(pos != -1)
		return ans;
	//环形处理
	cur = cur->par;
	while(cur && pos == -1){
		pos = tree.GetPos(cur->top->w, cur->w);
		if(pos > 0){
			ans = max(ans, tree.Query(pos + 1, cur->w));
		}else{
			ans = max(ans, tree.Query(cur->top->w, cur->w));
		}
		cur = cur->top;
		if(cur->root)
			break;
		else
			cur = cur->par;
	}
	if(pos != -1)
		return ans;
	else
		return INF;
}

void deal(){
	int i, a, ans, T, F, sp = 0;
	static int t_stk[MAXN];

	scanf("%d", &T);
	for(i = 0; i < T; i ++){
		scanf("%d", &a);
		if(!node[a].vis)
			continue;
		tree.Set(node[a].w, true);
		t_stk[sp ++] = a;
	}
	ans = INF;
	scanf("%d", &F);
	for(i = 0; i < F; i ++){
		scanf("%d", &a);
		if(!node[a].vis)
			continue;
		ans = min(ans, query(node + a));
	}
	while(sp --){
		tree.Set(node[t_stk[sp]].w, false);
	}
	//if(ans == 0)
	//	throw 0;
	if(ans < INF){
		printf("%d\n", ans);
	}else{
		printf("-1\n");
	}
}

int main(){
	int a, b, i;

	init();
	scanf("%d", &M);
	for(i = 1; i <= M; i ++){
		scanf("%d%d", &a, &b);
		addedge(node + a, node + b, i);
	}
	for(i = 1; i < MAXN; i ++){
		//先找树的根
		if(node[i].vis && node[i].val == INF)
			node[i].root = true;
		//树或者环的根
		if(node[i].root)
			a = BCS::Split(node + i);
	}
	for(i = 1; i < MAXN; i ++){
		varr[node[i].w] = node[i].val;
	}
	tree.Init(a);

	scanf("%d", &Q);
	while(Q --){
		deal();
	}
	return 0;
}
