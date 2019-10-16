#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 100005;
struct POS{
	int pos;
	int dis;
	int cnt;
	bool operator<(const POS& rhs)const{
		return dis > rhs.dis;
	}
}arr[MAXN];
int N, K, L;
LL ans, ans1, ans2;	//ans是默认的，ans1是无环，ans2是有环
int res[2];	//残留量
LL dp[2][MAXN];	//容让距离，就是如果跑一个环，左边的让出i个给右边的，左边需要多加多少距离

void noring(){
	int i, sz, ps;
	int cap;
	static vector<POS> vec[2];
	vec[0].clear();
	vec[1].clear();
	for(i = 0; i < N; i ++){
		if(arr[i].cnt == 0)
			continue;
		if(arr[i].pos == arr[i].dis)
			vec[0].push_back(arr[i]);
		else
			vec[1].push_back(arr[i]);
	}
	vec[0].push_back(arr[MAXN - 1]);	//三0哨兵
	vec[1].push_back(arr[MAXN - 1]);	//三0哨兵
	sort(vec[0].begin(), vec[0].end());
	sort(vec[1].begin(), vec[1].end());

	for(ps = 0; ps <= 1; ps ++){
		for(i = 0, sz = vec[ps].size(); i < sz; ){
			ans1 += LL(vec[ps][i].dis) * 2;
			for(cap = 0; i < sz; i ++){
				cap += vec[ps][i].cnt;
				if(cap > K)
					break;
				if(cap > 0 && i + 1 < sz)
					dp[ps][K - cap + 1] += LL(vec[ps][i].dis - vec[ps][i + 1].dis) * 2;
			}
			if(i < sz)
				vec[ps][i].cnt = (cap - K);
			else
				res[ps] = K - cap;
		}
		for(i = 1; i <= K; i ++)
			dp[ps][i] += dp[ps][i - 1];
	}
}

void ring(){
	int lgo = K - res[1], rgo = res[1];
	int pl, pr;
	LL tmp = 0x3fffffffffffffffll;
	//这里的特判是错误的，因为即使不能减少出发次数，我们也能均摊距离
	//if(res[0] + res[1] < K){
	//	ans2 = ans1;
	//	return ;
	//}
	ans2 = L;
	for(pl = 1, pr = K - 1; pl <= K - 1; pl ++, pr --){
		if(dp[0][pl] + dp[1][pr] < tmp){
			tmp = dp[0][pl] + dp[1][pr];
			lgo = pl;
			rgo = pr;
		}
	}

	int i, sz, cap;
	int lbegin, rbegin;
	static vector<POS> vec[2];
	vec[0].clear();
	vec[1].clear();
	for(i = 0; i < N; i ++){
		if(arr[i].cnt == 0)
			continue;
		if(arr[i].pos == arr[i].dis)
			vec[0].push_back(arr[i]);
		else
			vec[1].push_back(arr[i]);
	}
	sort(vec[0].begin(), vec[0].end());
	sort(vec[1].begin(), vec[1].end());

	//找到后续的起点
	cap = lgo;
	for(lbegin = 0, sz = vec[0].size(); lbegin < sz; lbegin ++){
		cap += vec[0][lbegin].cnt;
		if(cap > K)
			break;
	}
	if(lbegin < sz)
		vec[0][lbegin].cnt = cap - K;
	cap = rgo;
	for(rbegin = 0, sz = vec[1].size(); rbegin < sz; rbegin ++){
		cap += vec[1][rbegin].cnt;
		if(cap > K)
			break;
	}
	if(rbegin < sz)
		vec[1][rbegin].cnt = cap - K;
	//开始操作
	for(i = lbegin, sz = vec[0].size(); i < sz; ){
		ans2 += LL(vec[0][i].dis) * 2;
		for(cap = 0; i < sz; i ++){
			cap += vec[0][i].cnt;
			if(cap > K)
				break;
		}
		if(i < sz)
			vec[0][i].cnt = (cap - K);
	}
	for(i = rbegin, sz = vec[1].size(); i < sz; ){
		ans2 += LL(vec[1][i].dis) * 2;
		for(cap = 0; i < sz; i ++){
			cap += vec[1][i].cnt;
			if(cap > K)
				break;
		}
		if(i < sz)
			vec[1][i].cnt = (cap - K);
	}
}

void domain(){
	int i;
	scanf("%d%d%d", &L, &N, &K);
	ans = ans1 = ans2 = 0;
	memset(dp, 0, sizeof(dp));
	for(i = 0; i < N; i ++){
		scanf("%d%d", &arr[i].pos, &arr[i].cnt);
		arr[i].dis = min(arr[i].pos, L - arr[i].pos);
		ans += LL(arr[i].cnt / K) * arr[i].dis * 2;
		arr[i].cnt %= K;
	}
	noring();
	ring();
	printf("%I64d\n", ans + min(ans1, ans2));
};

int main(){
	int T;
	scanf("%d", &T);
	while(T --){
		domain();
	}
	return 0;
}
