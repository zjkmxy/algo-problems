//并查集是只能回退不能减的，所以莫队的时候要特殊处理：
//1.组内的查询，暴力出答案
//2.组间查询，右边界从组的右边界开始只加不减，每次到一个查询的时候，先把查询在组内的部分加上，然后退掉。每个查询在组外的部分是单调的。
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAXN = 100005;
class UnionSet{
public:
	//节点
	typedef struct NODE{
		NODE *par;
		int rank;		//秩
		bool zusammen;	//和父亲是否一致
	}*PNODE;
	NODE node[MAXN];
	//记录历史操作的栈
	struct OPERAT{
		PNODE sub, par; //孩子，父亲
		bool addrk;     //父亲的秩是否增加
		OPERAT(){};
		OPERAT(PNODE sub, PNODE par, bool addrk):sub(sub),par(par),addrk(addrk){}
	}stk[MAXN];
	int top;
	//初始化
	void Init(int N){
		for(int i = 0; i <= N; i ++){
			node[i].par = &node[i];
			node[i].rank = 0;
			node[i].zusammen = true;
		}
		top = 0;
	}
	//获得a在并查集里的根，以及深度是否为奇数(_In_out_)
	inline PNODE GetRoot(PNODE a, bool &Odd){
		for(; a->par != a; Odd = (Odd == a->zusammen), a = a->par);
		return a;
	}
	//合并a和b，返回此次操作是否安全
	bool Union(PNODE a, PNODE b){
		bool ret = true;
		a = GetRoot(a, ret);
		b = GetRoot(b, ret);
		if(a != b){
			bool addrk = false;
			if(a->rank > b->rank){
				swap(a, b);
			}else if(a->rank == b->rank){
				b->rank ++;
				addrk = true;
			}
			stk[top ++] = OPERAT(a, b, addrk);
			a->par = b;
			a->zusammen = !ret;
			return true;
		}else
			return !ret;
	}
	//Rollback直到base点
	void Regret(int base){
		while(top > base){
			top --;
			stk[top].sub->par = stk[top].sub;
			stk[top].sub->zusammen = true;
			if(stk[top].addrk)
				stk[top].par->rank --;
		}
	}
}us;

struct EDGE{
	int a, b;
}edge[MAXN];
struct QUERY{
	int id, grp;
	int l, r;
	bool operator<(const QUERY& rhs)const{
		return grp < rhs.grp || (grp == rhs.grp && r < rhs.r);
	}
}query[MAXN];
int qcnt, grpsz;
//因为有些不加到Query里面，所以Answer单算
bool ans[MAXN];
int N, M, Q;

//暴力求解
bool BF(int l, int r){
	int i;
	bool ret = true;
	us.Init(N);
	for(i = l; i <= r; i ++){
		ret = ret && us.Union(us.node + edge[i].a, us.node + edge[i].b);
	}
	return ret;
}

//成组求解
void GroupSolve(){
	int grp, i, r, l, base;
	bool ret;
	for(grp = 0; grp < qcnt; grp = i){
		us.Init(N);
		r = grpsz * (query[grp].grp + 1);
		ret = true;
		for(i = grp; i < qcnt && query[i].grp == query[grp].grp; i ++){
			while(r < query[i].r){
				++ r;
				ret = ret && us.Union(us.node + edge[r].a, us.node + edge[r].b);
			}
			if(!ret){
				ans[query[i].id] = false;
			}else{
				base = us.top;
				for(l = grpsz * (query[i].grp + 1); l >= query[i].l; l --){
					if(!us.Union(us.node + edge[l].a, us.node + edge[l].b))
						break;
				}
				ans[query[i].id] = l < query[i].l;
				us.Regret(base);
			}
		}
	}
}

int main(){
	int i;
	scanf("%d%d%d", &N, &M, &Q);
	for(i = 1; i <= M; i ++){
		scanf("%d%d", &edge[i].a, &edge[i].b);
	}
	grpsz = floor(sqrt(M));
	qcnt = 0;
	for(i = 1; i <= Q; i ++){
		scanf("%d%d", &query[qcnt].l, &query[qcnt].r);
		query[qcnt].grp = query[qcnt].l / grpsz;
		if(query[qcnt].r - query[qcnt].grp * grpsz <= grpsz){
			ans[i] = BF(query[qcnt].l, query[qcnt].r);
		}else{
			query[qcnt].id = i;
			qcnt ++;
		}
	}
	sort(query, query + qcnt);
	GroupSolve();
	for(i = 1; i <= Q; i ++){
		if(ans[i]){
			puts("Possible");
		}else{
			puts("Impossible");
		}
	}
	return 0;
}
