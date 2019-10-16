/*
题目大意：
要求输入一篇N个字符的文章，对所有非负整数i：
每到第i+0.1秒时可以输入一个文章字符
每到第i+0.9秒时有P的概率崩溃（回到开头或者上一个存盘点）
每到第i秒有一次机会可以选择按下X个键存盘，或者不存
打印完整篇文章之后必须存盘一次才算完成
输入多组N,P,X选择最佳策略使得输入完整篇文章时候按键的期望最小，输出此期望

题目分析：
1.设F[n]表示连续输入n个字符不存盘的期望按键数。
F[0]=0；用期望DP倒推可以得到一个O(n)的公式一路计算F[1]..F[n]，不表。
特别注意这里F是单调递增的，而且是凹函数
即对a > b >= c > d，有：F[a] + F[d] > F[b] + F[c]
这个性质是之后优化的关键

2.设dp[i]表示输入长度为i的文章所需的最小期望按键数，显见有
dp[0] = 0
dp[i] = min{dp[i-j] + F[j] + X}  j < i
dp[i] = min{F[i] + X, dp[i-j] + dp[j]}  j < i
后两个方程二选一即可

3.再设s为最小的需要存盘才最优的点，即dp[s] < F[s] + X。显然s存在（可能比N大）。
那么对于所有k>=s，一定有：dp[k] < F[k] + X
证：
不妨设：dp[s] = F[s-s'] + F[s'] + 2X，s >= 2 * s'
dp[k] <= dp[k-s'] + F[s'] + X <= F[k-s'] + F[s'] + 2X
注意：F[k] + F[s-s'] > F[s] + F[k-s']
带入上式得：
dp[k] < F[k] + F[s-s'] + F[s'] - F[s] + 2X = F[k] + dp[s] - F[s] < F[k] + X

4.设s,s'如上，有s' = floor(s / 2)
证：由凹函数不等式，dp[s]取得最优值时s'和(s-s')至多相差1，因此必有上式。

5.dp[n]可以展开到m个dp[pi]相加的和，对任意的pi和pj，二者最多相差1，且每一个都不大于s
证：
如若不然，设pi < pj - 1，令pi增加1，pj减少1，结果更优，与dp[n]最优矛盾
如果某一个pi大于s，直接利用dp[s]的转移点拆开dp[pi]即可
注意到对以上所有的pi，或者pi=s，或者dp[pi]=F[pi]+X

算法设计：
1.首先计算s和所有i<=s的F[i]。为方便令F[s]=dp[s]-X，其他的保留
2.通过枚举dp[N]展开为dp[pi]的pi，不计算dp数组直接计算dp[N]，具体见代码
此算法时间复杂度只和P与X有关而和N无关，总时间效率为O(s)。s很小故为近似O(1)。
*/
#include <cstdio>
#include <limits>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXS = 30;  //P = 0.1, X = 100时S最大为28，随机数据S一般不超过10
int N, X, S;
double P, F[MAXS];

//计算F数组O(S)
void CalcF(){
	int i;
	double a = 1, b = 1. / (P - 1.);
	S = numeric_limits<int>::max();
	F[0] = 0.;
	F[1] = - b / a;
	for(i = 2; i <= N && i <= S; i ++){
		a = (a - P) / (1 - P);
		b = (b - 1) / (1 - P);
		F[i] = - b / a;
		if(F[i] > F[i >> 1] + F[i - (i >> 1)] + X){
			F[i] = F[i >> 1] + F[i - (i >> 1)] + X;
			S = i;
		}
	}
}

//暴力的求解方式O(N*S)
double BFSolve(){
	static double dp[MAXN];
	int i, j;
	dp[0] = 0;
	for(i = 1; i <= N; i ++){
		if(i <= S){
			dp[i] = F[i] + X;
		}else{
			dp[i] = numeric_limits<double>::max();
		}
		for(j = 1; j <= S && j < i; j ++){
			dp[i] = min(dp[i], dp[i - j] + dp[j]);
		}
	}
	return dp[N];
}

//改进的计算方法O(S)
double Solve(){
	int i, a, b;
	double ans = numeric_limits<double>::max();
	if(N < S)
		return F[N] + X;
	for(i = 1; i <= S; i ++){
		a = N / i;
		b = N % i;
		//注意N,a,b既可以理解成N = a * i + b也可以理解成N = (a + 1) * i - (i - b)
		//两种不同的理解方式导致了以下两种不同的分组方式
		if(a >= b && i < S){
			//a-b个i加上b个(i+1)
			ans = min(ans, (a - b) * F[i] + b * F[i + 1] + a * X);
		}
		if(a + b + 1 >= i){
			//(i-b)个(i-1)加上(a+1-(i-b))个i
			ans = min(ans, (a + 1 - i + b) * F[i] + (i - b) * F[i - 1] + (a + 1) * X);
		}
	}
	return ans;
}

int main(){
	int T, i;
	scanf("%d", &T);
	for(i = 1; i <= T; i ++){
		scanf("%d%lf%d", &N, &P, &X);
		CalcF();
		printf("Case #%d: %.6lf\n", i, Solve());
	}
	return 0;
}
