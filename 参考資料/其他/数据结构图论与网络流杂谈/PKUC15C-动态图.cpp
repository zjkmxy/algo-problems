#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
typedef long long LL;

//并查集
class UnionSet{
public:
	//节点
	typedef struct NODE{
		NODE *par;
		int rank;  //秩
		int siz;   //子集大小
	}*PNODE;
	NODE node[MAXN];
	//记录历史操作的栈
	struct OPERAT{
		PNODE sub, par; //孩子，父亲
		int siz;        //孩子的大小
		bool addrk;     //父亲的秩是否增加
		OPERAT(){};
		OPERAT(PNODE sub, PNODE par, int siz, bool addrk):sub(sub),par(par),siz(siz),addrk(addrk){}
	}stk[MAXN];
	int top;
	//初始化
	void Init(int N){
		for(int i = 0; i <= N; i ++){
			node[i].par = &node[i];
			node[i].rank = 0;
			node[i].siz = 1;
		}
		top = 0;
	}
	//获得a在并查集里的根
	inline PNODE GetRoot(PNODE a){
		while(a->par != a)
			a = a->par;
		return a;
	}
	//合并a和b，返回合并产生的可达顶点对数
	LL Union(PNODE a, PNODE b){
		a = GetRoot(a);
		b = GetRoot(b);
		if(a != b){
			LL ret = (LL)(a->siz) * (LL)(b->siz);
			bool addrk = false;
			if(a->rank > b->rank){
				swap(a, b);
			}else if(a->rank == b->rank){
				b->rank ++;
				addrk = true;
			}
			stk[top ++] = OPERAT(a, b, a->siz, addrk);
			a->par = b;
			b->siz += a->siz;
			a->siz = 0;
			return ret;
		}else
			return 0;
	}
	//Rollback直到base点
	void Regret(int base){
		while(top > base){
			top --;
			stk[top].sub->par = stk[top].sub;
			stk[top].sub->siz = stk[top].siz;
			stk[top].par->siz -= stk[top].siz;
			if(stk[top].addrk)
				stk[top].par->rank --;
		}
	}
}us;
//边的集合
struct EDGE{
	UnionSet::PNODE a, b;  //所连顶点
	int tim;   //阻塞时间
	bool operator<(const EDGE& rhs)const{
		return tim < rhs.tim;
	};
}edge[MAXM];
int N, M, K;
bool Input(){
	int a, b, i;
	if(scanf("%d%d%d", &N, &M, &K) == EOF)
		return false;
	for(i = 0; i < M; i ++){
		scanf("%d%d%d", &a, &b, &edge[i].tim);
		edge[i].a = us.node + a;
		edge[i].b = us.node + b;
	}
	sort(edge, edge + M);
	return true;
}
//对时间区间分治解决
//左时间边界，右时间边界，预先加好的答案，egde的左边界，edge的右边界
void DivAndConq(int l, int r, LL pre, int el, int er){
	//递归终点
	if(l == r){
		printf("%lld\n", pre);
		return ;
	}
	const int mid = (l + r) >> 1, base = us.top;
	int i;
	LL ans;
	//把时间超过mid的边都合并，然后递归处理左孩子
	for(ans = pre, i = er; i >= el && edge[i].tim > mid; i --)
		ans += us.Union(edge[i].a, edge[i].b);
	DivAndConq(l, mid, ans, el, i);
	us.Regret(base);
	//处理右孩子
	for(ans = pre, i = el; i <= er && edge[i].tim <= mid; i ++)
		ans += us.Union(edge[i].a, edge[i].b);
	DivAndConq(mid + 1, r, ans, i, er);
	us.Regret(base);
}
void Solve(){
	LL ans = 0;
	int ep;
	us.Init(N);
	for(ep = M - 1; ep >= 0 && edge[ep].tim > K; ep --)
		ans += us.Union(edge[ep].a, edge[ep].b);
	DivAndConq(1, K, ans, 0, ep);
}
int main(){
	while(Input())
		Solve();
	return 0;
}