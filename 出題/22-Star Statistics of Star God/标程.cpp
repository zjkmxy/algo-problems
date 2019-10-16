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
int N, Q, K;

int comp(const void* A, const void* B){
	ANS *a = (ANS*)A, *b = (ANS*)B;
	return (a->id - b->id);
}

//平衡树
class Tree{
public:
	RANK node[MAXN];
	set<RANK> q;
	void Init(int N){
		for(int i = 0; i <= N; i ++){
			node[i].pos = i;
			node[i].cnt = 0;
			node[i].sum = 0;
		}
		q.clear();
	}
	void Update(int pos, int cnt, int val){
		if(node[pos].cnt > 0)
			q.erase(node[pos]);
		node[pos].cnt += cnt;
		node[pos].sum += val;
		if(node[pos].cnt > 0)
			q.insert(node[pos]);
	}
	void Query(ANS& ans){
		set<RANK>::iterator it;
		it = q.begin();
		for(int i = 0; i < 3; i ++){
			if(it == q.end()){
				ans.pos[i] = -1;
			}else{
				ans.pos[i] = it->pos;
				ans.val[i] = double(it->sum) / double(it->cnt);
				it ++;
			}
		}
	}
} tree;

//离散化其实是可有可无的
//只要块标号是对的，就没问题
void Distinct(){
	int i, v, t;
	sort(arr + 1, arr + N + 1);
	t = arr[N].tim;
	v = sqrt(t);
	for(i = 0; i < Q; i ++){
		ans[i].blk = ans[i].l / v;
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
	int L = 1, R = 0, i;
	for(i = 0; i < Q; i ++){
		for(; R < N && arr[R+1].tim <= ans[i].r; R ++)
			tree.Update(arr[R+1].pos, 1, arr[R+1].val);
		for(; R >= 0 && arr[R].tim > ans[i].r; R --)
			tree.Update(arr[R].pos, -1, -arr[R].val);
		for(; L <= N && arr[L].tim < ans[i].l; L ++)
			tree.Update(arr[L].pos, -1, -arr[L].val);
		for(; L > 1 && arr[L-1].tim >= ans[i].l; L --)
			tree.Update(arr[L-1].pos, 1, arr[L-1].val);
		tree.Query(ans[i]);
	}
}

int main(){
	Input();
	Distinct();
	Solve();
	Print();
	return 0;
}