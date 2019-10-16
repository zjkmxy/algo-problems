//为保证字典序最优，需要倒着做
#include <cstdio>
#include <set>
#include <list>
#include <algorithm>
#include <cctype>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 50005;

int N;
struct NODE{
	int i, l, r;
	int dpl, pre;	//dpl是长度，pre是前向
}arr[MAXN];
int ans[MAXN];

inline void update(NODE& p, int len, int pre){
	if(p.dpl < len || p.dpl == len && p.pre < pre){
		p.dpl = len;
		p.pre = pre;
	}
}

class Segtree{
public:
	NODE node[MAXN<<2];
	void build(int id, int l, int r){
		node[id].dpl = 0;
		node[id].pre = -1;
		if(l < r){
			int mid = (l + r) >> 1;
			build(id << 1, l, mid);
			build(id << 1 | 1, mid + 1, r);
		}
	}
	void upd(int id, int pos, int dpl, int pre, int l, int r){
		if(l == r){
			update(node[id], dpl, pre);
			return ;
		}
		int mid = (l + r) >> 1;
		if(pos <= mid){
			upd(id << 1, pos, dpl, pre, l, mid);
		}else{
			upd(id << 1 | 1, pos, dpl, pre, mid + 1, r);
		}
		node[id].dpl = 0;
		node[id].pre = -1;
		update(node[id], node[id << 1].dpl, node[id << 1].pre);
		update(node[id], node[id << 1 | 1].dpl, node[id << 1 | 1].pre);
	}
	void query(int id, int ql, int qr, NODE& ans, int l, int r){
		if(l == ql && r == qr){
			update(ans, node[id].dpl, node[id].pre);
			return ;
		}
		int mid = (l + r) >> 1;
		if(qr <= mid){
			query(id << 1, ql, qr, ans, l, mid);
		}else if(ql > mid){
			query(id << 1 | 1, ql, qr, ans, mid + 1, r);
		}else{
			query(id << 1, ql, mid, ans, l, mid);
			query(id << 1 | 1, mid + 1, qr, ans, mid + 1, r);
		}
	}
	void clean(int id, int pos, int l, int r){
		if(l == r){
			node[id].dpl = 0;
			node[id].pre = -1;
			return ;
		}
		int mid = (l + r) >> 1;
		if(pos <= mid){
			clean(id << 1, pos, l, mid);
		}else{
			clean(id << 1 | 1, pos, mid + 1, r);
		}
		node[id].dpl = 0;
		node[id].pre = -1;
		update(node[id], node[id << 1].dpl, node[id << 1].pre);
		update(node[id], node[id << 1 | 1].dpl, node[id << 1 | 1].pre);
	}
}sgt;

struct DTA{
	int r;
	int id;
	bool operator<(const DTA& rhs)const{
		if(r != rhs.r)
			return r < rhs.r;
		else
			return id < rhs.id;
	}
}dta[MAXN];	//solve用
void solve(int l, int r){
	int mid = (l + r) >> 1;
	if(l == r){
		update(arr[l], 1, -1);
		return ;
	}
	solve(l, mid);

	NODE ans;
	int dn = 0;	//dta大小
	for(int i = l; i <= r; i ++){
		dta[dn].id = i;
		dta[dn].r = arr[i].r;
		dn ++;
	}
	sort(dta, dta + dn);
	for(int i = 0; i < dn; i ++){
		int cur = dta[i].id;
		if(cur <= mid){
			sgt.upd(1, arr[cur].l, arr[cur].dpl, cur, 0, N);	//这里传入pre为位置
		}else{
			ans.dpl = 0;
			ans.pre = -1;
			sgt.query(1, 0, arr[cur].l, ans, 0, N);
			update(arr[cur], ans.dpl + 1, ans.pre);	//这里传入dpl+1，pre为位置
		}
	}
	for(int i = 0; i < dn; i ++){
		int cur = dta[i].id;
		if(cur <= mid){
			sgt.clean(1, arr[cur].l, 0, N);
		}
	}

	solve(mid + 1, r);
}

bool domain(){
	int i, anslen, anspos;
	static int dist[MAXN], disn;

	//读入，按i排序
	if(scanf("%d", &N) == EOF)
		return false;
	for(i = N; i >= 1; i --){
		arr[i].i = i;
		scanf("%d", &arr[i].l);
	}
	for(i = N; i >= 1; i --){
		scanf("%d", &arr[i].r);
		arr[i].dpl = 0;
		arr[i].pre = -1;
		arr[i].r = 1000000001 - arr[i].r;
	}

	//离散化
	for(i = 1; i <= N; i ++){
		dist[i - 1] = arr[i].l;
	}
	sort(dist, dist + N);
	disn = unique(dist, dist + N) - dist;
	for(i = 1; i <= N; i ++){
		arr[i].l = lower_bound(dist, dist + disn, arr[i].l) - dist;
	}
	for(i = 1; i <= N; i ++){
		dist[i - 1] = arr[i].r;
	}
	sort(dist, dist + N);
	disn = unique(dist, dist + N) - dist;
	for(i = 1; i <= N; i ++){
		arr[i].r = lower_bound(dist, dist + disn, arr[i].r) - dist;
	}

	//求解
	sgt.build(1, 0, N);
	solve(1, N);
	anslen = 0;
	for(i = 1; i <= N; i ++){
		if(arr[i].dpl >= anslen){
			anslen = arr[i].dpl;
			anspos = i;
		}
	}
	for(i = 1; i <= anslen; i ++, anspos = arr[anspos].pre){
		ans[i] = anspos;
	}
	printf("%d\n", anslen);
	for(i = 1; i <= anslen; i ++){
		printf("%d", N + 1 - ans[i]);
		if(i == anslen){
			printf("\n");
		}else{
			printf(" ");
		}
	}

	return true;
}

int main(){

	freopen("1009.in", "r", stdin);
	freopen("1009.my", "w", stdout);

	while(domain());
	return 0;
}
