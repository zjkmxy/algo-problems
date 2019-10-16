//Tag: 二分
//这是真正掌握二分思想的人能写出的解法
#include <cstdio>
#include <queue>
using namespace std;

typedef unsigned long long ull;
ull arr[105];
int N;

ull solve(){
	int i, k;
	ull ans = ~(0ull), cur;
	if(N >= 7)
		return 0;
	for(k = 0; k < (1 << N); k ++){
		cur = ~(0ull);
		for(i = 0; i < N; i ++){
			if(k & (1 << i))
				cur &= arr[i];
			else
				cur &= ~arr[i];
		}
		ans = min(ans, cur);
	}
	return ans;
}

int main(){
	int i, t = 0;
	while(scanf("%d", &N) != EOF){
		for(i = 0; i < N; i ++)
			scanf("%llu", &arr[i]);
		printf("Case #%d: %llu\n", ++ t, solve());
	}
	return 0;
}