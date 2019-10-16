/*
1,2,...,n����ȡm�����Ŀ��ؼ��ĸ���
�ȼ�װ���ǲ�֪������C(n+m-1, m)
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

//�㷨1������dp
class Alg1{
public:
	//dp[m][n]��ʾm�����ķǼ����У��Բ�����n��β�ķ�����
	//��m�ķǼ����п�ͷ��0���������bi����ȡ���ø��巨���������
	//���������Ȳ��������
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

//�㷨2��ö�ٵȼ��໮��+Burnside����
//Ԫ�ػ��ֵȼ����˼·�ͼ���ö��һ��
//Ȼ����ÿ��i-�ֻ��ĵȼ����Ӧ(i-1)!���û�������ȥ
//�����Burnside����ĳ����������õ���ʽ
class Alg2{
public:
	ll inv[MAXM];
	Alg2(){
		for(int i = 1; i < MAXM; i ++)
			inv[i] = powll(i, MODN - 2);
	}
public:
	//��ͬ��С�ȼ���ĸ���
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

//�㷨3�����ɶ���ʽ��ֵ
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

		//����ֻ����n=0..m�Ϳ���
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