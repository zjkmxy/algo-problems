#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wsign-compare"
#define nullptr NULL

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <limits>
using namespace std;

const int MAXN = 1005;
const double INF = numeric_limits<double>::max() / 3.0;
double mat[MAXN][MAXN];
struct NODE;
struct EDGE;
typedef NODE *PNODE;
typedef EDGE *PEDGE;
struct NODE{
	int id;		//编号
	bool vis;	//是否探查过
	double dis;	//Prim距离
	PNODE par, top, son;	//父亲，链头，重孩子
	PEDGE head;
	int siz, dep;
	int w, plast;
}node[MAXN];
typedef void(*CBFUNC)(NODE *x, NODE *y, bool last);
struct EDGE{
	PNODE a, b;
	PEDGE next;
	double c;
}edge[MAXN];
int N, M, Q, ecnt;
double mst, ans;

void addedge(PNODE a, PNODE b, double c){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].c = c;
	edge[ecnt].next = a->head;
	a->head = &edge[ecnt];
	ecnt ++;
}

double Prim(){
	int i, j, lab;
	double ans = 0.0;

	ecnt = 0;
	node[0].vis = true;
	for(j = 1; j < N; j ++){
		node[j].dis = mat[0][j];
		node[j].par = &node[0];
	}
	for(i = 1; i < N; i ++){
		for(j = 0; node[j].vis; j ++);
		lab = j;
		for(j ++; j < N; j ++){
			if(!node[j].vis && node[j].dis < node[lab].dis)
				lab = j;
		}
		ans += node[lab].dis;
		node[lab].vis = true;
		addedge(node[lab].par, node + lab, node[lab].dis);
		for(j = 0; j < N; j ++){
			if(!node[j].vis && node[j].dis > mat[lab][j]){
				node[j].dis = mat[lab][j];
				node[j].par = &node[lab];
			}
		}
	}
	return ans;
}

//大树锯
namespace BTS{
	void DFS(NODE *x){
		x->siz = 1;  	x->son = nullptr;
		for(EDGE *j = x->head; j; j = j->next){
			NODE *u = j->b;
			if(u == x->par) 	continue;
			u->par = x;	u->dep = x->dep + 1;
			DFS(u);
			if(!x->son || u->siz > x->son->siz) 	x->son = u;
			x->siz += u->siz;
		}
	}
	int tz; NODE *tp;
	void DFS2(NODE *x){
		x->w = ++tz;	x->top = tp;  //分配编号
		if(x->son)  DFS2(x->son);
		for(EDGE *j = x->head; j; j = j->next){
			if(j->b == x->son || j->b == x->par) 	continue;
			tp = j->b; 	DFS2(j->b);
		}
		x->plast = tz;
	}
	inline void Split(NODE *root){
		tz = 0, tp = root, root->dep = 0;	DFS(root);	DFS2(root);
	}
};
//剖分路径
void Breakdown(NODE *va, NODE *vb, CBFUNC cbf){
	NODE *f1 = va->top, *f2 = vb->top;
	while(f1 != f2){
		if(f1->dep < f2->dep){
			swap(f1, f2);	swap(va, vb);
		}
		cbf(va, f1, false);	va = f1->par;	f1 = va->top;
	}
	if(va->dep > vb->dep)	swap(va, vb);
	cbf(vb, va, true);
}

//最小值线段树
class Segtree{
private:
	struct TNODE{
		double mark;	//修改标记
		double val;	//区间最小值
	}node[MAXN<<3];
	int n;
	inline int LCHD(int id){
		return id << 1;
	}
	inline int RCHD(int id){
		return id << 1 | 1;
	}
	inline int MID(int l, int r){
		return (l + r) >> 1;
	}
	inline void popup(int id){
		node[id].val = min(node[LCHD(id)].val, node[RCHD(id)].val);
	}
	inline void pushdown(int id){
		if(node[id].mark < INF){
			dealson(LCHD(id), node[id].mark);
			dealson(RCHD(id), node[id].mark);
			node[id].mark = INF;
		}
	}
	inline void dealson(int id, double mk){
		node[id].mark = min(node[id].mark, mk);
		node[id].val = min(node[id].val, mk);
	}
	void build(int id, int l, int r){
		node[id].val = node[id].mark = INF;
		if(l != r){
			int mid = MID(l, r);
			build(LCHD(id), l, mid);
			build(RCHD(id), mid + 1, r);
			popup(id);
		}
	}
	void update(int id, int l, int r, double mk, int L, int R){
		int mid = MID(L, R);
		if(L == l && r == R){
			dealson(id, mk);
		}else{
			pushdown(id);
			if(r <= mid){
				update(LCHD(id), l, r, mk, L, mid);
			}else if(l > mid){
				update(RCHD(id), l, r, mk, mid + 1, R);
			}else{
				update(LCHD(id), l, mid, mk, L, mid);
				update(RCHD(id), mid + 1, r, mk, mid + 1, R);
			}
			popup(id);
		}
	}
	double query(int id, int pos, int L, int R){
		int mid = MID(L, R);
		if(L == R){
			return node[id].val;
		}else{
			pushdown(id);
			if(pos <= mid){
				return query(LCHD(id), pos, L, mid);
			}else{
				return query(RCHD(id), pos, mid + 1, R);
			}
		}
	}
public:
	inline void Build(int N){
		n = N;
		build(1, 1, n);
	}
	inline void Update(int l, int r, double mk){
		update(1, l, r, mk, 1, n);
	}
	inline double Query(int pos){
		return query(1, pos, 1, n);
	}
}segt;


double CBFVAL;
void cbf(NODE *x, NODE *y, bool last){
	if(!last){
		segt.Update(y->w, x->w, CBFVAL);
	}else{
		if(y->w >= x->w)
			return ;
		segt.Update(y->w + 1, x->w, CBFVAL);	//LCA不能更新，其他点都更新
	}
}

bool Solve(){
	int a, b, c, i;
	scanf("%d%d", &N, &M);
	if(N == 0)
		return false;
	for(i = 0; i < N; i ++){
		node[i].id = i;
		node[i].par = NULL;
		node[i].dis = INF;
		node[i].vis = false;
		node[i].head = NULL;
	}
	for(a = 0; a < N; a ++){
		for(b = 0; b < N; b ++)
			mat[a][b] = a == b ? 0. : INF;
	}
	while(M --){
		scanf("%d%d%d", &a, &b, &c);
		mat[a][b] = mat[b][a] = c;
	}

	mst = Prim();
	BTS::Split(&node[0]);
	segt.Build(N);
	for(a = 0; a < N; a ++){
		for(b = a + 1; b < N; b ++){
			if(node[a].par != &node[b] && node[b].par != &node[a] && mat[a][b] < INF){
				CBFVAL = mat[a][b];
				Breakdown(node + a, node + b, cbf);
			}
		}
	}

	ans = 0.0;
	scanf("%d", &Q);
	for(i = 0; i < Q; i ++){
		scanf("%d%d%d", &a, &b, &c);
		if(node[b].par == &node[a])
			swap(a, b);
		if(node[a].par == &node[b]){
			ans += mst - mat[a][b] + min(double(c), segt.Query(node[a].w));
		}else{
			ans += mst;
		}
	}
	printf("%.4lf\n", ans / Q);

	return true;
}

int main(){
	while(Solve());
	return 0;
}