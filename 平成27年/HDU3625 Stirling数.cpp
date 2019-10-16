#include <cstdio>
#include <cmath>
using namespace std;

typedef long long ll;
const int MAXN = 21;
ll s[MAXN][MAXN], S[MAXN][MAXN];	//第1,2类斯特林数
ll fac[MAXN];

void pre(){
	int n, k;

	fac[0] = 1;
	for(n = 1; n < MAXN; n ++)
		fac[n] = fac[n - 1] * n;

	//将N个数分成K个循环排列的个数
	s[1][1] = 1;
	for(n = 2; n < MAXN; n ++){
		for(k = 1; k <= n; k ++){
			s[n][k] = s[n - 1][k - 1] + s[n - 1][k] * (n - 1);
		}
	}

	//将N个数分成K个不相交非空子集的个数
	S[1][1] = 1;
	for(n = 2; n < MAXN; n ++){
		for(k = 1; k <= n; k ++){
			S[n][k] = S[n - 1][k - 1] + S[n - 1][k] * k;
		}
	}
}

int main(){
	int T, N, K, i;
	ll cnt;

	pre();
	scanf("%d", &T);
	while(T --){
		scanf("%d%d", &N, &K);
		cnt = 0;
		//至多k个环
		for(i = 1; i <= K; i ++)
			cnt += s[N][i] - s[N - 1][i - 1];	//减去1单独成环的情况
		printf("%.4lf\n", 1.0 * cnt / fac[N]);
	}
	return 0;
}