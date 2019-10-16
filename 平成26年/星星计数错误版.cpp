#include <cstdio>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

const int MAXN = 131072;
typedef long long LL;
struct RANK{
	int pos, cnt, sum;
	bool operator<(const RANK& rhs)const{
		LL val = LL(sum) * LL(rhs.cnt) - LL(rhs.sum) * LL(cnt);
		if(val != 0){
			return val > 0;
		}else if(cnt != rhs.cnt){
			return cnt > rhs.cnt;
		}else{
			return pos < rhs.pos;
		}
	}
};
struct DATA{
	int tim, pos, val;
	bool operator<(const DATA& rhs)const{
		return tim < rhs.tim;
	}
}arr[MAXN];
struct ANS{
	int id;
	int l, r, blk;
	int pos[3];
	double val[3];
	bool operator<(const ANS& rhs)const{
		if(blk != rhs.blk)
			return blk < rhs.blk;
		else
			return r < rhs.r;
	}
}ans[MAXN];
int N, Q, K, V;

int comp(const void* A, const void* B){
	ANS *a = (ANS*)A, *b = (ANS*)B;
	return (a->id - b->id);
}

//单调栈
//答案是不对的，因为平均值会被拉低，而不仅仅是拉高
class Tree{
public:
	RANK node[MAXN];
	int mx[3];	//前三的排名
	int tim;	//时间戳
	int old[MAXN][3];	//旧的mx

	void Init(int N){
		for(int i = 0; i <= N; i ++){
			node[i].pos = i;
			node[i].cnt = 0;
			node[i].sum = 0;
		}
		mx[0] = mx[1] = mx[2] = 0;
		old[0][0] = old[0][1] = old[0][2] = 0;
		tim = 0;
	}
	void Update(int pos, int val){
		node[pos].cnt ++;
		node[pos].sum += val;
		
		if(pos != mx[0] && pos != mx[1] && pos != mx[2] && node[pos] < node[mx[2]]){
			mx[2] = pos;
		}
		if(node[mx[2]] < node[mx[1]]){
			swap(mx[2], mx[1]);
		}
		if(node[mx[1]] < node[mx[0]]){
			swap(mx[1], mx[0]);
		}
		if(node[mx[2]] < node[mx[1]]){
			swap(mx[2], mx[1]);
		}

		copy(mx, mx + 3, old[++ tim]);
	}
	void Regret(int pos, int val){
		node[pos].cnt --;
		node[pos].sum -= val;

		-- tim;
		copy(old[tim], old[tim] + 3, mx);
	}
	void Query(ANS& ans){
		for(int i = 0; i < 3; i ++){
			if(mx[i] == 0){
				ans.pos[i] = -1;
			}else{
				ans.pos[i] = mx[i];
				ans.val[i] = double(node[mx[i]].sum) / double(node[mx[i]].cnt);
			}
		}
	}
} tree;

void Distinct(){
	int i;
	static int ps[MAXN];

	sort(arr + 1, arr + N + 1);
	for(i = 1; i <= N; i ++){
		ps[i] = arr[i].tim;
	}
	ps[N + 1] = 0x3fffffff;
	for(i = 0; i < Q; i ++){
		ans[i].l = lower_bound(ps + 1, ps + N + 2, ans[i].l) - ps;
		ans[i].r = upper_bound(ps + 1, ps + N + 2, ans[i].r) - ps - 1;
	}

	V = int(sqrt(N));
	for(i = 0; i < Q; i ++){
		ans[i].blk = ans[i].l / V;
	}
	sort(ans, ans + Q);
}

void Print(){
	int i, j;
	qsort(ans, Q, sizeof(ANS), comp);
	for(i = 0; i < Q; i ++){
		for(j = 0; j < 3; j ++){
			if(ans[i].pos[j] > 0){
				printf("%d/%.2lf;", ans[i].pos[j], ans[i].val[j]);
			}else{
				printf("N/A;");
			}
		}
		printf("\n");
	}
}

void Input(){
	int i;
	scanf("%d%d%d", &N, &Q, &K);
	tree.Init(K);
	for(i = 1; i <= N; i ++){
		scanf("%d%d%d", &arr[i].tim, &arr[i].pos, &arr[i].val);
	}
	for(i = 0; i < Q; i ++){
		scanf("%d%d", &ans[i].l, &ans[i].r);
		ans[i].id = i;
	}
}

void Solve(){
	int st, i, j, rp, bp;
	for(st = 0; st < Q; st = i){
		tree.Init(K);
		//区块内暴力
		for(i = st; i < Q && ans[i].r - ans[i].l <= V; i ++){
			for(j = ans[i].l; j <= ans[i].r; j ++){
				tree.Update(arr[j].pos, arr[j].val);
			}
			tree.Query(ans[i]);
			for(j = ans[i].r; j >= ans[i].l; j --){
				tree.Regret(arr[j].pos, arr[j].val);
			}
		}
		//区块间莫队
		bp = rp = (ans[i].blk + 1) * V;
		for(; i < Q && ans[i].blk == ans[st].blk; i ++){
			while(ans[i].r > rp){
				++ rp;
				tree.Update(arr[rp].pos, arr[rp].val);
			}
			for(j = bp; j >= ans[i].l; j --){
				tree.Update(arr[j].pos, arr[j].val);
			}
			tree.Query(ans[i]);
			for(j = ans[i].l; j <= bp; j ++){
				tree.Regret(arr[j].pos, arr[j].val);
			}
		}
	}
}

int main(){
	Input();
	Distinct();
	Solve();
	Print();
	return 0;
}
