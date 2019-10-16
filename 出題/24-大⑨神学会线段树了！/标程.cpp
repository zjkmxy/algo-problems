#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

#define nullptr NULL
const int MAXN = 100005;
const int MOD = 9;
int arr[MAXN], N, Q;

inline void plus(int &x, int v){
	x += v;
	if(x >= MOD)
		x -= MOD;
}
class Segtree{
public:
	struct SN{
		SN *l, *r;
		int v[MOD], add;

		inline void doplus(int pv){
			int j, k, tmp[MOD];
			for(j = 0, k = pv; j < MOD; j ++, k ++){
				if(k == MOD)
					k = 0;
				tmp[j] = v[k];
			}
			for(j = 0; j < MOD; j ++)
				v[j] = tmp[j];
			plus(add, pv);
		}
		inline void popup(){
			int j;
			for(j = 0; j < MOD; j ++)
				v[j] = l->v[j] * r->v[j] % MOD;
		}
		inline void pushdown(){
			l->doplus(add);
			r->doplus(add);
			add = 0;
		}
	}sn[MAXN<<1];
	int scnt;

	SN* build(int L, int R){
		SN* rt = &sn[scnt ++];
		rt->add = 0;
		if(L == R){
			rt->v[0] = arr[L] % MOD;
			for(int j = 1; j < MOD; j ++){
				rt->v[j] = rt->v[j - 1] + 1;
				if(rt->v[j] >= MOD)
					rt->v[j] = 0;
			}
			rt->l = rt->r = nullptr;
		}else{
			int mid = (L + R) >> 1;
			rt->l = build(L, mid);
			rt->r = build(mid + 1, R);
			rt->popup();
		}
		return rt;
	}
	void update(SN* rt, int l, int r, int v, int L, int R){
		if(l == L && R == r){
			rt->doplus(v);
			return ;
		}
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
	int query(SN* rt, int l, int r, int L, int R){
		if(l == L && R == r){
			return rt->v[0];
		}
		int mid = (L + R) >> 1, ans;
		rt->pushdown();
		if(r <= mid){
			ans = query(rt->l, l, r, L, mid);
		}else if(l > mid){
			ans = query(rt->r, l, r, mid + 1, R);
		}else{
			ans = query(rt->l, l, mid, L, mid);
			ans = ans * query(rt->r, mid + 1, r, mid + 1, R) % MOD;
		}
		return ans;
	}
public:
	inline void Build(){
		scnt = 0;
		build(1, N);
	}
	inline void Update(int l, int r, int v){
		update(sn, l, r, v, 1, N);
	}
	inline int Query(int l, int r){
		return query(sn, l, r, 1, N);
	}
}segt;

char B[1<<15], *S=B, *T=B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
	while(ch=getc(), !isdigit(ch) && ch);
	if(!ch) return 0;
	int aa = ch - '0';
	while(ch=getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
	return aa;
}

int main(){
	int c, l, r, x, i;
	while(N = ReadInt()){
		Q = ReadInt();
		for(i = 1; i <= N; i ++)
			arr[i] = ReadInt();
		segt.Build();
		while(Q --){
			c = ReadInt();
			l = ReadInt();
			r = ReadInt();
			if(c == 1){
				x = ReadInt();
				segt.Update(l, r, x % MOD);
			}else if(c == 2){
				x = ReadInt();
				x = MOD - (x % MOD);
				segt.Update(l, r, x % MOD);
			}else{
				printf("%d\n", segt.Query(l, r));
			}
		}
	}
	return 0;
}