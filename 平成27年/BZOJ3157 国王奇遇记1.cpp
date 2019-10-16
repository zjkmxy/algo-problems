/*
构造向量V={Sn, n^0*m^n, n^1*m^n, ..., n^m*m^n}
然后V的递推矩阵是m乘上一个杨辉三角
然后特征多项式是(x-1)*(x-m)^(m+1)
然后多项式优化成O(M^2logN)的就能过
*/
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 2003;
const int MODN = 1000000007;
ll comb[MAXN][MAXN];
ll A[MAXN];
int N, M;

ll powk(ll x, ll k){
	ll ret = 1;
	for(; k; k >>= 1){
		if(k & 1)
			ret = ret * x % MODN;
		x = x * x % MODN;
	}
	return ret;
}

typedef long long int LLI;
template<typename T>
struct Polynomial {
	T v[MAXN];
	int sz;
	static int mod;
	Polynomial() : sz(0) {}
	Polynomial(int n, const T &x = 0) : sz(n) { fill(v, v + n, x); }
	int size()const{ 
		return sz;
	}
	void resize(int n) 
	{ 
		sz = n;
	}
	T &operator [](int k) { 
		return v[k]; 
	}

	Polynomial operator *(const Polynomial &o) const {
		Polynomial ret(sz + o.sz - 1);
		int i, j;
		for (i = 0; i < sz; ++i) {
			if (!v[i]) continue;
			for (j = 0; j < o.sz; ++j) {
				ret.v[i + j] += v[i] * o.v[j];
				if (~mod) ret.v[i + j] %= mod;
			}
		}
		return ret;
	}

	Polynomial operator %(const Polynomial &o) const {
		Polynomial ret = *this;
		int i, j, n = o.size() - 1;
		for (i = sz - 1; i >= n; --i) {
			if (!ret.v[i]) continue;
			for (j = 1; j <= n; ++j) {
				ret.v[i - j] -= ret.v[i] * o.v[n - j];
				if (~mod && (ret.v[i - j] %= mod) < 0) ret.v[i - j] += mod;
			}
		}
		ret.sz = min(ret.sz, n);
		return ret;
	}

	Polynomial pow(LLI n, const Polynomial &p) const {
		Polynomial ret(1, 1), a = *this;
		for ( ; n; n >>= 1) {
			if (n & 1) ret = (ret * a) % p;
			a = (a * a) % p;
		}
		return ret;
	}
	Polynomial pow(LLI n) const {
		Polynomial ret(1, 1), a = *this;
		for ( ; n; n >>= 1) {
			if (n & 1) ret = (ret * a);
			a = (a * a);
		}
		return ret;
	}
};
template<typename X>
int Polynomial<X>::mod = MODN;

LLI solve()
{
	static Polynomial<LLI> u(1), w(2), t(2), p(2);
	int i;
	LLI ret = 0;

	t[0] = - M;
	t[1] = 1;
	p[0] = - 1;
	p[1] = 1;
	u = p * t.pow(M + 1);
	w[0] = 0;
	w[1] = 1;
	w = w.pow(N, u);
	for(i=0;i<w.size();i++)
		ret = (ret + w[i] * A[i]) % MODN;
	return ret;
}

int main(){
	int i;

	scanf("%d%d", &N, &M);
	A[0] = 0;
	for(i = 1; i <= M + 1; i ++){
		A[i] = (A[i - 1] + powk(i, M) * powk(M, i) % MODN) % MODN;
	}
	printf("%lld\n", solve());
	return 0;
}
