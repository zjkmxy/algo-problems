//然而这道题的dp公式是可以展开然后保存前缀和的，因此有O(N)解法
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

typedef unsigned long long ull;
typedef long long ll;
typedef unsigned long ul;
const ul MODN = 998244353;
const ul MAXN = 262144;
const ul MN = 100000;
struct uint32{
	ul n;
	uint32(ul n = 0):n(n){}
	void assign(int x){	for(x %= MODN; x < 0; x += MODN);	n = (ul)x;	}
	inline uint32 operator-() const{	return uint32(MODN - n);	};
	friend inline uint32 operator*(const uint32& a, const uint32& b){
		ull A(a.n), B(b.n);
		return uint32(A * B % MODN);
	}
	friend inline uint32 operator+(const uint32& a, const uint32& b){
		ul n = a.n + b.n;
		return uint32((n < a.n || n >= MODN) ? (n - MODN) : n);
	}
	friend inline uint32 operator-(const uint32& a, const uint32& b){
		return (a.n < b.n) ? (a.n + MODN - b.n) : (a.n - b.n);
	}
	uint32 pow(ul p) const{
		uint32 ret(1), a(n);
		for(; p >= 1; p >>= 1, a = a * a)	if(p & 1) ret = ret * a;
		return ret;
	}
	uint32 inv() const{
		int x1 = 1, x2 = 0, x3 = MODN, y1 = 0, y2 = 1, y3 = n, k;
		while (y3 != 1){
			k = x3 / y3, x1 -= y1 * k, x2 -= y2 * k, x3 -= y3 * k;
			swap(x1, y1), swap(x2, y2), swap(x3, y3);
		}
		return uint32(y2 >= 0 ? y2 : y2 + MODN);
	}
	friend inline uint32 operator/(const uint32& a, const uint32& b){	return a * b.inv();	}
	inline uint32 div2() const{	return (*this) * uint32((MODN + 1) >> 1);	};
	inline uint32 sqrt()const{	return pow(MODN - 2); 	}
};
namespace ntt{
	const uint32 spec(3);
	ul maxm, maxn;
	uint32 *ftab;
	//初始化：给出最大的计算代数m(2^m=n)
	void init(int m){
		uint32 f0;
		maxm = m, maxn = (1 << m);
		if((MODN - 1) % maxn != 0) throw 0;
		f0 = spec.pow((MODN - 1) / maxn);
		ftab = new uint32[maxn + 1];
		ftab[0] = 1;
		for(int i = 1; i <= maxn; i ++) ftab[i] = ftab[i - 1] * f0;
	}
	void fin(){	delete [] ftab;	}
	void fntt(int M, uint32 seq_in[], uint32 seq_out[], bool tran){
		int i, j, k, flg, flg2, tmp, p, n = (1<<M);
		uint32 t1, t2, divs = uint32(n).pow(MODN - 2);
		//倒位
		for(tmp = 0, i = 0; i < n; i ++, tmp |= (1<<j)){
			seq_out[i] = seq_in[tmp];
			if(i == n-1) break;
			for(j = M - 1; tmp & (1<<j); j --) tmp ^= (1<<j);
		}
		//计算
		for(i=0,tmp=1,flg=1<<(maxm-1);i<M;i++,tmp<<=1,flg>>=1){
			for(j=0;j<n;j+=(tmp<<1)){
				flg2 = tran ? 0 : maxn;
				for(k=0;k<tmp;k++){
					p = j + k;
					t1 = seq_out[p];			t2 = seq_out[p|tmp] * ftab[flg2];
					seq_out[p] = t1 + t2;		seq_out[p|tmp] = t1 - t2;
					flg2 = tran ? flg2 + flg : flg2 - flg;
				}
			}
		}
		if(!tran)
			for(i=0;i<n;i++) seq_out[i] = seq_out[i] * divs;
	}
};

uint32 dp[MAXN], fac[MAXN], faci[MAXN], sqr[MAXN];

void prepare(){
	int i;
	ntt::init(17);
	fac[0] = faci[0] = 1;
	sqr[0] = 0;
	for(i = 1; i <= MN; i ++){
		fac[i] = fac[i - 1] * uint32(i);
		faci[i] = fac[i].inv();
		sqr[i] = uint32(i) * uint32(i);
	}
}

void calc_bf(){
	int i, j;
	dp[0] = 1;
	dp[1] = 1;
	for(i = 2; i <= 100; i ++){
		dp[i] =0;
		for(j = 0; j < i; j ++){
			dp[i] = dp[i] + dp[j] * faci[j] * sqr[i - j];
		}
		dp[i] = dp[i] * fac[i - 1];
	}
}

//m是2次幂
void solve(int m, int l, int r){
	if(l == r){
		dp[l] = l > 0 ? dp[l] * fac[l - 1] : 1;
		return ;
	}
	int mid = (l + r) >> 1, len = mid - l + 1;
	solve(m - 1, l, mid);

	int i;
	static uint32 A[MAXN], B[MAXN], TA[MAXN], TB[MAXN];
	for(i = 0; i < (1 << m); i ++){
		A[i] = i < len ? dp[l + i] * faci[l + i] : 0;
		B[i] = sqr[i];
		TA[i] = TB[i] = 0;
	}
	ntt::fntt(m, A, TA, true);
	ntt::fntt(m, B, TB, true);
	for(i = 0; i < (1 << m); i ++)
		TA[i] = TA[i] * TB[i];
	ntt::fntt(m, TA, A, false);
	for(i = mid + 1; i <= r; i ++){
		dp[i] = dp[i] + A[i - l];
	}

	solve(m - 1, mid + 1, r);
}

int main(){
	int n;
	prepare();
	solve(17, 0, MN);
	while(scanf("%d", &n) != EOF){
		printf("%d\n", dp[n].n);
	}
	ntt::fin();
	return 0;
}