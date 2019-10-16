#include <cstdio>
#include <algorithm>
using namespace std;

#define nullptr NULL
const int MAXN = 200005;
class Treap{
private:
	struct NODE{
		NODE *l, *r, *par;	//左，右，父亲
		bool root;			//是否为根
		int fix, val, size;
		int mv;  //最大值
		NODE(int val = 0):fix(rand()),val(val),size(1){
			par = l = r = nullptr;
			root = true;
			mv = val;
		};
	}*root;
	NODE pool[MAXN];
	int pcnt;
	typedef pair<NODE*, NODE*> Droot;
	inline int size(NODE* cur){  //取得大小
		return cur ? cur->size : 0;
	}
	inline void pushdown(NODE* cur){}
	inline void popup(NODE* cur){
		cur->size = 1;
		cur->mv = cur->val;
		if(cur->l){
			cur->l->root = false;
			cur->l->par = cur;
			cur->size += cur->l->size;
			cur->mv = max(cur->mv, cur->l->mv);
		}
		if(cur->r){
			cur->r->root = false;
			cur->r->par = cur;
			cur->size += cur->r->size;
			cur->mv = max(cur->mv, cur->r->mv);
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
	//Droot split(NODE *A, int k){  //分裂:(前k个,剩下的)
	//	if(!A) return Droot(nullptr, nullptr);
	//	Droot ret;
	//	pushdown(A);
	//	if(size(A->l) >= k){
	//		ret = split(A->l, k);
	//		A->l = ret.second;
	//		popup(A);
	//		ret.second = A;
	//	}else{
	//		ret = split(A->r, k - size(A->l) - 1);
	//		A->r = ret.first;
	//		popup(A);
	//		ret.first = A;
	//	}
	//	return ret;
	//}
	Droot split(int pos){
		if(pos <= 0 || pos >= pcnt)
			return Droot(nullptr, nullptr);
		NODE *A = &pool[pos];
		Droot ret(A, A->r);
		NODE *tmp;
		if(A->r){
			A->r->root = true;
			A->r->par = nullptr;
			A->r = nullptr;
		}
		popup(A);
		while(!A->root){
			tmp = A->par;
			if(A == tmp->l){
				tmp->l = ret.second;
				ret.second = tmp;
				popup(tmp);
				A->par = tmp->par;
				A->root = tmp->root;
				tmp->par = nullptr;
				tmp->root = true;
				if(!A->root){
					if(A->par->l == tmp){
						A->par->l = A;
					}else{
						A->par->r = A;
					}
				}
			}else{
				ret.first = A = tmp;
				popup(A);
			}
		}
		return ret;
	}
	NODE *build(int arr[], int N){
		static NODE *stk[MAXN], *x, *last;
		int i, sp = 0;
		pcnt = 1;
		for(i = 1; i <= N; i ++){
			x = &pool[pcnt ++];
			*x = NODE(arr[i]);
			last = nullptr;
			while(sp && stk[sp]->fix > x->fix){
				popup(stk[sp]);
				last = stk[sp];
				sp --;
			}
			if(sp)
				stk[sp]->r = x;
			x->l = last;
			stk[++ sp] = x;
		}
		while(sp)
			popup(stk[sp --]);
		return stk[1];
	}
	void update(NODE* cur, int k, int val){
		int sz = size(cur->l);
		if(sz == k - 1){
			cur->val = val;
			popup(cur);
		}else if(sz < k - 1){
			update(cur->r, k - sz - 1, val);
			popup(cur);
		}else{
			update(cur->l, k, val);
			popup(cur);
		}
	}
public:
	inline void Build(int arr[], int N){
		root = build(arr, N);
	}
	int Max(int l, int r){
		//Droot x = split(root, l - 1);
		//Droot y = split(x.second, r - l + 1);
		Droot x = split(l - 1);
		Droot y = split(r);
		int ret = y.first->mv;
		root = concat(x.first, concat(y.first, y.second));
		return ret;
	}
	inline void Update(int k, int val){
		update(root, k, val);
	}
}tree;
int arr[MAXN], N, M;
int main(){
	char op[3];
	int a, b;
	while(scanf("%d%d", &N, &M)!=EOF){
		for(a = 1; a <= N; a ++){
			scanf("%d", &arr[a]);
		}
		tree.Build(arr, N);
		while(M --){
			scanf("%s%d%d", op, &a, &b);
			if(op[0] == 'Q'){
				printf("%d\n", tree.Max(a, b));
			}else{
				tree.Update(a, b);
			}
		}
	}
	return 0;
}
