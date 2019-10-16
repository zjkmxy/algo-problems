/*
	Xor[s,i] = And[s, i]
=>	Xor[i] = And[s, i] ^ Xor[s - 1]
�����Ⱥ��ұߵĽ��
��ͬһ��s�����And[s, i]��ĳһλ��Ϊ0���Դ���i��j��And[s, j]��ͬһλ�����ٱ�Ϊ1
�����������ܽ����Ϊ0�ģ������Ⱥ��ұߵĽ�����ɡ�ÿ��Ԫ��ÿһλֻ��Ҫ��1��
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

//����ÿһ��s <= i����Xorǰ׺��s���
map<UINT, int> Count;
//��������
UINT arr[MAXN + 1];
//��׺�����ǰ׺������
UINT andXor[MAXN + 1];
//��ÿһλ�����һ��Ϊ0�ĵ�
int lastZero[BITS];
int n;

//�������������Ա�֤�㹻�Ŀռ�
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
		//����֮ǰ��And���
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
		//�����𰸣�+1�������Լ���
		curXor ^= arr[i];
		ret += Count[curXor] + 1;
		//Ϊ��һ��׼��
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
