#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;
const ll MOD = 1000000007;
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

const int MAXN = 102;
ll stir[MAXN][MAXN];
int prime[MAXN], pcnt, phi[MAXN];
ll cnt[MAXN];
int N, M, C;

void init(){
	int n, k, t;

	//第一类斯特林数
	stir[1][1] = 1;
	for(n = 2; n < MAXN; n ++){
		for(k = 1; k <= n; k ++){
			stir[n][k] = (stir[n - 1][k - 1] - (n - 1) * stir[n - 1][k]) % MOD;
			if(stir[n][k] < 0)
				stir[n][k] += MOD;
		}
	}

	//欧拉函数
	for(n = 0; n < MAXN; n ++)
		phi[n] = -1;
	phi[0] = phi[1] = 1;
	for(n = 2; n < MAXN; n ++){
		if(phi[n] == - 1){
			prime[pcnt ++] = n;
			phi[n] = n - 1;
		}
		for(k = 0; k < pcnt; k ++){
			t = n * prime[k];
			if(t >= MAXN)
				break;
			if(n % prime[k] == 0){
				phi[t] = phi[n] * prime[k];
				break;
			}else{
				phi[t] = phi[n] * (prime[k] - 1);
			}
		}
	}
}

void deal(int len, int coe){
	ll x = powk(C, len), cur = x;
	for(int i = 1; i <= M; i ++){
		cnt[i] = (cnt[i] + cur * coe % MOD) % MOD;
		cur = cur * x % MOD;
	}
}

bool domain(){
	int i;
	ll div, ans = 0;

	if(scanf("%d%d%d", &N, &M, &C) == EOF)
		return false;
	for(i = 1; i <= M; i ++)
		cnt[i] = 0;

	//旋转
	for(i = 1; i * i < N; i ++){
		if(N % i > 0)
			continue;
		deal(i, phi[N / i]);
		deal(N / i, phi[i]);
	}
	if(i * i == N)
		deal(i, phi[i]);

	//归一化、反演
	div = powk(N, MOD - 2);
	for(i = 1; i <= M; i ++){
		cnt[i] = cnt[i] * div % MOD;
		ans = (ans + cnt[i] * stir[M][i] % MOD) % MOD;
	}

	printf("%lld\n", ans);

	return true;
}

int main(){
	init();
	while(domain());
	return 0;
}