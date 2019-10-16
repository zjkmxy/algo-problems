/*
初始化一个全0的N元素数组，Q次操作如下：
1 l r x : 把[l,r]区间内的所有元素加x，但是如果加得超过100了就变成100.x不超过100
2 l r x : 把[l,r]区间内的所有元素减x，但是如果减得低于0了就变成0.
3 l r   : 求[l,r]区间内所有元素的和

思路：
块状链表套值域存个数的线段树
*/
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXC = 100;
const int MAXN = 100001;
const int MAXB = 320;
const int SIZE = 320;

//暴力求解：对拍用
namespace BF{
	int arr[MAXN], N, Q;
	void add(int l, int r, int val){
		for(int i = l; i <= r; i ++){
			arr[i] += val;
			arr[i] = min(max(arr[i], 0), 100);
		}
	}
	int sum(int l, int r){
		int ret = 0;
		for(int i = l; i <= r; i ++){
			ret += arr[i];
		}
		return ret;
	}
};

//值域求和型线段树，功能见注释
class Segtree{
	struct NODE{
		int cnt, sum;	//个数，和
		bool rst;		//清零标志
		NODE *lchd, *rchd;
	}node[MAXC * 2 + 5];
	int ncnt;
	inline void popup(NODE* cur){
		cur->sum = cur->lchd->sum + cur->rchd->sum;
		cur->cnt = cur->lchd->cnt + cur->rchd->cnt;
	}
	inline void pushdown(NODE* cur){
		if(cur->rst){
			cur->lchd->cnt = 0;
			cur->lchd->sum = 0;
			cur->lchd->rst = true;
			cur->rchd->cnt = 0;
			cur->rchd->sum = 0;
			cur->rchd->rst = true;
			cur->rst = false;
		}
	}
	NODE* build(int L, int R){
		NODE *ret = &node[ncnt ++];
		ret->sum = ret->cnt = 0;
		ret->rst = false;
		if(L < R){
			int mid = (L + R) >> 1;
			ret->lchd = build(L, mid);
			ret->rchd = build(mid + 1, R);
		}else{
			ret->lchd = ret->rchd = NULL;
		}
		return ret;
	}
	//加入val个值为pos的数
	void update(NODE* cur, int pos, int val,int L, int R){
		if(L == R){
			cur->cnt += val;
			cur->sum += val * L;
			return ;
		}
		int mid = (L + R) >> 1;
		pushdown(cur);
		if(pos <= mid){
			update(cur->lchd, pos, val, L, mid);
		}else{
			update(cur->rchd, pos, val, mid + 1, R);
		}
		popup(cur);
	}
	//将值在[l, r]内的所有数删去，返回删除的数字个数
	int erase(NODE* cur, int l, int r, int L, int R){
		int ret = 0;
		if(l == L && r == R){
			ret = cur->cnt;
			cur->cnt = cur->sum = 0;
			cur->rst = true;
			return ret;
		}
		int mid = (L + R) >> 1;
		pushdown(cur);
		if(r <= mid){
			ret += erase(cur->lchd, l, r, L, mid);
		}else if(l > mid){
			ret += erase(cur->rchd, l, r, mid + 1, R);
		}else{
			ret += erase(cur->lchd, l, mid, L, mid);
			ret += erase(cur->rchd, mid + 1, r, mid + 1, R);
		}
		popup(cur);
		return ret;
	}
public:
	//建树
	inline void Build(){
		ncnt = 0;
		build(0, MAXC);
	}
	//加入cnt个值为val的数
	inline void Insert(int val, int cnt){
		update(node, val, cnt, 0, MAXC);
	}
	//将值在[l, r]内的所有数删去，返回删除的数字个数
	inline int Erase(int l, int r){
		return erase(node, l, r, 0, MAXC);
	}
	//计算所有数字的和
	inline int GetSum(){
		return node->sum;
	}
};

//数据分配表
class DAT{
	int arr[SIZE];	//数据值
	int len;		//块大小
	int add;		//整体加减值的标记
	Segtree sumt;	//整体一段在不同数值上的个数的线段树

	//以上结构可以完成整体的操作，但是没办法force
	//为了force操作的成功完成，特加入以下元素
	int sup;	//如果arr[i] > sup，把它变成sup
	int inf;	//如果arr[i] < inf，把它变成inf
	int cover;	//把全部的arr[i]变成cover，默认-1

	//强制求值add，不操作线段树
	inline void force(){
		int i;

		//处理force标记
		if(cover >= 0){
			fill_n(arr, len, cover);
		}else{
			for(i = 0; i < len; i ++){
				if(arr[i] > sup)
					arr[i] = sup;
				if(arr[i] < inf)
					arr[i] = inf;
			}
		}
		sup = 100;
		inf = 0;
		cover = -1;

		//处理add标记
		for(i = 0; i < len; i ++){
			arr[i] = max(min(arr[i] + add, 100), 0);
		}
		add = 0;
	}
public:
	//初始化
	void Init(int len){
		//初始化查询数据
		this->len = len;
		add = 0;
		fill_n(arr, len, 0);
		sumt.Build();
		sumt.Insert(0, len);

		//初始化force标记
		sup = 100;
		inf = 0;
		cover = -1;
	}
	//内部加减数，注意l,r是内部标号
	void AddInner(int l, int r, int val){
		int i;

		sumt.Erase(0, MAXC);
		force();
		for(i = l; i <= r; i ++){
			arr[i] = max(min(arr[i] + val, 100), 0);
		}
		for(i = 0; i < len; i ++){
			sumt.Insert(arr[i], 1);
		}
	}
	//整体加减数
	void AddWhole(int val){
		int del;	//被移动的数字

		//先进行操作
		add += val;
		add = max(min(add, 100), - 100);
		if(add > 0){
			del = sumt.Erase(MAXC - add, MAXC);
			sumt.Insert(MAXC - add, del);
		}else if(add < 0){
			del = sumt.Erase(0, - add);
			sumt.Insert(- add, del);
		}

		//再处理force标记
		if(cover >= 0){
			if(add > 0){
				cover = min(cover, MAXC - add);
			}else if(add < 0){
				cover = max(cover, - add);
			}
		}else{
			if(add > 0){
				sup = min(sup, MAXC - add);
				if(sup < inf)
					cover = sup;
			}else if(add < 0){
				inf = max(inf, - add);
				if(inf > sup)
					cover = inf;
			}
		}
	}
	//整体求和
	inline int GetSum(){
		return sumt.GetSum() + add * len;
	}
	//部分求和
	inline int PartSum(int l, int r){
		int i, ans = 0;
		sumt.Erase(0, MAXC);
		force();
		for(i = l; i <= r; i ++){
			ans += arr[i];
		}
		for(i = 0; i < len; i ++){
			sumt.Insert(arr[i], 1);
		}
		return ans;
	}
};

//块状链表
class BlockList{
	DAT dat[MAXB];
	int siz;	//单块大小
	int bcnt;	//块数
	int N;		//[1..N]
public:
	void Init(int N){
		this->N = N;
		siz = static_cast<int>(floor(sqrt(N)));
		bcnt = N / siz + 1;
		for(int i = 0; i < bcnt; i ++)
			dat[i].Init(siz);
	}
	void Add(int l, int r, int val){
		int bl = l / siz, br = r / siz;
		int pl = l % siz, pr = r % siz;
		if(bl == br){
			dat[bl].AddInner(pl, pr, val);
		}else{
			for(int i = bl + 1; i < br; i ++)
				dat[i].AddWhole(val);
			dat[bl].AddInner(pl, siz - 1, val);
			dat[br].AddInner(0, pr, val);
		}
	}
	int GetSum(int l, int r){
		int bl = l / siz, br = r / siz;
		int pl = l % siz, pr = r % siz;
		if(bl == br){
			return dat[bl].PartSum(pl, pr);
		}else{
			int ans = 0;
			for(int i = bl + 1; i < br; i ++){
				ans += dat[i].GetSum();
			}
			ans += dat[bl].PartSum(pl, siz - 1);
			ans += dat[br].PartSum(0, pr);
			return ans;
		}
	}
}bl;

int main(){
	int N, Q, c, x, y, z;
	while(scanf("%d%d", &N, &Q) != EOF){
		bl.Init(N);
		while(Q --){
			scanf("%d%d%d", &c, &x, &y);
			if(c == 1){
				scanf("%d", &z);
				bl.Add(x, y, z);
			}else if(c == 2){
				scanf("%d", &z);
				bl.Add(x, y, - z);
			}else{
				printf("%d\n", bl.GetSum(x, y));
			}
		}
	}
	return 0;
}