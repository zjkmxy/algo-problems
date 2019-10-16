//利用单调性避免值域维护
#include <cstdio>
#include <set>
#include <list>
#include <algorithm>
#include <cctype>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 100005;
const int INF = 0x3fffffff;

pair<int, int> pos[MAXN];
int N, M, X1, Y1, X2, Y2;

class Segtree{
public:
	struct NODE{
		int mx, my;
		int cnt;
		int addx, addy;
		NODE *lchd, *rchd;
		inline void pushdown(){
			if(addx > 0){
				if(lchd->cnt > 0){
					lchd->addx += addx;
					lchd->mx += addx;
				}
				if(rchd->cnt > 0){
					rchd->addx += addx;
					rchd->mx += addx;
				}
			}
			if(addy > 0){
				if(lchd->cnt > 0){
					lchd->addy += addy;
					lchd->my += addy;
				}
				if(rchd->cnt > 0){
					rchd->addy += addy;
					rchd->my += addy;
				}
			}
			addx = addy = 0;
		}
		inline void popup(){
			mx = max(lchd->mx, rchd->mx);
			my = max(lchd->my, rchd->my);
			cnt = lchd->cnt + rchd->cnt;
		}
	}node[MAXN<<1];
	int ncnt, cx, cy, N;

	NODE* build(int l, int r){
		NODE* ret = &node[ncnt ++];
		ret->addx = ret->addy = 0;
		if(l == r){
			ret->mx = pos[l].first;
			ret->my = pos[l].second;
			ret->cnt = (ret->mx <= cx && ret->my <= cy) ? 1 : 0;
			if(ret->cnt == 0)
				ret->mx = ret->my = - INF;
			ret->lchd = ret->rchd = nullptr;
		}else{
			int mid = (l + r) >> 1;
			ret->lchd = build(l, mid);
			ret->rchd = build(mid + 1, r);
			ret->popup();
		}
		return ret;
	}
	void restri(NODE* cur, int l, int r){
		while(true){
			if(cur->mx <= cx && cur->my <= cy)
				return ;
			if(l == r){
				cur->mx = cur->my = - INF;
				cur->cnt = 0;
			}else{
				int mid = (l + r) >> 1;
				cur->pushdown();
				restri(cur->lchd, l, mid);
				restri(cur->rchd, mid + 1, r);
				cur->popup();
			}
		}
	}
	void add(NODE* cur, int l, int r, int d, bool ax, int L, int R){
		if(l == L && r == R){
			if(ax){
				cur->addx += d;
				cur->mx += d;
			}else{
				cur->addy += d;
				cur->my += d;
			}
			restri(cur, L, R);
			return ;
		}
		int mid = (L + R) >> 1;
		cur->pushdown();
		if(r <= mid){
			add(cur->lchd, l, r, d, ax, L, mid);
		}else if(l > mid){
			add(cur->rchd, l, r, d, ax, mid + 1, R);
		}else{
			add(cur->lchd, l, mid, d, ax, L, mid);
			add(cur->rchd, mid + 1, r, d, ax, mid + 1, R);
		}
		cur->popup();
	}
	int query(NODE* cur, int l, int r, int L, int R){
		if(l == L && r == R){
			return cur->cnt;
		}
		int mid = (L + R) >> 1;
		cur->pushdown();
		if(r <= mid){
			return query(cur->lchd, l, r, L, mid);
		}else if(l > mid){
			return query(cur->rchd, l, r, mid + 1, R);
		}else{
			return query(cur->lchd, l, mid, L, mid) + query(cur->rchd, mid + 1, r, mid + 1, R);
		}
	}

	inline void Build(int N, int cx, int cy){
		this->N = N, this->cx = cx, this->cy = cy;
		ncnt = 0;
		build(1, N);
	}
	inline void Add(int l, int r, int v, bool ax){
		add(node, l, r, v, ax, 1, N);
	}
	inline int Query(int l, int r){
		return query(node, l, r, 1, N);
	}
}t1, t2, t3, t4;

int main(){
	int T, i, op, l, r, d;
	scanf("%d", &T);
	while(T --){
		scanf("%d%d%d%d%d", &N, &X1, &Y1, &X2, &Y2);
		for(i = 1; i <= N; i ++){
			scanf("%d%d", &l, &r);
			pos[i] = make_pair(l, r);
		}
		t1.Build(N, X2, Y2);
		t2.Build(N, X2, Y1 - 1);
		t3.Build(N, X1 - 1, Y2);
		t4.Build(N, X1 - 1, Y1 - 1);
		scanf("%d", &M);
		while(M --){
			scanf("%d%d%d", &op, &l, &r);
			if(op == 1){
				scanf("%d", &d);
				t1.Add(l, r, d, true);
				t2.Add(l, r, d, true);
				t3.Add(l, r, d, true);
				t4.Add(l, r, d, true);
			}else if(op == 2){
				scanf("%d", &d);
				t1.Add(l, r, d, false);
				t2.Add(l, r, d, false);
				t3.Add(l, r, d, false);
				t4.Add(l, r, d, false);
			}else if(op == 3){
				int v1, v2, v3, v4;
				v1 = t1.Query(l, r);
				v2 = t2.Query(l, r);
				v3 = t3.Query(l, r);
				v4 = t4.Query(l, r);
				printf("%d\n", v1 - v2 - v3 + v4);
			}
		}
	}
	return 0;
}