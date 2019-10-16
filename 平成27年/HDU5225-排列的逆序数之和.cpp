#include <cstdio>
#include <cstdlib>
#include <algorithm>

typedef long long ll;
const int MAXN = 2005;
const ll MODN = 1000000007;
int arr[MAXN], N;
ll cntf[MAXN], cntb[MAXN];	//从前和从后各自小于的个数
ll fac[MAXN], inv[MAXN];

ll ModExp(ll x, int p){
	ll ret = 1;
	for(; p; p >>= 1){
		if(p & 1)
			ret = ret * x % MODN;
		x = x * x % MODN;
	}
	return ret;
}

void Init(){
	int i;
	fac[0] = 1;
	for(i = 1; i < MAXN; i ++)
		fac[i] = fac[i - 1] * i % MODN;
	inv[0] = 1;
	for(i = 1; i < MAXN; i ++)
		inv[i] = ModExp(fac[i], MODN - 2);
}

void Regular(){
	int i, j;
	for(i = 1; i <= N; i ++){
		for(j = i + 1; j <= N; j ++){
			if(arr[j] > arr[i])
				arr[j] --;
		}
	}
}

void GetCnt(){
	int i;
	cntf[0] = 0;
	cntf[1] = arr[1] - 1;
	for(i = 2; i <= N; i ++){
		cntf[i] = (cntf[i - 1] * (N - i + 1) % MODN + (arr[i] - 1)) % MODN;
	}
	cntb[N + 1] = 0;
	cntb[N] = arr[N] - 1;
	for(i = N - 1; i > 0; i --){
		cntb[i] = ((arr[i] - 1) * fac[N - i] % MODN + cntb[i + 1]) % MODN;
	}
}

ll DealPoint(int pos, int val){
	ll ret = 0;
	//之前小，之后随便取
	ret = (ret + cntf[pos - 1] * fac[N - pos] % MODN) % MODN;
	//之前等
	if(val < arr[pos]){
		//本位小，随便取
		ret = (ret + fac[N - pos]) % MODN;
	}else if(val == arr[pos]){
		//本位等，之后只能取小
		ret = (ret + cntb[pos + 1]) % MODN;
	}
	return ret * (val - 1) % MODN;
}

ll Calc(){
	int i, j;
	ll ret = 0;
	for(i = 1; i <= N; i ++){
		for(j = 1; j <= N + 1 - i; j ++){
			ret = (ret + DealPoint(i, j)) % MODN;
		}
	}
	return ret;
}

int main(){
	int i;
	Init();
	while(scanf("%d", &N) != EOF){
		for(i = 1; i <= N; i ++)
			scanf("%d", &arr[i]);
		Regular();
		GetCnt();
		printf("%lld\n", Calc());
	}
	return 0;
}