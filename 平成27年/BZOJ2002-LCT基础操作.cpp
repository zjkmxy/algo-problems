/**************************************************************
Problem: 2002
User: zjkmxy
Language: C++
Result: Accepted
Time:2108 ms
Memory:7064 kb
****************************************************************/

#include <cstdio>
#include <algorithm>
using namespace std;

#define nullptr NULL
const int MAXN = 200005;
class Treap{
private:
	typedef struct NODE{
		NODE *l, *r, *par;  //左，右，父亲
		bool root;          //是否为根（可以用函数查）
		int fix, size;
		NODE():fix(rand()),size(1){
			par = l = r = nullptr;
			root = true;
		};
	}*PNODE;
	PNODE root;
	NODE node[MAXN];
	int pcnt;
	typedef pair<PNODE, PNODE> Droot;
	inline int size(PNODE cur){  //取得大小
		return cur ? cur->size : 0;
	}
	inline void pushdown(PNODE cur){}
	inline void popup(PNODE cur){
		cur->size = 1;
		if(cur->l){
			cur->l->root = false;
			cur->l->par = cur;
			cur->size += cur->l->size;
		}
		if(cur->r){
			cur->r->root = false;
			cur->r->par = cur;
			cur->size += cur->r->size;
		}
	}
	void getrank(PNODE A, PNODE& rt, int& rk){
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
	void Change(int a, int p){
		PNODE A = &node[a], rt;
		int rk;
		access(A);
		getrank(A, rt, rk);
		rt = split(rt, rk - 1).second;
		rt->par = node + p;
	}
	void Init(int *arr, int N){
		pcnt = N + 1;
		for(int i = 1; i <= N; i ++)
			node[i].par = node + arr[i];
	}
	int Query(int a){
		return access(node + a)->size - 1;
	}
}tree;
int arr[MAXN], N, M;
int main(){
	int i, j, k;
	scanf("%d", &N);
	for(i = 1; i <= N; i ++){
		scanf("%d", &arr[i]);
		arr[i] += i;
		if(arr[i] > N)
			arr[i] = 0;
	}
	tree.Init(arr, N);
	scanf("%d", &M);
	while(M --){
		scanf("%d%d", &i, &j);
		j ++;
		if(i == 1){
			printf("%d\n", tree.Query(j));
		}else{
			scanf("%d", &k);
			k += j;
			if(k > N)
				k = 0;
			if(arr[j] == k)
				continue;
			arr[j] = k;
			tree.Change(j, k);
		}
	}
	return 0;
}