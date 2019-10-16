#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-result"
#include <cstdio>  
#include <cstdlib>
#include <cmath>
#include <algorithm>  
using namespace std;  

//模大整数余数类
struct ul{
	static const unsigned long MODN = 1000000009;
	static const unsigned long DIV2 = 500000005;
	unsigned long n;
	ul(unsigned long n = 0):n(n){};
	inline friend ul operator+(const ul& a, const ul& b){
		unsigned long r = a.n + b.n;
		return (r < a.n || r >= MODN) ? ul(r - MODN) : ul(r);
	}
	inline friend ul operator-(const ul& a, const ul& b){
		return (a.n >= b.n) ? ul(a.n - b.n) : ul(a.n + MODN - b.n);
	}
	inline friend ul operator*(const ul& a, const ul& b){
		unsigned long long r = (unsigned long long)(a.n) * (unsigned long long)(b.n);
		return ul(r % MODN);
	}
	inline ul half()const{
		return (*this) * DIV2;
	}
};

const int MAXN = 100005;
ul cnt[9];				//选取某些牌，使得余数为x的方法数
static ul tab[9][9][9];	//逆运算的矩阵的表
int arr[MAXN];			//每一张的卡牌
int N, M;

//预先打好逆运算的表
void inittab(){
	int i, j, p, ed;
	for(i = 1; i < 9; i ++){
		for(j = 0; j < 9; j ++){
			ed = (j + i) % 9;
			for(p = ed; p != j; p = (p + i + i) % 9)
				tab[i][j][(p + i) % 9] = 2;
			if(i == 3 || i == 6){
				for(p = 0; p < 9; p ++){
					if((p - j + 3) % 3 != 0){
						tab[i][j][p] = 1;
					}
				}
			}
		}
	}
}

//添加某一张牌
void add(int val){
	int i, j;
	ul tmp[9];
	for(i = 0, j = val; i < 9; i ++, j = (j + 1) % 9)
		tmp[j] = cnt[i] + cnt[j];
	tmp[val] = tmp[val] + ul(1);
	copy(tmp, tmp + 9, cnt);
}

//减少某一张牌
void substract(int val){
	int i, j;
	cnt[val] = cnt[val] - 1;
	if(val == 0){
		for(i = 0; i < 9; i ++)
			cnt[i] = cnt[i].half();
	}else{
		ul tmp[9], sum = 0;
		for(i = 0; i < 9; i ++)
			sum = sum + cnt[i];
		fill(tmp, tmp + 9, sum);
		for(i = 0; i < 9; i ++){
			for(j = 0; j < 9; j ++)
				tmp[i] = tmp[i] - tab[val][i][j] * cnt[j];
			tmp[i] = tmp[i].half();
		}
		copy(tmp, tmp + 9, cnt);
	}
}

int main(){
	int i, a, b;
	inittab();
	scanf("%d%d", &N, &M);
	for(i = 1; i <= N; i ++){
		scanf("%u", &arr[i]);
		arr[i] %= 9;
		add(arr[i]);
	}
	printf("%lu\n", cnt[0].n);
	while(M --){
		scanf("%d%d", &a, &b);
		b %= 9;
		if(arr[a] != b){
			substract(arr[a]);
			arr[a] = b;
			add(arr[a]);
		}
		printf("%lu\n", cnt[0].n);
	}
	return 0;
}