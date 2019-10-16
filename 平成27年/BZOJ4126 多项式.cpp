/**************************************************************
    Problem: 4126
    User: zjkmxy
    Language: C++
    Result: Accepted
    Time:4196 ms
    Memory:26688 kb
****************************************************************/

//G(i) = sum(i=0..n-1){ P(i) * Q^i } = Q^n * F(n) - F(0)
//上式中F是和P同阶的多项式，因此可以插值
//假设P是d阶，那么F在d+1阶二项式反演下得到的最高次系数为0
//又有F(i+1) = (F(i) + P(i)) / Q
//通过以上两式得到F(0)的一次方程解出F(0)来，答案就出来了
 
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <queue>
using namespace std;
 
typedef long long ll;
const int MAXN = 500005;
const ll MOD = 1000000007;
ll npm[MAXN], F[MAXN], fac[MAXN], inv[MAXN], ifac[MAXN];
int prime[MAXN], pcnt;
bool isnp[MAXN];
int N, M;
 
ll powk(ll x, ll k){
    ll ret = 1;
    for(; k; k >>= 1){
        if(k & 1)
            ret = ret * x % MOD;
        x = x * x % MOD;
    }
    return ret;
}
 
//Lagrange插值，用f的[0..m]插第n个
ll lagrange(ll f[], int m, ll n){
    if(n < m)
        return f[n];
 
    int i;
    static ll pr[MAXN];
    ll pl = 1, ans = 0, cur = 0;
    pr[m] = 1;
    for(i = m; i >= 1; i --)
        pr[i - 1] = (n - i) * pr[i] % MOD;
    for(i = 0; i <= m; i ++){
        cur = ((m - i) & 1) ? MOD - 1 : 1;
        cur = cur * pl % MOD * pr[i] % MOD;
        cur = cur * ifac[m - i] % MOD * ifac[i] % MOD;
        cur = cur * f[i] % MOD;
        ans = (ans + cur) % MOD;
        pl = (n - i) * pl % MOD;
    }
    return ans;
}
 
int main(){
    int i, j, t;
    scanf("%d%d", &N, &M);
 
    //数据准备
    fac[0] = 1;
    for(i = 1; i <= M + 1; i ++)
        fac[i] = fac[i - 1] * i % MOD;
    ifac[M + 1] = powk(fac[M + 1], MOD - 2);
    for(i = M + 1; i >= 1; i --)
        ifac[i - 1] = ifac[i] * i % MOD;
    for(i = 1; i <= M + 1; i ++)
        inv[i] = fac[i - 1] * ifac[i] % MOD;
 
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
 
    //递推系数A和B
    ll A = 1, B = 0, PA = 0, PB = 0, tmp = 1, tmp2;
    for(i = 0; i <= M + 1; i ++){
        tmp2 = (i & 1) ? MOD - tmp : tmp;
        PA = (PA + tmp2 * A % MOD) % MOD;
        PB = (PB + tmp2 * B % MOD) % MOD;
 
        A = A * inv[M] % MOD;
        B = (B + npm[i]) % MOD * inv[M] % MOD;
        tmp = tmp * (M + 1 - i) % MOD * inv[i + 1] % MOD;
    }
    F[0] = (MOD - PB) * powk(PA, MOD - 2) % MOD;
 
    //递推得到F
    for(i = 0; i <= M; i ++)
        F[i + 1] = (F[i] + npm[i]) % MOD * inv[M] % MOD;
 
    //插值得到答案
    ll ans = lagrange(F, M + 1, N + 1); //这里写M+1则AC，写M则WA，但是本地查不出任何不同点来……
    ans = (ans * powk(M, N + 1) % MOD + (MOD - F[0])) % MOD;
    printf("%lld\n", ans);
    return 0;
}