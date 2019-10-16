//Tag: 题意理解 快速幂 小数素性判断
//值得注意的是，本题并不需要选手看懂FFT、FNTT或原根是什么东西
//这并不是最简洁的解法
#include <cstdio>

typedef unsigned long ul;
typedef unsigned long long ull;
bool isnp[65537];
ul prime[6601], pcnt;

//线性筛法取得质数表
void InitPrimeTable(){
	ul i, j, t;
	for(i = 2; i < 65536; i ++){
		if(!isnp[i]){
			prime[pcnt ++] = i;
		}
		for(j = 0; j < pcnt; j ++){
			//此时prime[j]是t的最小质因子
			t = i * prime[j];
			if(t >= 65536)
				break;
			isnp[t] = true;
			if(i % prime[j] == 0)
				break;
		}
	}
}

//x<=2^32，因此可以直接用2^16内的素数判
//比暴力求快一些，不过本题用例不多，也不打算拿这个卡人
bool IsPrime(ul x){
	ul i;
	if(x < 65536)
		return !isnp[x];
	for(i = 0; i < pcnt; i ++){
		if(x % prime[i] == 0)
			return false;
	}
	return true;
}

//快速模幂
ull powk(ull a, ull k, ull p){
	ull ret = 1;
	for(; k; k >>= 1){
		if(k & 1)
			ret = ret * a % p;
		a = a * a % p;
	}
	return ret;
}

int main(){
	ul P, p, k;
	ul t1, t2, a;

	InitPrimeTable();
	while(scanf("%lu", &P) != EOF){
		if(IsPrime(P)){
			for(p = P - 1, k = 0; !(p & 1); p >>= 1, k ++);
			t1 = (P - 1) / 2;
			t2 = (P - 1) / p;
			for(a = 2; powk(a, t1, P) == 1 || powk(a, t2, P) == 1; a ++);
			printf("%lu\n", a);
		} else{
			printf("-1\n");
		}
	}
	return 0;
}