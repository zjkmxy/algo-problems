#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 105
double dp[MAXN][MAXN][MAXN];
int r, s, p;

//a>b>c>a的条件下的计算
//dp[a][b][c]时a的胜率
double dpcalc(int a, int b, int c){
	int i, j, k;
	double tmp;
	memset(dp, 0, sizeof(dp));
	dp[0][0][0] = 1.0;
	for(i = 1; i <= a; i ++){
		for(j = 0; j <= b; j ++){
			for(k = 0; k <= c; k ++){
				tmp = i * j + j * k + k * i;
				if(j > 0){
					//ab相遇
					dp[i][j][k] += (i * j) / tmp * dp[i][j - 1][k];
				}
				if(j > 0 && k > 0){
					//bc相遇
					dp[i][j][k] += (j * k) / tmp * dp[i][j][k - 1];
				}
				if(k > 0){
					//ca相遇
					dp[i][j][k] += (k * i) / tmp * dp[i - 1][j][k];
				}
				if(j == 0 && k == 0)
					dp[i][j][k] = 1.0;
			}
		}
	}
	return dp[a][b][c];
}

int main(){
	double pr, ps, pp;
	while(scanf("%d%d%d", &r, &s, &p) != EOF){
		pr = dpcalc(r, s, p);
		ps = dpcalc(s, p, r);
		pp = dpcalc(p, r, s);
		printf("%.6lf %.6lf %.6lf\n", pr, ps, pp);
	}
	return 0;
}