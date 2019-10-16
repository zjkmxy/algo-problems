/* 给一个nxm的方格矩形图，对其中所有的正方形，将被其完全覆盖的方格+1，求所有方格的和
 * 解：
 * 每一个正方形一定可以被比它更大的平行于方格的正方形所覆盖，并在四个角留下四个三角形
 * 假设三角形边长为a和b(>a)，那么三角形的斜边跨过的方格个数为a * ceil(b / a);
 * 设a=kp,b=kq,则ceil(b / a) = (p + q - 1) / q
 * 因此 方格个数 = a + b - k = a + b - gcd(a, b)
 * 其余内容请参看题解。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<set>
#include<numeric>
using namespace std;

typedef long long LL;
const int MAXN = 1000002;
const LL MODN = 1000000007;
const LL DIV3 = 333333336;
LL euler[MAXN], gcds[MAXN];  //欧拉函数值,gcd和值
bool prime[MAXN];
LL sf[MAXN], sumsf[MAXN], lsumsf[MAXN], l2sumsf[MAXN];

void CalcEuler(){
	int i, j;
	for(i = 0; i < MAXN; i ++){
		euler[i] = i;
		prime[i] = true;
	}
	prime[0] = prime[1] = false;
	for(i = 2; i < MAXN; i ++){
		if(prime[i]){
			euler[i] = euler[i] / i * (i - 1);
			for(j = i + i; j < MAXN; j += i){
				prime[j] = false;
				euler[j] = euler[j] / i * (i - 1);
			}
		}
	}
	for(i = 1; i < MAXN; i ++){
		for(j = i; j < MAXN; j += i){
			gcds[j] += i * euler[j / i] % MODN;
			gcds[j] %= MODN;
		}
	}
}

void CalcSumOfF(){
	LL i, j;
	LL tmp, tmp2;

	sf[0] = 0;
	for(i = 1; i < MAXN; i ++){
		sf[i] = 0;
		sf[i] += i * (i + 1ll) % MODN * (i + i + 1ll) % MODN * DIV3 % MODN;
		sf[i] -= 3ll * i * i % MODN;
		sf[i] = (sf[i] % MODN + MODN) % MODN;
		sf[i] += 2ll * gcds[i] % MODN;
		sf[i] = (sf[i] % MODN + MODN) % MODN;
	}
	sumsf[0] = lsumsf[0] = l2sumsf[0] = 0;
	for(i = 1; i < MAXN; i ++){
		sumsf[i] = (sumsf[i - 1] + sf[i]) % MODN;
		lsumsf[i] = (lsumsf[i - 1] + i * sf[i] % MODN) % MODN;
		l2sumsf[i] = (l2sumsf[i - 1] + i * i % MODN * sf[i] % MODN) % MODN;
	}
}

LL CalcResultBF(LL m, LL n){
	LL ans = 0, i;
	if(m > n){
		swap(m, n);
	}
	for(i = 1; i <= m; i ++){
		ans += (n - i + 1) * (m - i + 1) % MODN * sf[i] % MODN;
		ans %= MODN;
	}
	return ans;
}

LL CalcResult(LL m, LL n){
	LL ans = 0;
	if(m > n){
		swap(m, n);
	}
	ans += (n + 1) * (m + 1) % MODN * sumsf[m] % MODN;
	ans -= (n + m + 2) * lsumsf[m] % MODN;
	ans += l2sumsf[m];
	ans = (ans % MODN + MODN) % MODN;
	return ans;
}

int main(){
	int t, m, n;
	CalcEuler();
	CalcSumOfF();
	scanf("%d", &t);
	while(t --){
		scanf("%d%d", &m, &n);
		printf("%I64d\n", CalcResult(m, n));
	}
	return 0;
}
