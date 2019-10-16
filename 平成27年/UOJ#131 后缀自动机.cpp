//这是O(N)的做法
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
inline LL C2(LL x){
	return (x * (x - 1)) >> 1;
}

const int ALB = 26;
const int MAXN = 300005;
const int INF = 0x3fffffff;
const LL BASE = - 0x3fffffffffffffffll;

int N;
int arr[MAXN];
char str[MAXN];

//利用个数的前缀和以及乘积的单调性，O(N)维护
class Segtree{
public:
	LL ans1[MAXN], ans2[MAXN];
	//初始化
	void Build(){
		for(int i = 0; i <= N; i ++){
			ans1[i] = 0;
			ans2[i] = BASE;
		}
	}
	//展开答案
	void Expand(){
		LL cur = 0;
		//前缀和展开
		for(int i = 0; i < N; i ++){
			cur += ans1[i];
			ans1[i] = cur;
		}
		//逆序最大值展开
		for(int i = N - 1; i >= 0; i --){
			ans2[i] = max(ans2[i], ans2[i + 1]);
		}
	};
	//查询答案
	inline void Query(int pos, LL& ans1, LL& ans2){
		ans1 = this->ans1[pos];
		ans2 = this->ans2[pos];
	}
	//更新答案
	inline void Update(int l, int r, LL a1, LL a2){
		ans1[l] += a1;
		ans1[r + 1] -= a1;
		ans2[r] = max(ans2[r], a2);
	}
}segt;

//反插SAM成前缀，right变left
class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//当前状态max的值
		int rcnt;	//right之个数
		int mx[2];	//所有点中，最大的两个
		int mn[2];	//所有点中，最小的两个
		NODE(int val):par(NULL),val(val),rcnt(1){
			fill_n(go, ALB, (NODE*)NULL);
			fill_n(mx, 2, - INF);
			fill_n(mn, 2, INF);
		}
		NODE(){};
	}node[MAXN<<1], *last;
	int ncnt;
public:
	void Init(){
		ncnt = 1;
		last = node;
		node[0] = NODE(0);
	}
	void Append(int x, int v){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1);
		np->mx[0] = np->mn[0] = v;
		for(; p && !p->go[x]; p = p->par)
			p->go[x] = np;
		if(p){
			NODE *q = p->go[x];
			if(p->val + 1 == q->val){
				np->par = q;
			}else{
				NODE *nq = &node[ncnt ++];
				*nq = *q;
				nq->rcnt = 0;
				nq->mx[0] = - INF;
				nq->mn[0] = INF;
				nq->val = p->val + 1;
				q->par = nq;
				np->par = nq;
				for(; p && p->go[x] == q; p = p->par)
					p->go[x] = nq;
			}
		}else{
			np->par = node;
		}
		last = np;
	}
	void Deal(){
		int i, tmp;
		NODE *cur, *p;
		static int c[MAXN];
		static NODE* top[MAXN<<1];

		//SAM排序的正确写法
		for(i = 0; i < ncnt; i ++)
			c[node[i].val] ++;
		for(i = 1; i <= N; i ++)
			c[i] += c[i - 1];
		for(i = 0; i < ncnt; i ++)
			top[-- c[node[i].val]] = &node[i];
		
		for(i = ncnt - 1; i > 0; i --){
			cur = top[i];
			p = cur->par;

			p->rcnt += cur->rcnt;
			tmp = min(p->mx[0], cur->mx[0]);
			p->mx[0] = max(p->mx[0], cur->mx[0]);
			p->mx[1] = max(p->mx[1], max(cur->mx[1], tmp));

			tmp = max(p->mn[0], cur->mn[0]);
			p->mn[0] = min(p->mn[0], cur->mn[0]);
			p->mn[1] = min(p->mn[1], min(cur->mn[1], tmp));
		}

		for(i = ncnt - 1; i > 0; i --){
			if(node[i].rcnt <= 1)
				continue;
			segt.Update(node[i].par->val + 1, node[i].val, C2(node[i].rcnt), 
				max(1ll * node[i].mn[0] * node[i].mn[1], 1ll * node[i].mx[0] * node[i].mx[1]));
		}
	}
}sam;

int main(){
	int i, mx1, mx2, mn1, mn2;
	LL ans1, ans2;
	scanf("%d%s", &N, str);
	if(N <= 1){
		puts("0 0\n");
		return 0;
	}
	mx1 = mx2 = - INF;
	mn1 = mn2 = INF;
	for(i = 0; i < N; i ++){
		scanf("%d", &arr[i]);

		if(arr[i] >= mx1){
			mx2 = mx1;
			mx1 = arr[i];
		}else if(arr[i] > mx2){
			mx2 = arr[i];
		}
		if(arr[i] <= mn1){
			mn2 = mn1;
			mn1 = arr[i];
		}else if(arr[i] < mn2){
			mn2 = arr[i];
		}
	}
	sam.Init();
	segt.Build();
	for(i = N - 1; i >= 0; i --)
		sam.Append(str[i] - 'a', arr[i]);
	sam.Deal();
	segt.Expand();
	ans1 = C2(N);
	ans2 = max(1ll * mx1 * mx2, 1ll * mn1 * mn2);
	printf("%lld %lld\n", ans1, ans2);
	for(i = 1; i < N; i ++){
		segt.Query(i, ans1, ans2);
		if(ans1 > 0)
			printf("%lld %lld\n", ans1, ans2);
		else
			printf("0 0\n");
	}
	return 0;
}