#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 100005;
typedef long long ll;
int N;

class Treap{
private:
	struct NODE{
		NODE *l, *r, *par;
		int fix, val, size;
		int mv;	//最早插入点
		ll sum;	//总和
		NODE(int val):fix(rand()),val(val),size(1){
			par = l = r = nullptr;
			sum = val;
			mv = val >= 0 ? val : -1;
		};
		NODE(){};
	}node[MAXN<<1], *root;
	typedef NODE *PNODE;
	int ncnt;
	PNODE push[MAXN], pop[MAXN];	//加减档的位点

	typedef pair<NODE*, NODE*> Droot;
	inline int size(NODE* cur){  //取得大小
		return cur ? cur->size : 0;
	}
	inline void pushdown(NODE* cur){}
	inline void popup(NODE* cur){
		cur->size = 1;
		cur->mv = -1;
		cur->sum = cur->val;
		if(cur->l){
			cur->l->par = cur;
			cur->size += cur->l->size;
			cur->mv = cur->l->mv;
			cur->sum += cur->l->sum;
		}
		if(cur->mv == -1 && cur->val >= 0)
			cur->mv = cur->val;
		if(cur->r){
			cur->r->par = cur;
			cur->size += cur->r->size;
			if(cur->mv == -1)
				cur->mv = cur->r->mv;
			cur->sum += cur->r->sum;
		}
	}
	NODE *concat(NODE *A, NODE *B){ //保序合并
		if(!A) return B;
		if(!B) return A;
		if(A->fix < B->fix){
			pushdown(A);
			A->r = concat(A->r, B);
			popup(A);
			return A;
		}else{
			pushdown(B);
			B->l = concat(A, B->l);
			popup(B);
			return B;
		}
	}
	Droot split(NODE *A, int k){  //分裂:(前k个,剩下的)
		if(!A) return Droot(nullptr, nullptr);
		Droot ret;
		pushdown(A);
		if(size(A->l) >= k){
			ret = split(A->l, k);
			A->l = ret.second;
			popup(A);
			ret.second = A;

			A->par = nullptr;
		}else{
			ret = split(A->r, k - size(A->l) - 1);
			A->r = ret.first;
			popup(A);
			ret.first = A;

			A->par = nullptr;
		}
		return ret;
	}
	int getrank(PNODE A){
		PNODE rt = A;
		int rk = size(rt->l) + 1;
		while(rt->par){
			if(rt == rt->par->r)
				rk += rt->par->size - rt->size;
			rt = rt->par;
		}
		return rk;
	}

	void print(PNODE p){
		if(p->l) print(p->l);
		printf("%d ", p->val);
		if(p->r) print(p->r);
	}
public:
	inline void Build(){
		root = nullptr;
		ncnt = 0;
	}

	void DoInsert(int pos, int val){
		Droot x = split(root, pos);
		int mv = x.second ? x.second->mv : -1;
		NODE *p = &node[ncnt ++];
		*p = NODE(val);
		push[val] = p;

		if(mv == -1)
			mv = size(x.second);
		else
			mv = getrank(pop[mv]) - 1;

		Droot y = split(x.second, mv);
		NODE *q = &node[ncnt ++];
		*q = NODE(- val);
		pop[val] = q;
		root = concat(x.first, concat(p, concat(y.first, concat(q, y.second))));
	}
	void DoDelete(int val){
		int pos = getrank(push[val]);
		Droot x = split(root, pos - 1);
		Droot y = split(x.second, 1);
		root = concat(x.first, y.second);

		pos = getrank(pop[val]);
		x = split(root, pos - 1);
		y = split(x.second, 1);
		root = concat(x.first, y.second);

		push[val] = pop[val] = nullptr;
	}
	ll DoQuery(int val){
		int p1 = getrank(push[val]), p2 = getrank(pop[val]);
		Droot x = split(root, p2 - 1);
		Droot y = split(x.first, p1);
		ll ans = y.second ? y.second->sum : 0;
		root = concat(concat(y.first, y.second), x.second);
		return ans;
	}
	void Print(){
		if(root) print(root);
		printf("\n");
	}
}tree;


class Segtree{
public:
	int sn[MAXN<<2];	//最小未用，全用了就是INF
	
	void build(int id, int l, int r){
		sn[id] = l;
		if(l < r){
			int mid = (l + r) >> 1;
			build(id << 1, l, mid);
			build(id << 1 | 1, mid + 1, r);
		}
	}
	void update(int id, int p, int v, int L, int R){
		if(L == R){
			sn[id] = v;
			return ;
		}
		int mid = (L + R) >> 1;
		if(p <= mid)
			update(id << 1, p, v, L, mid);
		else
			update(id << 1 | 1, p, v, mid + 1, R);
		sn[id] = min(sn[id << 1], sn[id << 1 | 1]);
	}
	int query(int id, int l, int r, int L, int R){
		if(l == L && R == r){
			return sn[id];
		}
		int mid = (L + R) >> 1;
		if(r <= mid){
			return query(id << 1, l, r, L, mid);
		}else if(l > mid){
			return query(id << 1 | 1, l, r, mid + 1, R);
		}else{
			return min(query(id << 1, l, mid, L, mid), query(id << 1 | 1, mid + 1, r, mid + 1, R));
		}
	}
public:
	inline void Build(){
		build(1, 1, N);
	}
	inline void Update(int p, int v){
		update(1, p, v, 1, N);
	}
	inline int Query(int l, int r){
		return query(1, l, r, 1, N);
	}
}segt;

bool domain(){
	static int cas;
	int i, v, v2;
	char op[10];

	if(scanf("%d", &N) == EOF)
		return false;
	printf("Case #%d:\n", ++ cas);

	segt.Build();
	tree.Build();
	for(i = 0; i < N; i ++){
		scanf("%s%d", &op, &v);
		if(op[0] == 'i'){
			v2 = segt.Query(1, N);
			tree.DoInsert(v, v2);
			segt.Update(v2, 0x3fffffff);
		}else if(op[0] == 'r'){
			tree.DoDelete(v);
			segt.Update(v, v);
		}else{
			ll sum = tree.DoQuery(v);
			printf("%I64d\n", sum);
		}

		//tree.Print();
	}
	return true;
}

int main(){
	while(domain());
	return 0;
}
