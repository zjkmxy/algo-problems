//首先要了解原题的解法，然后重点注意到转移矩阵可以转化成三角矩阵，用黑盒线代
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 10;
const int MAXS = (1 << MAXN);
const int MODN = 1000000007;

char source[MAXN+1];
int pattern[4], N, M, S;
int sttab[MAXS];
int got[MAXS][4];
int mat[MAXS][MAXS]; //mat[st][ed]表示从st转移到ed的方案数
ll dp[MAXS][MAXS];   //dp[i][st]表示长度为i的子串状态为st时的个数
ll res[MAXS], ans[MAXN+1];

//预处理位图
void set_pattern(){
	int i, j, k;

	memset(pattern, 0, sizeof(pattern));
	for(i = 0, j = 1; i < N; i ++, j <<= 1){
		switch(source[i]){
		case 'A':
			k = 0;
			break;
		case 'G':
			k = 1;
			break;
		case 'C':
			k = 2;
			break;
		case 'T':
			k = 3;
			break;
		}
		pattern[k] = (pattern[k] | j);
	}
}

//位数计算
int bitcnt(int x){
	int res = 0, t, j;
	int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};

	for(t = 1, j = 0; j < 5; j ++, t <<= 1)
		x = (x & b[j]) + ((x >> t) & b[j]);
	return x;
}

//贪心计算，y是已匹配的位点，x是能匹配的位点，返回新的匹配位点
inline int domatch(int y, int x)
{
	int tx = y | x;
	int ty = (y << 1) | 1;
	return (tx & ((tx - ty) ^ tx));
	//如果已经匹配的位为0，这一句会匹配x的最低位
	//如果已经匹配的位不为0，这一句会用每一段最低的1换下同段末的1
	//如AXBCDA,BDA,此时结果是001010=>100011
	//于是计算的结果就是在保证最长长度的情况下可能有的最低位数分布
	//注意：如果要求出LCS的值而非长度，此时会出现错误
}

//对状态进行排序处理
inline bool comp_st(const int& a, const int& b){
	int ba = bitcnt(a), bb = bitcnt(b);
	if(ba != bb)
		return ba < bb;
	else
		return b < a;
}
void set_sttab(){
	int i;
	static int tmp[MAXS];
	S = 1 << N;
	for(i = 0; i < S; i ++)
		tmp[i] = i;
	sort(tmp, tmp + S, comp_st);
	for(i = 0; i < S; i ++)
		sttab[tmp[i]] = i;
}

//构造转移矩阵，进行DP计算
void build_mat(){
	int i, st, j, ed;
	const int ED = min(M + 1, S);

	memset(mat, 0, sizeof(mat));
	for(st = 0; st < S; st ++){
		for(j = 0; j < 4; j ++){
			ed = domatch(st, pattern[j]);
			got[sttab[st]][j] = sttab[ed];
			mat[sttab[st]][sttab[ed]] ++;
		}
	}
	memset(dp, 0, sizeof(dp));
	dp[0][0] = 1;
	for(i = 1; i < ED; i ++){
		for(st = 0; st < S; st ++){
			if(dp[i - 1][st] == 0)
				continue;
			//枚举字符
			for(j = 0; j < 4; j ++){
				ed = got[st][j];
				dp[i][ed] += dp[i - 1][st];
				dp[i][ed] %= MODN;
			}
		}
	}
};

//朴素多项式的模板
//赌五毛FFT乘法过不去这道题
struct Polynomial {
	ll v[MAXS<<2];
	int sz;
	static int mod;
	Polynomial() : sz(0) {}
	Polynomial(int n, const ll &x = 0) : sz(n) { fill(v, v + n, x); }
	int size()const{ 
		return sz;
	}
	void resize(int n) 
	{ 
		sz = n;
	}
	ll &operator [](int k) { 
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

	Polynomial pow(ll n, const Polynomial &p) const {
		Polynomial ret(1, 1), a = *this;
		for ( ; n; n >>= 1) {
			if (n & 1) ret = (ret * a) % p;
			a = (a * a) % p;
		}
		return ret;
	}
};
int Polynomial::mod = MODN;

void run(){
	Polynomial u(1), w(2);
	int i, p;

	memset(res, 0, sizeof(res));
	memset(ans, 0, sizeof(ans));
	if(M >= S){
		u[0] = 1;
		for(i = 0; i < S; i ++){
			w[0] = - mat[i][i];
			w[1] = 1;
			u = u * w;
		}
		w[0] = 0;
		w[1] = 1;
		w = w.pow(M, u);
		for(p = 0; p < w.size(); p ++){
			for(i = 0; i < S; i ++){
				res[i] = (res[i] + dp[p][i] * w[p] % MODN) % MODN;
			}
		}
	}else{
		for(i = 0; i < S; i ++)
			res[i] = dp[M][i];
	}
	for(i = 0; i < S; i ++){
		p = bitcnt(i);
		ans[p] = (ans[p] + res[sttab[i]]) % MODN;
	}
}

int main(){
	int t, i;
	scanf("%d",&t);
	while(t--){
		scanf("%d%d%s", &N, &M, source);
		set_pattern();
		set_sttab();
		build_mat();
		run();
		for(i = 0; i <= N; i ++){
			printf("%lld\n", ans[i]);
		}
	}
	return 0;
}