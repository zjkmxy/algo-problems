#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 300005;
class Treap{
private:
	typedef struct NODE{
		NODE *l, *r, *par;  //左，右，父亲
		bool root;          //是否为根（可以用函数查）
		int fix, size;

		//补充标记
		int add;    //路径加和
		bool rev;    //反转
		int mx;        //最大值
		int val;    //本值
		NODE(int val = 0):fix(rand()),size(1),val(val){
			par = l = r = nullptr;
			root = true;

			add = 0;
			mx = val;
			rev = false;
		};
	}*PNODE;
	PNODE root;
	NODE node[MAXN];
	int pcnt;
	typedef pair<PNODE, PNODE> Droot;
	inline int size(PNODE cur){  //取得大小
		return cur ? cur->size : 0;
	}
	inline void do_rev(PNODE cur){
		if(cur){
			cur->rev = !cur->rev;
			swap(cur->l, cur->r);
		}
	}
	inline void do_add(PNODE cur, int v){
		if(cur){
			cur->add += v;
			cur->mx += v;
			cur->val += v;
		}
	}
	inline void pushdown(PNODE cur){
		if(cur->rev){
			do_rev(cur->l);
			do_rev(cur->r);
			cur->rev = false;
		}
		if(cur->add){
			do_add(cur->l, cur->add);
			do_add(cur->r, cur->add);
			cur->add = 0;
		}
	}
	inline void popup(PNODE cur){
		cur->size = 1;
		cur->mx = cur->val;
		if(cur->l){
			cur->l->root = false;
			cur->l->par = cur;
			cur->size += cur->l->size;
			cur->mx = max(cur->mx, cur->l->mx);
		}
		if(cur->r){
			cur->r->root = false;
			cur->r->par = cur;
			cur->size += cur->r->size;
			cur->mx = max(cur->mx, cur->r->mx);
		}
	}
	void getrank(PNODE A, PNODE& rt, int& rk){
		static PNODE stk[MAXN];
		int sp = 0;
		//先强制更新
		rt = A;
		while(!rt->root){
			rt = rt->par;
			stk[sp ++] = rt;
		}
		while(sp --){
			pushdown(stk[sp]);
		}
		//再处理操作
		rt = A;
		rk = size(rt->l) + 1;
		while(!rt->root){
			if(rt == rt->par->r)
				rk += rt->par->size - rt->size;
			rt = rt->par;
		}
	}
	PNODE concat_it(PNODE A, PNODE B){ //保序合并
		if(!A) return B;
		if(!B) return A;
		if(A->fix < B->fix){
			pushdown(A);
			A->r = concat_it(A->r, B);
			popup(A);
			return A;
		}else{
			pushdown(B);
			B->l = concat_it(A, B->l);
			popup(B);
			return B;
		}
	}
	inline PNODE concat(PNODE A, PNODE B){
		PNODE ret = concat_it(A, B);
		ret->root = true;
		return ret;
	}
	Droot split_it(PNODE A, int k){
		if(!A) return Droot(nullptr, nullptr);
		Droot ret;
		pushdown(A);
		A->root = true;
		if(size(A->l) >= k){
			ret = split_it(A->l, k);
			A->l = ret.second;
			popup(A);
			ret.second = A;
		}else{
			ret = split_it(A->r, k - size(A->l) - 1);
			A->r = ret.first;
			popup(A);
			ret.first = A;
		}
		return ret;
	}
	inline Droot split(PNODE rt, int k){  //分裂点，排位
		NODE *tmp = rt->par;
		Droot ret = split_it(rt, k);
		if(ret.first)
			ret.first->par = tmp;
		if(ret.second){
			for(tmp = ret.first; tmp->r; tmp = tmp->r);
			ret.second->par = tmp;
		}
		return ret;
	}
	PNODE access(PNODE A){
		PNODE rt, pchd = nullptr;
		int rk;
		while(A){
			getrank(A, rt, rk);
			A = rt->par;
			pchd = concat(split(rt, rk).first, pchd);
			pchd->par = A;
		}
		return pchd;
	}
public:
	void Init(int *arr, int N){
		pcnt = N + 1;
		for(int i = 1; i <= N; i ++){
			node[i] = NODE(arr[i]);
		}
	}
	inline NODE* GetRt(PNODE p){
		while(p->par)
			p = p->par;
		return p;
	}
	inline void Link(PNODE u, PNODE p){
		PNODE rt = access(u);
		do_rev(rt);
		rt->par = p;
	}
	inline void Cut(PNODE u){
		PNODE rt;
		int rk;
		access(u);
		getrank(u, rt, rk);
		rt = split(rt, rk - 1).second;
		rt->par = nullptr;
	}
	inline void ChangeRoot(PNODE u){
		do_rev(access(u));
	}
	inline void DealOne(int u_no, int v_no){
		PNODE u = node + u_no, v = node + v_no;
		if(GetRt(u) == GetRt(v)){
			puts("-1");
		}else{
			Link(u, v);
		}
	}
	inline void DealTwo(int u_no, int v_no){
		PNODE u = node + u_no, v = node + v_no;
		if(u == v || GetRt(u) != GetRt(v)){
			puts("-1");
		}else{
			ChangeRoot(u);
			Cut(v);
		}
	}
	inline void DealThree(int w, int u_no, int v_no){
		PNODE u = node + u_no, v = node + v_no;
		if(GetRt(u) != GetRt(v)){
			puts("-1");
		}else{
			ChangeRoot(u);
			v = access(v);
			do_add(v, w);
		}
	}
	inline void DealFour(int u_no, int v_no){
		PNODE u = node + u_no, v = node + v_no;
		if(GetRt(u) != GetRt(v)){
			puts("-1");
		}else{
			ChangeRoot(u);
			v = access(v);
			printf("%d\n", v->mx);
		}
	}
	inline void DLink(int u_no, int v_no){
		Link(node + u_no, node + v_no);
	}
}tree;

int eu[MAXN], ev[MAXN], arr[MAXN], N, Q;
int main(){
	int u, v, w, op;
	while(scanf("%d", &N) != EOF){
		for(int i = 0; i < N - 1; i ++){
			scanf("%d%d", &eu[i], &ev[i]);
		}
		for(int i = 1; i <= N; i ++){
			scanf("%d", &arr[i]);
		}
		tree.Init(arr, N);
		for(int i = 0; i < N - 1; i ++){
			tree.DLink(eu[i], ev[i]);
		}
		scanf("%d", &Q);
		while(Q --){
			scanf("%d%d%d", &op, &u, &v);
			if(op == 1){
				tree.DealOne(u, v);
			}else if(op == 2){
				tree.DealTwo(u, v);
			}else if(op == 3){
				w = u;
				scanf("%d", &u);
				tree.DealThree(w, u, v);
			}else{
				tree.DealFour(u, v);
			}
		}
		puts("");
	}
	return 0;
}