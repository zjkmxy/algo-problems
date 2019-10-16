/*
1,2,...,n个数取m个数的可重集的个数
先假装我们不知道它是C(n+m-1, m)
*/
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXM = 11;
const int MAXN = 1001;
const int MODN = 1000000007;

ll powll(ll x, ll k){
	ll ret = 1;
	for(; k; k >>= 1){
		if(k & 1)
			ret = ret * x % MODN;
		x = x * x % MODN;
	}
	return ret;
}

//算法1：基本dp
class Alg1{
public:
	//dp[m][n]表示m个数的非减序列，以不超过n结尾的方案数
	//把m的非减序列开头补0，差分完变成bi的任取，用隔板法就是组合数
	//不过我们先不用组合数
	int dp[MAXM][MAXN];
	Alg1(){
		int m, n;
		for(n = 1; n < MAXN; n ++)
			dp[0][n] = 1;
		for(m = 1; m < MAXM; m ++){
			for(n = 1; n < MAXN; n ++)
				dp[m][n] = (dp[m][n - 1] + dp[m - 1][n]) % MODN;
		}
	};
	inline int query(int n, int m){
		return dp[m][n];
	}
}alg1;

//算法2：枚举等价类划分+Burnside引理
//元素划分等价类的思路和集合枚举一样
//然后考虑每个i-轮换的等价类对应(i-1)!种置换，乘上去
//最后是Burnside引理的除法，化简后得到下式
class Alg2{
public:
	ll inv[MAXM];
	Alg2(){
		for(int i = 1; i < MAXM; i ++)
			inv[i] = powll(i, MODN - 2);
	}
public:
	//不同大小等价类的个数
	int cnt[MAXM];
	ll ans;
	void dfs(int res, int v, ll cur, int n){
		if(res == 0){
			ans = (ans + cur) % MODN;
			return ;
		}
		cur = cur * n % MODN;
		for(; v <= res; v ++){
			cnt[v] ++;
			dfs(res - v, v, cur * inv[v] % MODN * inv[cnt[v]] % MODN, n);
			cnt[v] --;
		}
	}
	inline int query(int n, int m){
		ans = 0;
		dfs(m, 1, 1, n);
		return int(ans);
	}
}alg2;

//算法3：生成多项式插值
class Alg3{
public:
	ll fac[MAXM], ifac[MAXM];
	ll dp[MAXM][MAXM];
	Alg3(){
		fac[0] = ifac[0] = 1;
		for(int i = 1; i < MAXM; i ++){
			fac[i] = fac[i - 1] * i % MODN;
			ifac[i] = powll(fac[i], MODN - 2);
		}

		//这里只计算n=0..m就可以
		int m, n;
		for(n = 1; n < MAXM; n ++)
			dp[0][n] = 1;
		for(m = 1; m < MAXM; m ++){
			for(n = 1; n < MAXM; n ++)
				dp[m][n] = (dp[m][n - 1] + dp[m - 1][n]) % MODN;
		}
	}
public:
	int query(int n, int m){
		if(n <= m)
			return dp[m][n];

		int j;
		ll p1 = 1, ans = 0, cur;

		for(j = 0; j <= m; j ++)
			p1 = p1 * (n - j) % MODN;
		for(j = 0; j <= m; j ++){
			cur = ((m - j) & 1) ? MODN - 1 : 1;
			cur = cur * p1 % MODN * dp[m][j] % MODN;
			cur = cur * ifac[m - j] % MODN * ifac[j] % MODN;
			cur = cur * powll(n - j, MODN - 2) % MODN;
			ans = (ans + cur) % MODN;
		}
		return int(ans);
	}
}alg3;

int main(){
	int n, m;
	while(scanf("%d%d", &n, &m) != EOF){
		printf("%d\n", alg3.query(n, m));
	}
	return 0;
}