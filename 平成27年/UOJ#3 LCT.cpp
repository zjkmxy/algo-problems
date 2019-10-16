#include <cstdio>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
const int INF = 0x3fffffff;
struct EDGE{
	int a, b;
	int p, q;
	bool used;
}edge[MAXM];
int N, M, Q;
struct OPT{
	int p, q, id;
	OPT(){};
	OPT(int p, int q, int id):p(p),q(q),id(id){};
	bool operator<(const OPT& rhs)const{
		if(q != rhs.q)
			return q < rhs.q;
		else
			return p < rhs.p;
	};
}opt[MAXM];

char B[1<<15], *S=B, *T=B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
	while(ch=getc(), !isdigit(ch));
	int aa = ch - '0';
	while(ch=getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
	return aa;
}

class LCT{
private:
	typedef struct NODE{
		NODE *l, *r, *par;  //左，右，父亲
		bool root;          //是否为根（可以用函数查）
		int fix, size;

		//补充标记
		bool rev;    //反转
		int val;    //本值
		NODE* mx;    //最大值点
		NODE(int val = -1):fix(rand()),size(1),val(val){
			par = l = r = nullptr;
			root = true;

			mx = this;
			rev = false;
		};
	}*PNODE;
	PNODE root;
	NODE node[MAXN], edl[MAXM];    //代表顶点和边的点
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
	inline void pushdown(PNODE cur){
		if(cur->rev){
			do_rev(cur->l);
			do_rev(cur->r);
			cur->rev = false;
		}
	}
	inline void popup(PNODE cur){
		cur->size = 1;
		cur->mx = cur;
		if(cur->l){
			cur->l->root = false;
			cur->l->par = cur;
			cur->size += cur->l->size;
			if(cur->l->mx->val > cur->mx->val)
				cur->mx = cur->l->mx;
		}
		if(cur->r){
			cur->r->root = false;
			cur->r->par = cur;
			cur->size += cur->r->size;
			if(cur->r->mx->val > cur->mx->val)
				cur->mx = cur->r->mx;
		}
	}

	//这里其实是可以自底向上写的，比较烦没写
	inline void getrank(PNODE A, PNODE& rt, int& rk){
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
private:
	inline NODE* GetRt(PNODE p){
		while(p->par)
			p = p->par;
		return p;
	}
	inline void Link(int j){
		edl[j].par = node + edge[j].a;
		PNODE rt = access(edl + j);
		do_rev(rt);
		rt->par = node + edge[j].b;
		edge[j].used = true;
	}
	inline void DoCut(PNODE u){
		PNODE rt;
		int rk;
		access(u);
		getrank(u, rt, rk);
		rt = split(rt, rk - 1).second;
		rt->par = nullptr;
	}
	inline void Cut(int j){
		ChangeRoot(edl + j);
		DoCut(node + edge[j].a);
		DoCut(node + edge[j].b);
		edge[j].used = false;
	}
	inline void ChangeRoot(PNODE u){
		do_rev(access(u));
	}

public:
	//题目中操作：加边换边
	void AddEdge(int j){
		PNODE u = node + edge[j].a, v = node + edge[j].b, z;
		//是否直接链接？
		if(GetRt(u) != GetRt(v)){
			Link(j);
			return;
		}
		//访问路径最大
		ChangeRoot(u);
		v = access(v);
		z = v->mx;
		//换边？
		if(edge[j].p < z->val){
			Cut(z - edl);
			Link(j);
		}
	}
	//题目中操作：路径最大
	int Query(){
		PNODE u =  &node[1], v = &node[N];
		if(GetRt(u) != GetRt(v)){
			return INF;
		}
		ChangeRoot(u);
		v = access(v);
		u = v->mx;
		return u->val;
	}
	//初始化
	void Init(int N, int M){
		int i;

		for(i = 0; i <= N; i ++)
			node[i] = NODE();
		for(i = 0; i < M; i ++){
			edge[i].a = ReadInt();
			edge[i].b = ReadInt();
			edge[i].p = ReadInt();
			edge[i].q = ReadInt();
			opt[i] = OPT(edge[i].p, edge[i].q, i);
			edge[i].used = false;
			edl[i] = NODE(edge[i].p);
		}
		sort(opt, opt + M);
	}
}tree;

int main(){
	int i, ans = INF, cur;

	srand(73);

	N = ReadInt(), M = ReadInt();
	tree.Init(N, M);
	for(i = 0; i < M; i ++){
		tree.AddEdge(opt[i].id);
		cur = opt[i].q + tree.Query();
		ans = min(ans, cur);
	}
	if(ans >= INF)
		ans = -1;
	printf("%d\n", ans);
	return 0;
}
