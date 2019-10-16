#include <cstdio>
#include <algorithm>
using namespace std;

#define nullptr NULL
const int MAXN = 100005;
class Treap{
private:
	struct NODE{
		NODE *l, *r;
		int fix, val, size;
		int add, mv;  //加数标记，最小值
		bool rev;     //反转标记
		NODE(int val):fix(rand()),val(val),size(1){
			l = r = nullptr;
			add = 0;
			rev = false;
			mv = val;
		};
	}*root;
	typedef pair<NODE*, NODE*> Droot;
	inline int size(NODE* cur){  //取得大小
		return cur ? cur->size : 0;
	}
	inline void pushdown(NODE* cur){
		if(cur->rev){
			if(cur->l){
				swap(cur->l->l, cur->l->r);
				cur->l->rev = !cur->l->rev;
			}
			if(cur->r){
				swap(cur->r->l, cur->r->r);
				cur->r->rev = !cur->r->rev;
			}
			cur->rev = false;
		}
		if(cur->add != 0){
			if(cur->l){
				cur->l->mv += cur->add;
				cur->l->add += cur->add;
				cur->l->val += cur->add;
			}
			if(cur->r){
				cur->r->mv += cur->add;
				cur->r->add += cur->add;
				cur->r->val += cur->add;
			}
			cur->add = 0;
		}
	}
	inline void popup(NODE* cur){
		cur->size = 1 + size(cur->l) + size(cur->r);
		cur->mv = cur->val;
		if(cur->l) cur->mv = min(cur->mv, cur->l->mv);
		if(cur->r) cur->mv = min(cur->mv, cur->r->mv);
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
		}else{
			ret = split(A->r, k - size(A->l) - 1);
			A->r = ret.first;
			popup(A);
			ret.first = A;
		}
		return ret;
	}
	NODE *build(int arr[], int N){
		static NODE *stk[MAXN], *x, *last;
		int i, sp = 0;
		for(i = 1; i <= N; i ++){
			x = new NODE(arr[i]);
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
public:
	inline void Build(int arr[], int N){
		root = build(arr, N);
	}
	void Insert(int pos, int val){
		Droot x = split(root, pos);
		NODE *p = new NODE(val);
		root = concat(x.first, concat(p, x.second));
	}
	void Delete(int pos){
		Droot x = split(root, pos - 1);
		Droot y = split(x.second, 1);
		root = concat(x.first, y.second);
	}
	void Add(int l, int r, int val){
		Droot x = split(root, l - 1);
		Droot y = split(x.second, r - l + 1);
		y.first->add += val;
		y.first->val += val;
		y.first->mv += val;
		root = concat(x.first, concat(y.first, y.second));
	}
	void Reverse(int l, int r){
		Droot x = split(root, l - 1);
		Droot y = split(x.second, r - l + 1);
		swap(y.first->l, y.first->r);
		y.first->rev = !y.first->rev;
		root = concat(x.first, concat(y.first, y.second));
	}
	int Min(int l, int r){
		Droot x = split(root, l - 1);
		Droot y = split(x.second, r - l + 1);
		int ret = y.first->mv;
		root = concat(x.first, concat(y.first, y.second));
		return ret;
	}
	void Revolve(int l, int r, int t){
		Droot a = split(root, l - 1);
		Droot b = split(a.second, r - l + 1);
		Droot c = split(b.first, r - l + 1 - t);
		root = concat(a.first, concat(concat(c.second, c.first), b.second));
	}
}tree;
int arr[MAXN], N, M;
int main(){
	int i, x, y, t;
	char op[20];
	scanf("%d", &N);
	for(i = 1; i <= N; i ++){
		scanf("%d", &arr[i]);
	}
	tree.Build(arr, N);
	scanf("%d", &M);
	while(M --){
		scanf("%s%d", op, &x);
		if(op[0] == 'A'){
			scanf("%d%d", &y, &t);
			tree.Add(x, y, t);
		}else if(op[0] == 'I'){
			scanf("%d", &t);
			tree.Insert(x, t);
		}else if(op[0] == 'D'){
			tree.Delete(x);
		}else if(op[0] == 'M'){
			scanf("%d", &y);
			printf("%d\n", tree.Min(x, y));
		}else if(op[3] == 'E'){
			scanf("%d", &y);
			tree.Reverse(x, y);
		}else{
			scanf("%d%d", &y, &t);
			t %= (y - x + 1);
			t += (y - x + 1);
			t %= (y - x + 1);
			if(t)
				tree.Revolve(x, y, t);
		}
	}
	return 0;
}