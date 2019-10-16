/*
需要高精度模板无法AC，而且CLJ标程都WA我也没办法
题意：给出N个球，每次随机把[L,R]内的球染黑，求全部染黑的期望次数
题解：用容斥原理枚举直到最后还是白色的球的集合数，考虑它们的贡献。
*/
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN  = 51;
//i是最后一个白球的位置，j是选中白球的个数，w是在这些球不能然的前提下还可以选择的区间个数，dp表示选择白球的方案个数（不是区间选择的方案个数）
ll dp[MAXN][2][MAXN*MAXN];  
int N, M;
double ans;

inline int C2(int n){
	return (n * (n + 1)) >> 1;
}

double calc(int j, int w){
	double p = static_cast<double>(w) / static_cast<double>(M);
	if(1 - p < 1e-10)
		p = 0.;
	else
		p = 1. / (1. - p);
	if(j == 0)
		return - p;
	else
		return p;
}

void dpCalc(){
	int i, j, k, w;
	memset(dp, 0, sizeof(dp));
	dp[0][0][0] = 1;
	for(i = 0; i < N; i ++){
		for(j = 0; j <= 1; j ++){
			for(w = 0; w <= M; w ++){
				for(k = i + 1; k <= N; k ++){
					dp[k][j ^ 1][w + C2(k - i - 1)] += dp[i][j][w];
				}
			}
		}
	}
	ans = 0.;
	for(i = 0; i <= N; i ++){
		if(i == N)
			i = i;
		for(j = 0; j <= 1; j ++){
			for(w = 0; w <= M; w ++){
				if(dp[i][j][w] == 0)
					continue;
				ans += calc(j, w + C2(N - i)) * dp[i][j][w];
			}
		}
	}
}

int main(){
	int t;
	scanf("%d", &t);
	while(t --){
		scanf("%d", &N);
		M = C2(N);
		dpCalc();
		printf("%.15lf\n", ans);
	}
	return 0;
}
