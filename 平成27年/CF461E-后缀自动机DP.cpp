#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;
const int ALB = 4;
const int MAXN = 100005;
class SAM{
public:
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//当前状态max的值
		int ml[ALB];
		NODE(int val):par(nullptr),val(val){
			fill_n(go, ALB, nullptr);
			fill_n(ml, ALB, -1);
		}
		NODE(){};
	}node[MAXN<<2], *last;
	int ncnt;

	void Init(){
		ncnt = 1;
		last = node;
		node[0] = NODE(0);
	}
	void Append(int x){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1);
		for(; p && !p->go[x]; p = p->par)
			p->go[x] = np;
		if(p){
			NODE *q = p->go[x];
			if(p->val + 1 == q->val){
				np->par = q;
			}else{
				NODE *nq = &node[ncnt ++];
				*nq = *q;
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
	//DFS处理，得到dp[a][b]
	void DFS(NODE* rt){
		int ch, t;
		if(rt->ml[0] != -1)
			return ;
		for(ch = 0; ch < ALB; ch ++){
			rt->ml[ch] = 0x3fffffff;
		}
		for(ch = 0; ch < ALB; ch ++){
			if(rt->go[ch]){
				DFS(rt->go[ch]);
				for(t = 0; t < ALB; t ++)
					rt->ml[t] = min(rt->ml[t], rt->go[ch]->ml[t] + 1);
			}else
				rt->ml[ch] = 0;
		}
	}
}sam;

char str[MAXN];
int dp[ALB][ALB]; //dp[a][b]->假设存在子串a……x是str的子串，而a……xb不是str的子串，那么dp[a][b]存的是最小满足条件的a……x的长度
ll L;

//恰好K步最短路的计算和二分
namespace KS{
	ll stp[64][ALB][ALB];
	ll ans[2][ALB][ALB];
	//预处理2^k步的最短路
	void CalcStp(){
		int p, i, j, k;
		ll q;
		for(i = 0; i < ALB; i ++)
			for(j = 0; j < ALB; j ++)
				stp[0][i][j] = dp[i][j];
		for(p = 1, q = 2; q <= L; p ++, q <<= 1){
			for(i = 0; i < ALB; i ++)
				for(j = 0; j < ALB; j ++)
					stp[p][i][j] = 0x3fffffffffffffff;
			for(k = 0; k < ALB; k ++){
				for(i = 0; i < ALB; i ++){
					for(j = 0; j < ALB; j ++){
						stp[p][i][j] = min(stp[p][i][j], stp[p - 1][i][k] + stp[p - 1][k][j]);
					}
				}
			}
		}
	}
	//计算恰好K步的最短路
	ll GetAns(ll K){
		int i, j, k, rd, wt, p;
		ll q;
		ll ret = 0x3fffffffffffffff;
		for(i = 0; i < ALB; i ++)
			for(j = 0; j < ALB; j ++)
				ans[0][i][j] = (i == j) ? 0 : 0x3fffffffffffffff;
		for(p = 0, q = 1, rd = 0, wt = 1; q <= K; p ++, q <<= 1){
			if((K & q) == 0)
				continue;
			for(i = 0; i < ALB; i ++)
				for(j = 0; j < ALB; j ++)
					ans[wt][i][j] = 0x3fffffffffffffff;
			for(k = 0; k < ALB; k ++){
				for(i = 0; i < ALB; i ++){
					for(j = 0; j < ALB; j ++){
						ans[wt][i][j] = min(ans[wt][i][j], ans[rd][i][k] + stp[p][k][j]);
					}
				}
			}
			rd ^= 1, wt ^= 1;
		}
		for(i = 0; i < ALB; i ++){
			for(j = 0; j < ALB; j ++){
				ret = min(ret, ans[rd][i][j]);
			}
		}
		return ret;
	}
	//二分找到最大的K，使得恰好K步的最短路长度小于L
	ll Solve(){
		ll l, r, mid = 0x3fffffffffffffff;

		CalcStp();

		for(l = 0; l < ALB; l ++)
			for(r = 0; r < ALB; r ++)
				mid = min(mid, (ll)dp[l][r]);
		l = 0, r = L / mid + 1;
		while(l < r - 1){
			mid = (l + r) >> 1;
			if(GetAns(mid) < L)
				l = mid;
			else
				r = mid - 1;
		}
		if(GetAns(r) < L)
			return r;
		else
			return l;
	}
};

int main(){
	char *ch;
	int s, t;

	scanf("%I64d\n%s\n", &L, &str);
	sam.Init();
	for(ch = str; *ch; ch ++)
		sam.Append(*ch - 'A');
	sam.DFS(sam.node);
	for(s = 0; s < ALB; s ++){
		for(t = 0; t < ALB; t ++){
			dp[s][t] = sam.node[0].go[s]->ml[t] + 1;
		}
	}
	printf("%I64d\n", KS::Solve() + 1);

	return 0;
}
