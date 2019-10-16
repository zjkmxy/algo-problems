#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int R, C, mst;
int bitcnt[1 << 16], len[200];
char str[200][20];
int dp[2][1 << 16];

bool domain(){
	int i, j, cur, pre, score;
	int rd, wt, st, nst;

	if(scanf("%d%d", &R, &C) == EOF)
		return false;
	for(i = 0; i < R; i ++){
		scanf("%s", str[i]);
		len[i] = strlen(str[i]);
	}
	mst = (1 << C);

	memset(dp, -1, sizeof(dp));
	rd = 0, wt = 1;
	dp[rd][0] = 0;
	for(i = 0; i < R; i ++){
		//对每一行，逐格DP
		for(j = 0; j < C; j ++, rd ^= 1, wt ^= 1){
			fill(dp[wt], dp[wt] + mst, -1);

			for(st = 0; st < mst; st ++){
				if(dp[rd][st] == -1)
					continue;

				//当前行的字符
				cur = bitcnt[st & ((1 << j) - 1)];
				//前一行的字符
				if((st & (mst >> 1)) && (i > 0)){
					pre = len[i - 1] - bitcnt[st >> j];
				} else{
					pre = -1;
				}
				//新的状态
				nst = (st << 1) & (mst - 1);

				//放了
				if(cur < len[i]){
					score = 0;
					if(cur > 0 && (st & 1) && str[i][cur] == str[i][cur - 1]){
						score += 2;
					}
					if(pre >= 0 && str[i - 1][pre] == str[i][cur]){
						score += 2;
					}
					dp[wt][nst | 1] = max(dp[wt][nst | 1], dp[rd][st] + score);
				}

				//不放
				dp[wt][nst] = max(dp[wt][nst], dp[rd][st]);
			}
		}

		//清理某一行没填完的状态
		for(st = 0; st < mst; st ++){
			if(bitcnt[st] != len[i])
				dp[rd][st] = -1;
		}
	}

	cur = 0;
	for(st = 0; st < mst; st ++)
		cur = max(cur, dp[rd][st]);
	printf("%d\n", cur);

	return true;
}

int main(){
	int i;
	for(i = 1; i < (1 << 16); ++ i)
		bitcnt[i] = bitcnt[i >> 1] + (i & 1);
	while(domain());
	return 0;
}