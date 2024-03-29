#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 1000005;
int arr[MAXN], N, Q;

class Segtree{
	struct NODE{
		int len;	//区间长度
		NODE *l, *r;//左右孩子
		int mv;		//最大值
		LL sum;		//和

		//是否是脏节点
		//（貌似我这里脏和纯的定义和大部分题是反的……改不了了，抱歉）
		//脏节点 = 需要下传mv的修改节点
		//脏节点 = 数据独立维护的节点
		//纯节点 = 通过孩子来维护mv和sum的节点
		//纯节点 = 以后要被最近的脏祖先所更新的节点
		bool dirty;
		//最大脏值
		//就是子树内（不算自己）最大的脏节点的mv
		//当每次更新小于mv而大于最大脏值的时候，我们是可以直接更新mv的
		int dirtymax;
		//纯长度，就是对于这个节点来说，它子树内所有纯节点的总个数
		//如果是脏节点，那么  纯长度=被脏节点支配的个数
		int purelen;

		//上传和与最值
		inline void popup(){
			mv = max(l->mv, r->mv);
			sum = l->sum + r->sum;

			purelen = 0;
			dirtymax = max(l->dirtymax, r->dirtymax);
			if(l->dirty)
				dirtymax = max(dirtymax, l->mv);
			else
				purelen += l->purelen;
			if(r->dirty)
				dirtymax = max(dirtymax, r->mv);
			else
				purelen += r->purelen;
			dirty = false;
		}
		//下传标记
		inline void pushdown(){
			if(dirty){
				if(l->mv >= mv){
					l->dirty = true;
					l->sum -= LL(l->mv) * l->purelen;
					l->mv = mv;
					l->sum += LL(l->mv) * l->purelen;
				}
				if(r->mv >= mv){
					r->dirty = true;
					r->sum -= LL(r->mv) * r->purelen;
					r->mv = mv;
					r->sum += LL(r->mv) * r->purelen;
				}
			}
		}
		//纯化mv比v大脏节点为v的纯节点
		//好吧我不知道正确的说法是什么……
		void purify(int v){
			//无意义的操作
			if(mv <= v)
				return;
			if(v >= dirtymax){
				//没有新的纯度变化
				sum -= LL(mv) * purelen;
				mv = v;
				sum += LL(mv) * purelen;
				dirty = false;
			}else{
				//左右孩子需要纯化
				l->purify(v);
				r->purify(v);
				popup();
			}
		}
	}node[MAXN<<1];
	int ncnt;

	//建树，常规操作
	NODE* build(int l, int r){
		NODE* ret = &node[ncnt ++];
		ret->len = r - l + 1;
		if(l == r){
			//注意一开始当且仅当叶子节点是脏的
			//此后叶子节点也永远是脏的
			ret->l = ret->r = NULL;
			ret->dirty = true;
			ret->sum = ret->mv = arr[l];
			ret->dirtymax = 0;
			ret->purelen = 1;
		}else{
			int mid = (l + r) >> 1;
			ret->l = build(l, mid);
			ret->r = build(mid + 1, r);
			ret->popup();
		}
		return ret;
	}
	//查询，常规操作
	pair<int, LL> query(NODE* rt, int l, int r, int L, int R){
		if(l == L && R == r){
			return make_pair(rt->mv, rt->sum);
		}
		int mid = (L + R) >> 1;
		rt->pushdown();
		rt->popup();	//这里需要更新dirty
		if(r <= mid){
			return query(rt->l, l, r, L, mid);
		}else if(l > mid){
			return query(rt->r, l, r, mid + 1, R);
		}else{
			pair<int, LL> r1 = query(rt->l, l, mid, L, mid);
			pair<int, LL> r2 = query(rt->r, mid + 1, r, mid + 1, R);
			return make_pair(max(r1.first, r2.first), r1.second + r2.second);
		}
	}
	//更新，常规操作
	void update(NODE* rt, int l, int r, int v, int L, int R){
		//无意义操作
		if(rt->mv <= v)
			return ;
		if(l == L && R == r){
			//因为是修改这个节点，所以先纯化，再标脏
			rt->purify(v);
			rt->dirty = true;
		}else{
			int mid = (L + R) >> 1;
			rt->pushdown();
			if(r <= mid){
				update(rt->l, l, r, v, L, mid);
			}else if(l > mid){
				update(rt->r, l, r, v, mid + 1, R);
			}else{
				update(rt->l, l, mid, v, L, mid);
				update(rt->r, mid + 1, r, v, mid + 1, R);
			}
			rt->popup();
		}
	}
public:
	//以下三个是对外接口
	inline void Build(){
		ncnt = 0;
		build(1, N);
	}
	inline void Update(int l, int r, int v){
		update(node, l, r, v, 1, N);
	}
	inline pair<int, LL> Query(int l, int r){
		return query(node, l, r, 1, N);
	}
}segt;

//读入挂
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

int main(){
	int T, i, op, l, r;
	T = ReadInt();
	while(T --){
		N = ReadInt(), Q = ReadInt();
		for(i = 1; i <= N; i ++){
			arr[i] = ReadInt();
		}
		segt.Build();
		while(Q --){
			op = ReadInt(), l = ReadInt(), r = ReadInt();
			if(op == 0){
				i = ReadInt();
				segt.Update(l, r, i);
			}else{
				pair<int, LL> ret = segt.Query(l, r);
				if(op == 1){
					printf("%d\n", ret.first);
				}else{
					printf("%I64d\n", ret.second);
				}
			}
		}
	}
	return 0;
}
