#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <map>
using namespace std;

typedef unsigned long DWORD;
const int MAXN = 8;
const int MODN = 1000000007;
map<DWORD, int> F[2][MAXN + 1];
DWORD block[MAXN];
int N;

namespace DP{
	int ans;
	int dp[2][MAXN];

	inline void unfold(int ans, DWORD st){
		int i;
		DP::ans = ans;
		for(i = 0; i < N; i ++, st >>= 4)
			dp[0][i] = st & 0x0F;
	}
	inline DWORD fold(){
		int i;
		DWORD ans;
		for(i = N - 1, ans = 0; i >= 0; i --){
			ans <<= 4;
			ans |= dp[1][i];
		}
		return ans;
	}
	//1ÊÇºÚ0ÊÇ°×
	inline int dpcalc(DWORD mp){
		int nans = ans;
		for(int i = 0; i < N; i ++){
			if((mp >> i) & 1){
				dp[1][i] = 0;
			}else{
				dp[1][i] = min(dp[0][i], i);
				if(i > 0){
					dp[1][i] = min(dp[1][i], min(dp[1][i - 1], dp[0][i - 1]));
				}
				dp[1][i] ++;
				nans = max(nans, dp[1][i]);
			}
		}
		return nans;
	}
};

void solve(){
	const int S = (1 << N);
	int i, a, rd = 0, wt = 1, st;

	for(a = 0; a <= N; a ++)
		F[rd][a].clear();
	F[rd][0].insert(make_pair(0, 1));

	for(i = 0; i < N; i ++, rd ^= 1, wt ^= 1){
		for(a = 0; a <= N; a ++)
			F[wt][a].clear();
		for(a = 0; a <= N; a ++){
			for(const auto& it : F[rd][a]){
				DP::unfold(a, it.first);
				for(st = 0; st < S; st ++){
					if((st & block[i]) != block[i])
						continue;
					int nans = DP::dpcalc(st);
					int& tmp = F[wt][nans][DP::fold()];
					tmp = (tmp + it.second) % MODN;
				}
			}
		}
	}

	for(a = 0; a <= N; a ++){
		int ans = 0;
		for(const auto& it : F[rd][a]){
			ans = (ans + it.second) % MODN;
		}
		printf("%d\n", ans);
	}
}

int main(){
	int T, i, j, tmp;
	char str[10];
	scanf("%d", &T);
	while(T --){
		scanf("%d", &N);
		for(i = 0; i < N; i ++){
			scanf("%s", str);
			tmp = 0;
			for(j = N - 1; j >= 0; j --)
				tmp = (tmp << 1) | (str[j] == 'o' ? 0 : 1);
			block[i] = tmp;
		}
		solve();
	}
	return 0;
}