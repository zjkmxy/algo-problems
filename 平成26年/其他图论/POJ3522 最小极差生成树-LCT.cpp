#define nullptr
#include <cstdio>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
const int INF = 0x3fffffff;
struct EDGE{
	int a, b;
	int w;
	bool used;	//是否使用
	bool mine;	//是否是最小边
	bool operator<(const EDGE& rhs)const{
		return w < rhs.w;
	}
}edge[MAXM];
int Ans;	//答案
int MePos;	//最小边位置
int N, M;

int ReadInt(){
	int ret = 0;
	char c;
	while(!isdigit(c = getchar()));
	ret = c - '0';
	while(isdigit(c = getchar())){
		ret = ret * 10 + (c - '0');
	}
	return ret; 
}

class LCT{
private:
	typedef struct NODE{
		NODE *l, *r, *par;  //左，右，父亲
		bool root;          //是否为根（可以用函数查）
		int fix, size;

		//补充标记
		bool rev;	//反转
		int val;	//本值
		NODE* mx;	//最小值点
		NODE(int val = INF):fix(rand()),size(1),val(val){
			par = l = r = nullptr;
			root = true;

			mx = this;
			rev = false;
		};
	}*PNODE;
	NODE node[MAXN], edl[MAXM];	//代表顶点和边的点
	int pcnt, ecnt;
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
			if(cur->l->mx->val < cur->mx->val)
				cur->mx = cur->l->mx;
		}
		if(cur->r){
			cur->r->root = false;
			cur->r->par = cur;
			cur->size += cur->r->size;
			if(cur->r->mx->val < cur->mx->val)
				cur->mx = cur->r->mx;
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
	//更新答案
	void Update(int del, int add){
		if(edge[del].mine != true)
			return ;
		edge[del].mine = false;
		for(; !edge[MePos].used; MePos ++);
		edge[MePos].mine = true;
		if(ecnt == N - 1)
			Ans = min(Ans, edge[add].w - edge[MePos].w);
	}
	//加边换边
	void AddEdge(int j){
		PNODE u = node + edge[j].a, v = node + edge[j].b, z;
		//是否直接链接？
		if(GetRt(u) != GetRt(v)){
			Link(j);
			ecnt ++;
			if(ecnt == 1)
				edge[j].mine = true;
			if(ecnt == N - 1)
				Update(MePos, j);
			return;
		}
		//访问路径最小
		ChangeRoot(u);
		v = access(v);
		z = v->mx;
		//换边
		if(edge[j].w > z->val){
			int lab = z - edl;
			Cut(lab);
			Link(j);
			Update(lab, j);
		}
	}
	//初始化
	void Init(int N, int M){
		pcnt = N + 1;
		ecnt = 0;
		Ans = INF;
		MePos = 0;
		for(int i = 1; i <= N; i ++){
			node[i] = NODE();
		}
		for(int i = 0; i < M; i ++){
			edge[i].a = ReadInt();
			edge[i].b = ReadInt();
			edge[i].w = ReadInt();
			edge[i].used = edge[i].mine = false;
		}
		sort(edge, edge + M);
		for(int i = 0; i < M; i ++){
			edl[i] = NODE(edge[i].w);
		}
	}
}tree;

int main(){
	while(true){
		scanf("%d%d", &N, &M);
		if(N == 0)
			break;
		tree.Init(N, M);
		for(int i = 0; i < M; i ++){
			tree.AddEdge(i);
		}
		if(Ans >= INF){
			puts("-1");
		}else{
			printf("%d\n", Ans);
		}
	}
	return 0;
}
