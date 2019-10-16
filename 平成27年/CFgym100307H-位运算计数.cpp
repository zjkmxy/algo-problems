/*
	Xor[s,i] = And[s, i]
=>	Xor[i] = And[s, i] ^ Xor[s - 1]
存贮等号右边的结果
对同一个s，如果And[s, i]的某一位变为0，对大于i的j，And[s, j]的同一位不会再变为1
因此如果遇到能将其变为0的，修正等号右边的结果即可。每个元素每一位只需要变1次
*/
#include <cstdio>
#include <cstdlib>
#include <map>
using namespace std;

typedef unsigned long UINT;
typedef unsigned long long ULL;
const int MAXN	= 100000;
const int BITS	= 32;
const UINT MASK	= 0xffffffff;

//对于每一个s <= i，将Xor前缀和s异或
map<UINT, int> Count;
//输入序列
UINT arr[MAXN + 1];
//后缀与异或前缀和序列
UINT andXor[MAXN + 1];
//对每一位，最后一个为0的点
int lastZero[BITS];
int n;

//必须清除结果，以保证足够的空间
inline void update(UINT val, int cnt){
	Count[val] += cnt;
	if(Count[val] <= 0){
		Count.erase(val);
	}
}

ULL calc(){
	int i, j, k, mask;
	UINT curXor = 0;
	ULL ret = 0;

	for(i = 1; i <= n; i ++){
		//整理之前的And结果
		for(j = 0; j < BITS; j ++){
			mask = (1 << j);
			if(!(arr[i] & mask)){
				for(k = lastZero[j] + 1; k < i; k ++){
					update(andXor[k], -1);
					andXor[k] ^= mask;
					update(andXor[k], +1);
				}
				lastZero[j] = i;
			}
		}
		//计数答案（+1是算上自己）
		curXor ^= arr[i];
		ret += Count[curXor] + 1;
		//为下一轮准备
		andXor[i] = curXor ;
		update(andXor[i], +1);
	}
	return ret;
}

int main(){
#ifndef _DEBUG
	freopen("hack.in", "r", stdin);
	freopen("hack.out", "w", stdout);
#endif
	int i;
	scanf("%d", &n);
	for(i = 1; i <= n; i ++){
		scanf("%lu", &arr[i]);
	}
	printf("%I64u\n", calc());
	return 0;
}
