#include <cstdio>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 300005;
const int INF = 0x3fffffff;
struct EDGE{
	int a, b;
	int w;
	bool operator<(const EDGE& rhs)const{
		return w < rhs.w;
	}
	EDGE *next;
}edge[MAXM];
EDGE *head[MAXN];
bool isnp[MAXN];
int Ans;	//答案
int N, M;

class LCT{
public:
	typedef struct NODE{
		NODE *l, *r, *par;  //左，右，父亲
		bool root;          //是否为根（可以用函数查）
		int fix, size;

		//补充标记
		bool rev;	//反转
		int val;	//本值
		NODE* mx;	//最小值点
		int emp;	//对应的边值
		NODE():fix(rand()),size(1),emp(-1),val(INF){
			par = l = r = nullptr;
			root = true;
			mx = this;
			rev = false;
		};
		inline void do_rev(){ //点反转
			rev = !rev;
			swap(l, r);
		}
		inline void pushdown(){ //标记下传
			if(rev){
				if(l) l->do_rev();
				if(r) r->do_rev();
				rev = false;
			}
		}
		inline void popup(){ //信息维护
			size = 1;
			mx = this;
			if(l){
				l->root = false;
				l->par = this;
				size += l->size;
				if(l->mx->val < mx->val)
					mx = l->mx;
			}
			if(r){
				r->root = false;
				r->par = this;
				size += r->size;
				if(r->mx->val < mx->val)
					mx = r->mx;
			}
		}
	}*PNODE;
	NODE node[MAXN], edl[MAXN];	//代表顶点和边的点
	int pcnt, ecnt;
	typedef pair<PNODE, PNODE> Droot;

	inline int size(PNODE cur){  //取得大小
		return cur ? cur->size : 0;
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
		while(sp --)
			stk[sp]->pushdown();
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
			A->pushdown();
			A->r = concat_it(A->r, B);
			A->popup();
			return A;
		}else{
			B->pushdown();
			B->l = concat_it(A, B->l);
			B->popup();
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
		A->pushdown();
		A->root = true;
		if(size(A->l) >= k){
			ret = split_it(A->l, k);
			A->l = ret.second;
			A->popup();
			ret.second = A;
		}else{
			ret = split_it(A->r, k - size(A->l) - 1);
			A->r = ret.first;
			A->popup();
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
	inline void Link(int e, int j){
		edl[e].emp = j;
		edl[e].val = edge[j].w;

		edl[e].par = node + edge[j].a;
		PNODE rt = access(edl + e);
		rt->do_rev();
		rt->par = node + edge[j].b;
	}
	inline void DoCut(PNODE u){
		PNODE rt;
		int rk;
		access(u);
		getrank(u, rt, rk);
		rt = split(rt, rk - 1).second;
		rt->par = nullptr;
	}
	inline void Cut(int e, int j){
		ChangeRoot(edl + e);
		DoCut(node + edge[j].a);
		DoCut(node + edge[j].b);
	}
	inline void ChangeRoot(PNODE u){
		access(u)->do_rev();
	}

public:
	//加边换边
	void AddEdge(int j){
		PNODE u = node + edge[j].a, v = node + edge[j].b, z;
		//是否直接链接？
		if(GetRt(u) != GetRt(v)){
			Link(ecnt, j);
			ecnt ++;
			Ans += edge[j].w;
			return;
		}
		//访问路径最小
		ChangeRoot(u);
		v = access(v);
		z = v->mx;
		//换边
		if(edge[j].w > z->val){
			int lab = z - edl, del = z->emp;
			Ans -= z->val;
			Cut(lab, del);
			Link(lab, j);
			Ans += edge[j].w;
		}
	}
	//初始化
	void Init(int N){
		pcnt = N + 1;
		ecnt = 0;
		Ans = 0;
		for(int i = 0; i <= N; i ++){
			node[i] = NODE();
			edl[i] = NODE();
		}
	}
}tree;

char B[1<<15], *S=B, *T=B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
	while(ch=getc(), !isdigit(ch) && ch);
	if(!ch) return 0;
	int aa = ch - '0';
	while(ch=getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
	return aa;
}

int ans[MAXN];
int main(){
	int i, j;
	EDGE *e;
	tree.Init(100002);

	//预处理质数
	for(i = 2; i <= 100000; i ++){
		if(isnp[i])
			continue;
		for(j = i + i; j <= 100000; j += i){
			isnp[j] = true;
			edge[M].a = j / i;
			edge[M].b = j;
			edge[M].w = j / i;
			edge[M].next = head[j];
			head[j] = &edge[M ++];
		}
	}

	//预处理枚举边
	for(i = 2; i <= 100000; i ++){
		e = head[i];
		if(!e){
			//质数
			edge[M].a = 1;
			edge[M].b = i;
			edge[M].w = 1;
			tree.AddEdge(M ++);
		}else{
			//枚举因子
			for(; e; e = e->next){
				tree.AddEdge(e - edge);
			}
		}
		ans[i] = Ans;
	}

	while(N = ReadInt())
		printf("%d\n", ans[N]);

	return 0;
}
