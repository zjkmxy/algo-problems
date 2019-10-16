#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<set>
#include<numeric>
using namespace std;

typedef unsigned long long ull;
typedef long long ll;
typedef unsigned long ul;
const ul MODN = 998244353;  //常用参数:C0000001;3B800001
const ul MAXN = 262144;
struct uint32{
	ul n;
	uint32(ul n = 0):n(n){}
	void assign(int x){
		for(x %= MODN; x < 0; x += MODN);
		n = (ul)x;
	}
	inline uint32 operator-() const{
		return uint32(MODN - n);
	};
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
		while(p >= 1){
			if(p & 1)
				ret = ret * a;
			p >>= 1;
			a = a * a;
		}
		return ret;
	}
	uint32 inv() const{
		int x1 = 1, x2 = 0, x3 = MODN;
		int y1 = 0, y2 = 1, y3 = n, k;
		while (y3 != 1){
			k = x3 / y3;
			x1 -= y1 * k, x2 -= y2 * k, x3 -= y3 * k;
			swap(x1, y1), swap(x2, y2), swap(x3, y3);
		}
		return uint32(y2 >= 0 ? y2 : y2 + MODN);
	}
	friend inline uint32 operator/(const uint32& a, const uint32& b){
		return a * b.inv();
	}
	inline uint32 div2() const{
		return (*this) * uint32((MODN + 1) >> 1);
	};
};
namespace ntt{
	const uint32 spec(3); //初始因子
	ul maxm, maxn;
	uint32 *ftab;
	//给出最大的计算代数m(2^m=n)
	void init(int m){
		uint32 f0;
		maxm = m, maxn = (1 << m);
		if((MODN - 1) % maxn != 0) throw 0;
		f0 = spec.pow((MODN - 1) / maxn);
		ftab = new uint32[maxn + 1];
		ftab[0] = 1;
		for(int i = 1; i <= maxn; i ++) ftab[i] = ftab[i - 1] * f0;
	}
	void fin(){
		delete [] ftab;
	}
	void fntt(int M, uint32 seq_in[], uint32 seq_out[], bool tran){
		int i, j, k, flg, flg2, tmp, p, n = (1<<M);
		uint32 t1, t2, divs = uint32(n).pow(MODN - 2);
		//倒位
		for(tmp = 0, i = 0; i < n; i ++){
			seq_out[i] = seq_in[tmp];
			if(i == n-1) break;
			for(j = M - 1; tmp & (1<<j); j --) tmp ^= (1<<j);
			tmp |= (1<<j);
		}
		//计算
		for(i=0,tmp=1,flg=1<<(maxm-1);i<M;i++,tmp<<=1,flg>>=1){
			for(j=0;j<n;j+=(tmp<<1)){
				flg2 = tran ? 0 : maxn;
				for(k=0;k<tmp;k++){
					p = j + k;
					t1 = seq_out[p];
					t2 = seq_out[p|tmp] * ftab[flg2];
					flg2 = tran ? flg2 + flg : flg2 - flg;
					seq_out[p] = t1 + t2;
					seq_out[p|tmp] = t1 - t2;
				}
			}
		}
		if(!tran){
			for(i=0;i<n;i++)
				seq_out[i] = seq_out[i] * divs;
		}
	}
};
//模x^N多项式，uint32换成复数用fft亦可
class polynomial{
private:
	int M, N;  //这里的N是2幂，且务必同N操作
	uint32 *num;
	static uint32 pool[MAXN << 4];
	static ul pcnt;
	static uint32* get(ul num){
		uint32 *ret = pool + pcnt;
		pcnt += num;
		return ret;
	}
	static void clean(ul num){
		pcnt -= num;
	}
public:
	polynomial(const int n){
		for(M = 0, N = 1; N < n; M ++, N <<= 1);
		num = new uint32[N];
		fill(num, num + N, 0);
	}
	polynomial(const polynomial& other):N(other.N){
		num = new uint32[N];
		copy(other.num, other.num + N, num);
	}
	~polynomial(){
		delete [] num;
	}
	uint32& operator[](ul p){
		return num[p];
	}
	//x^N内乘法（rhs.N>=N,ret.N>=2N）
	void mulTo(const polynomial& rhs, polynomial& ret)const{
		uint32 *ta=get(N<<1),*tb=get(N<<1),*ffta=get(N<<1),*fftb=get(N<<1);
		copy(num, num + N, ta);
		copy(rhs.num, rhs.num + N, tb);
		fill(ta + N, ta + (N << 1), uint32(0));
		fill(tb + N, tb + (N << 1), uint32(0));
		ntt::fntt(M + 1, ta, ffta, true);
		ntt::fntt(M + 1, tb, fftb, true);
		for(int i = 0; i < (N << 1); i ++)
			ffta[i] = ffta[i] * fftb[i];
		ntt::fntt(M + 1, ffta, ret.num, false);
		clean(N<<3);
	};
	//对x^N取反(ret.N>=N)
	void invTo(polynomial& ret)const{
		int p, m;
		polynomial nr(N << 1);
		fill(ret.num, ret.num + N, 0);
		ret.num[0] = num[0].inv();
		for(p = 1, m = 2; m <= N; m <<= 1, p ++){
			int h = m >> 1;
			nr.N = m; nr.M = p;
			copy(num, num + m, nr.num);
			nr.mulTo(ret, nr);
			fill(nr.num, nr.num + h, 0);
			for(int i = h; i < m; i ++)
				nr.num[i] = - nr.num[i];
			nr.mulTo(ret, nr);
			copy(nr.num + h, nr.num + m, ret.num + h);
		}
	};
	//对x^N开平方根(ret.N>=N)，要求f[0]=1，否则不能对数字取模
	void sqrtTo(polynomial& ret)const{
		int p, m;
		polynomial rs(N<<1), nrs(N<<1); //动态求逆元用
		fill(ret.num, ret.num + N, 0);
		fill(rs.num, rs.num + N, 0);
		//rs.num[0] = ret.num[0] = num[0].sqrt();
		rs.num[0] = ret.num[0] = 1;
		for(p = 1, m = 2; m <= N; m <<= 1, p ++){
			int h = m >> 1;
			nrs.N = m; nrs.M = p;
			copy(ret.num, ret.num + h, nrs.num);
			fill(nrs.num + h, nrs.num + m, 0);
			nrs.mulTo(rs, nrs);
			fill(nrs.num, nrs.num + h, 0);
			for(int i = h; i < m; i ++)
				nrs.num[i] = - nrs.num[i];
			nrs.mulTo(rs, nrs);
			copy(rs.num, rs.num + h, nrs.num);
			nrs.mulTo(*this, nrs);
			for(int i = h; i < m; i ++)
				ret.num[i] = nrs.num[i].div2();
			copy(ret.num, ret.num + m, nrs.num);
			nrs.mulTo(rs, nrs);
			fill(nrs.num, nrs.num + h, 0);
			for(int i = h; i < m; i ++)
				nrs.num[i] = - nrs.num[i];
			nrs.mulTo(rs, nrs);
			copy(nrs.num + h, nrs.num + m, rs.num + h);
		}
	}
};
uint32 polynomial::pool[MAXN << 4];
ul polynomial::pcnt = 0;

int main(){
	int n, m, k;
	static int c[MAXN];
	ntt::init(20);
	scanf("%d%d", &n, &m);
	for(int i = 0; i < n; i ++){
		scanf("%d", &c[i]);
	}
	k = 1;
	while (k < m + 1)
		k <<= 1;
	polynomial a(k), b(k), ret(k);
	a[0] = 1;
	for(int i = 0; i < n; i ++){
		if(c[i] <= m){
			a[c[i]] = - uint32(4);
		}
	}
	a.sqrtTo(b);
	b[0] = b[0] + 1;
	for(int i = 0; i < k; i ++){
		b[i] = b[i].div2();
	}
	b.invTo(ret);
	for(int i = 1; i <= m; i ++){
		printf("%lu\n", ret[i]);
	}
	ntt::fin();
	return 0;
}