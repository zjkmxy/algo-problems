/*
题目概要：nXm的网格每个格染c色，要求任意两行与任意两列不能全同，求方案数。

把n行每一行看成是一个m个元素的向量，相当于对向量进行集合计数，额外条件要求列不同
备选的全集为c^m种不同的向量。

思路有三：
1.Stirling反演
首先考虑如果没有m列两两不同的限制，我们的方案数就是A[c^m,n]。我们设原来的答案是f(m)。
考虑对列进行分组（划分等价类），让等价类内部相等，不同等价类之间不等。
划分出i个等价类的种类数是S(m,i)，乘上i个不同时的答案f(i)得到：
A[c^m][n] = sum(i=1..m){ S(m,i) * f(i) }
做Stirling反演得到：
f(m) = sum(i=1..m){ s(m,i) * A[c^i][n] }  s(n,k)是第一类Stirling数

2.集合计数
全集中选出k个元素的集合，和选出k个元素的两两不同的排列，只差一个k!系数。我们把它们看作是相同的东西。
最单纯的想法是枚举每一对(i,j)表示序列中Ai==Aj然后做容斥。
进一步考虑到每次枚举若干对相等关系实际上是对序列划分了等价类，所以我们直接枚举等价类。
每一个等价类对应容斥的多种方案，详见dyh论文。
考虑到本题中每一个等价类的大小无足轻重，有影响的仅仅是等价类的个数。
所以直接枚举等价类个数，就是第一类Stirling数，得到答案。

3.DP做法
不会。看了别人代码也没看懂。

和Burnside引理的区别：
如果把m的全部置换作为置换群，做Burnside引理得到
g(m)=sum(i=1..m){ |s(m,i)| * A[c^i][n] } / m!
和f(m)的表达式只有符号不同
注意f(m)表示的是在全部（非恒等的）变换下两两不等的方案数，
而g(m)表示的是在全部变换下不会变相同的方案数。
这个规律没有普遍性。
*/
#include <cstdio>
#include <cmath>
using namespace std;

typedef long long ll;
const int MAXN = 4001;
const ll MODN = 1000000007;
class CountTables{
public:
	ll s[MAXN][MAXN]; //第一类Stirling数
	//s[n,k]
	CountTables(){
		int n, k;
		s[1][1] = 1;
		for(n = 2; n < MAXN; n ++){
			for(k = 1; k <= n; k ++){
				s[n][k] = (s[n - 1][k - 1] - (n - 1) * s[n - 1][k]) % MODN;
				if(s[n][k] < 0)
					s[n][k] += MODN;
			}
		}
	}
	//A[n,k]
	ll aray(int n, int k){
		ll ans = 1;
		if(n < k) n += MODN;
		for(; k > 0; n --, k --)
			ans = ans * n % MODN;
		return ans;
	}
public:
	int howMany(int N, int M, int C){
		ll way = 1, ans = 0;
		int i;

		for(i = 1; i <= M; i ++){
			way = way * C % MODN;
			ans = (ans + s[M][i] * aray(way, N)) % MODN;
		}
		return ans % MODN;
	}
};


////////////////////

CountTables ct;

int main(){
	int n, m, c, ans, my;
	while(scanf("%d%d%d%d", &n, &m, &c, &ans) != EOF){
		my = ct.howMany(n, m, c);
		if(my == ans){
			puts("OK");
		}else{
			printf("%d expected but %d found.\n", ans, my);
		}
	}
	return 0;
}
