#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXV = 80010;
const int BASE = 40010;
const int MAXN = 200;
const LL MODN = 1000000007;
int arr[MAXN];
LL dp[2][MAXV], delta[MAXV], step, acc;
int N, V;

void odd(int st, int len, LL val){
	delta[st] = ((delta[st] + val) % MODN + MODN) % MODN;
	delta[st + (len << 1)] = ((delta[st + (len << 1)] - (val << 1)) % MODN + MODN) % MODN;
	delta[st + (len << 2)] = ((delta[st + (len << 2)] + val) % MODN + MODN) % MODN;
}

void even(int st, int len, LL val){
	delta[st] = ((delta[st] + val) % MODN + MODN) % MODN;
	delta[st + (len << 1)] = ((delta[st + (len << 1)] - val) % MODN + MODN) % MODN;
	delta[st + (len << 1) + 2] = ((delta[st + (len << 1) + 2] - val) % MODN + MODN) % MODN;
	delta[st + (len << 2) + 2] = ((delta[st + (len << 2) + 2] + val) % MODN + MODN) % MODN;
}

LL dpcalc(){
	int i, j, rd, wt, s, len;

	V = 1;
	rd = 0, wt = 1;
	memset(dp[rd], 0, sizeof(dp[rd]));
	dp[0][BASE] = 1;
	for(i = 1; i <= N; i ++){
		s = arr[i];
		V += s;
		memset(dp[wt], 0, sizeof(dp[wt]));
		memset(delta, 0, sizeof(delta));
		for(j = - V; j <= V; j ++){
			if(dp[rd][BASE + j] == 0)
				continue;

			len = (s >> 1) + 1;
			odd(BASE + j - (len << 1), len, dp[rd][BASE + j]);
			len = (s + 1) >> 1;
			even(BASE + j - (len << 1) - 1, len, dp[rd][BASE + j]);
		}

		step = acc = 0;
		for(j = - V - 4; j <= V + 4; j += 2){
			acc = (acc + step) % MODN;
			dp[wt][BASE + j] = acc;
			step = (step + delta[BASE + j]) % MODN;
		}
		step = acc = 0;
		for(j = - V - 3; j <= V + 3; j += 2){
			acc = (acc + step) % MODN;
			dp[wt][BASE + j] = acc;
			step = (step + delta[BASE + j]) % MODN;
		}

		rd ^= 1, wt ^= 1;
	}
	return dp[rd][BASE];
}

int main(){
	int T, i;
	scanf("%d", &T);
	while(T --){
		scanf("%d", &N);
		for(i = 1; i <= N; i ++)
			scanf("%d", &arr[i]);
		printf("%I64d\n", dpcalc());
	}
	return 0;
}