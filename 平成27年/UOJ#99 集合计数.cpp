//详见杜教出题报告
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 51;
const int MAXT = 6400;
const ll MOD = 1000000021;
ll fac[MAXN], ifac[MAXN], inv[MAXN], comb[MAXN][MAXN];
int N, K;
ll A[MAXN], B[MAXN];
int eq[MAXN];	//等价类个数
ll dp[MAXT];	//插值用，线性方程的非负整数解数
ll g[MAXN][MAXN]; //当前等价类划分下i行和为a[i]且有j个元素大于b[i]此外任选的方案数
int vio[MAXN];	//违例等价类个数
ll ans1, ans2;

ll powk(ll x, ll k){
	ll ret = 1;
	for(; k; k >>= 1){
		if(k & 1)
			ret = ret * x % MOD;
		x = x * x % MOD;
	}
	return ret;
}
int gcd(int a, int b){
	return b ? gcd(b, a % b) : a;
}
inline int lcm(int a, int b){
	return a * b / gcd(a, b);
}

//数据准备
void prepare(){
	int n, m;

	fac[0] = 1;
	for(n = 1; n < MAXN; n ++)
		fac[n] = fac[n - 1] * n % MOD;
	ifac[MAXN - 1] = powk(fac[MAXN - 1], MOD - 2);
	for(n = MAXN - 2; n >= 0; n --)
		ifac[n] = ifac[n + 1] * (n + 1) % MOD;
	for(n = 1; n < MAXN; n ++)
		inv[n] = fac[n - 1] * ifac[n] % MOD;

	//如果dfs2也像dfs1那样写，组合数的表就不必要了
	comb[0][0] = 1;
	for(n = 1; n < MAXN; n ++){
		comb[n][0] = 1;
		for(m = 1; m <= n; m ++)
			comb[n][m] = (comb[n - 1][m] + comb[n - 1][m - 1]) % MOD;
	}
}

//Lagrange插值，周期是t，用f的前m个插第n个
ll lagrange(ll f[], int m, int t, ll n){
	if(n < m * t)
		return f[n];

	int i;
	static ll pl[MAXN], pr[MAXN];
	ll res = n % t, ans = 0, cur = 0;
	n = n / t % MOD;
	-- m;
	pl[0] = pr[m] = 1;
	for(i = 0; i < m; i ++)
		pl[i + 1] = pl[i] * (n - i) % MOD;
	for(i = m; i >= 1; i --)
		pr[i - 1] = pr[i] * (n - i) % MOD;
	for(i = 0; i <= m; i ++){
		cur = ((m - i) & 1) ? MOD - 1 : 1;
		cur = cur * pl[i] % MOD * pr[i] % MOD;
		cur = cur * ifac[m - i] % MOD * ifac[i] % MOD;
		cur = cur * f[i * t + res] % MOD;
		ans = (ans + cur) % MOD;
	}
	return ans;
}

//枚举容斥
//从全部的等价类中减去sum{y[k]}个全部维度小于等于b[i]的向量
//枚举位置，枚举终点，系数
void dfs2(int v, int ed, ll C1, ll C2){
	ll tmp;
	if(v <= ed){
		//x[k]个大小为k的等价类中，至少违反限制的有y[k]个
		//计算相应的容斥系数
		int &cur = vio[v];
		for(cur = 0; cur <= eq[v]; cur ++){
			tmp = comb[eq[v]][cur];
			if(cur & 1)
				tmp = tmp * (MOD - 1) % MOD;
			dfs2(v + 1, ed, C1 * tmp % MOD, C2 * tmp % MOD);
		}
	}else{
		int i, j;
		//计算系数
		//逐步淘汰，从完全任选的情况下减去大于的就得到了强制小于等于的
		//或者说看作二项式反演也可以？（待议）
		static ll C[MAXN];
		C[0] = 1;
		for(i = 1; i <= K; i ++)
			C[i] = 0;
		for(v = 1; v <= ed; v ++){
			for(j = 0; j < vio[v]; j ++){
				for(i = K; i >= v; i --){
					C[i] -= C[i - v];
					if(C[i] < 0)
						C[i] += MOD;
				}
			}
		}
		//计算答案
		for(i = 1; i <= N; i ++){
			tmp = 0;
			for(j = 0; j <= K; j ++)
				tmp = (tmp + C[j] * g[i][j] % MOD) % MOD;
			//各行完全独立，直接乘
			C1 = C1 * tmp % MOD;
			C2 = C2 * tmp % MOD;
		}
		ans1 = (ans1 + C1) % MOD;
		ans2 = (ans2 + C2) % MOD;
	}
}

//枚举等价类
//剩余位置，个数，当前等价类大小，周期，系数
void dfs1(int resk, int m, int v, int t, ll C1, ll C2){
	if(resk > 0){
		//DFS枚举
		//C1是等价类方案数乘以对应轮换数
		//C2是乘以对应G值
		for(; v <= resk; v ++){
			eq[v] ++;
			ll tmp1 = C1 * inv[v] % MOD * inv[eq[v]] % MOD;
			ll tmp2 = tmp1;
			if(!(v & 1))
				tmp1 = tmp1 * (MOD - 1) % MOD;
			dfs1(resk - v, m + 1, v, lcm(t, v), tmp1, tmp2);
			eq[v] --;
		}
	}else{
		int i, j;
		ll sum;
		//多重背包计算选择方案数
		//保持等价类关系下任意取k个非负整数，和为x的方案数是dp[x]
		for(i = 1; i < m * t; i ++)
			dp[i] = 0;
		dp[0] = 1;
		for(sum = 0; sum <= v; sum ++){
			for(j = 0; j < eq[sum]; j ++){
				for(i = sum; i < m * t; i ++)
					dp[i] = (dp[i] + dp[i - sum]) % MOD;
			}
		}
		//Lagrange插值
		for(i = 1; i <= N; i ++){
			for(j = 0, sum = A[i]; j <= K; j ++, sum -= B[i] + 1){
				if(sum >= 0){
					g[i][j] = lagrange(dp, m, t, sum);
				}else{
					g[i][j] = 0;
					sum = -1;
				}
			}
		}
		dfs2(1, v, C1, C2);
	}
}

int main(){
	int i;
	prepare();
	scanf("%d%d", &N, &K);
	for(i = 1; i <= N; i ++)
		scanf("%lld", &A[i]);
	for(i = 1; i <= N; i ++)
		scanf("%lld", &B[i]);
	dfs1(K, 0, 1, 1, 1, 1);
	printf("%lld %lld\n", ans1, ans2);
	return 0;
}