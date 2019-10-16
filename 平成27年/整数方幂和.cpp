#include <stdio.h>

typedef long long ll;
const ll MOD = 1000000007;
const int MAXN = 1000005;
ll npm[MAXN], sum[MAXN], fac[MAXN], ifac[MAXN];
int prime[MAXN], pcnt;
ll N;
int M;

ll powk(ll x, ll k){
	ll ret = 1;
	for(; k; k >>= 1){
		if(k & 1)
			ret = ret * x % MOD;
		x = x * x % MOD;
	}
	return ret;
}

void prepare(){
	static bool isnp[MAXN];
	int i, j, t;

	//线性筛法处理i^M
	npm[0] = 0;
    for(i = 1; i <= M + 1; i ++)
        npm[i] = 1;
    for(i = 2; i <= M + 1; i ++){
        if(!isnp[i]){
            prime[pcnt ++] = i;
            npm[i] = powk(i, M);
        }
        for(j = 0; j < pcnt; j ++){
            t = i * prime[j];
            if(t > M + 1)
                break;
            isnp[t] = true;
            npm[t] = npm[i] * npm[prime[j]] % MOD;
			if(i % prime[j] == 0)
				break;
        }
    }
	for(i = 1; i <= M + 1; i ++){
		sum[i] = sum[i - 1] + npm[i];
		if(sum[i] >= MOD)
			sum[i] -= MOD;
	}

	fac[0] = 1;
	for(i = 1; i <= M + 1; i ++)
		fac[i] = fac[i - 1] * i % MOD;
	ifac[M + 1] = powk(fac[M + 1], MOD - 2);
	for(i = M + 1; i >= 1; i --)
		ifac[i - 1] = ifac[i] * i % MOD;
}

//Lagrange插值，用f的[0..m]插第n个
ll lagrange(ll f[], int m, ll n){
	if(n < m)
		return f[n];

	int i;
	static ll pl[MAXN], pr[MAXN];
	ll ans = 0, cur = 0;
	pl[0] = pr[m] = 1;
	for(i = 0; i < m; i ++)
		pl[i + 1] = (n - i) % MOD * pl[i] % MOD;
	for(i = m; i >= 1; i --)
		pr[i - 1] = (n - i) % MOD * pr[i] % MOD;
	for(i = 0; i <= m; i ++){
		cur = ((m - i) & 1) ? MOD - 1 : 1;
		cur = cur * pl[i] % MOD * pr[i] % MOD;
		cur = cur * ifac[m - i] % MOD * ifac[i] % MOD;
		cur = cur * f[i] % MOD;
		ans = (ans + cur) % MOD;
	}
	return ans;
}

int main(){
	scanf("%d", &M);
	prepare();
	while(scanf("%lld", &N) != EOF)
		printf("%lld\n", lagrange(sum, M + 1, N));
	return 0;
}