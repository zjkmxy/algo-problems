#include <cstdio>
#include <set>
#include <list>
#include <algorithm>
#include <cctype>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 16;
const int MAXS = (1 << MAXN);
const int MODN = 1000000007;
char str[MAXN+1];
int F[2][MAXS];
int ans[MAXN];
int N, M, S;
vector<int> got[MAXS];

int dp[2][MAXN];

inline void decode(int st){
	int i, t;
	dp[0][0] = 0;
	for(i = 0; i < N; i ++)
		dp[0][i + 1] = dp[0][i] + ((st >> i) & 1);
}

inline int encode(){
	int i, st = 0;
	for(i = 0; i < N; i ++){
		st |= (dp[1][i + 1] - dp[1][i]) << i;
	}
	return st;
}

inline void dpcalc(char ch){
	dp[1][0] = 0;
	for(int i = 1; i <= N; i ++){
		if(ch == str[i]){
			dp[1][i] = dp[0][i - 1] + 1;
		}else{
			dp[1][i] = max(dp[0][i], dp[1][i - 1]);
		}
	}
}

inline char getv(char ch){
	switch(ch){
	case 'A':
		return 0;
	case 'G':
		return 1;
	case 'C':
		return 2;
	default:
		return 3;
	}
}

void CalcF(){
	int rd = 0, wt = 1, i, st, nst, K;
	char ch;

	S = (1 << N);

	for(st = 0; st < S; st ++){
		got[st].clear();
		decode(st);
		for(ch = 0; ch < 4; ch ++){
			dpcalc(ch);
			nst = encode();
			got[st].push_back(nst);
		}
	}

	memset(F, 0, sizeof(F));
	F[0][0] = 1;
	for(i = 1; i <= M; i ++, rd ^= 1, wt ^= 1){
		memset(F[wt], 0, sizeof(F[wt]));
		for(st = 0; st < S; st ++){
			if(F[rd][st] == 0)
				continue;
			for(auto nst : got[st])
				F[wt][nst] = (F[wt][nst] + F[rd][st]) % MODN;
		}
	}

	memset(ans, 0, sizeof(ans));
	for(st = 0; st < S; st ++){
		if(F[rd][st] == 0)
			continue;
		decode(st);
		K = dp[0][N];
		ans[K] = (ans[K] + F[rd][st]) % MODN;
	}
}

int main(){
	int T, i;
	scanf("%d", &T);
	while(T --){
		scanf("%s%d", str + 1, &M);
		N = strlen(str + 1);
		for(i = 1; i <= N; i ++)
			str[i] = getv(str[i]);
		CalcF();
		for(i = 0; i <= N; i ++)
			printf("%d\n", ans[i]);
	}
	return 0;
}
