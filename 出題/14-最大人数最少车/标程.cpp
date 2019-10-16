#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>
using namespace std;

const int MAXN = 2005;
int C, T, S, E;
int arr[MAXN], sum[MAXN];
int dp[MAXN];	//��ʱ��tΪ���һ�η�����һ����װ�ص��������
int MaxPeople, MinCarry, FirstTime;

int GetTime(){
	int h, m;
	scanf("%d:%d", &h, &m);
	return h * 60 + m;
}

void PutTime(int t){
	printf("%02d:%02d", t / 60, t % 60);
}

//�����������������������ٳ�
void solve1(){
	int acc = 0;	//�ۻ���ʷʹ����
	int tim;

	MaxPeople = 0;
	MinCarry = 0;
	for(tim = E; tim >= 0; tim -= T){
		if(sum[tim] - acc <= C){
			//һ��δ����ֱ���˳�
			MaxPeople += sum[tim] - acc;
			if(sum[tim] - acc > 0)
				MinCarry ++;
			break;
		} else{
			//���أ������ۼ�ֵ��������
			MaxPeople += C;
			MinCarry ++;
			acc += C;
			acc -= sum[tim] - sum[tim - T];
			acc = max(acc, 0);
		}
	}
}

//������һ����ʱ�䣬�Լ��������
void solve2(){
	int tim;
	for(tim = 0; tim < T; tim ++){
		dp[tim] = min(sum[tim], C);
	}
	for(tim = T; tim <= E; tim ++){
		dp[tim] = min(sum[tim], dp[tim - T] + C);
	}
	FirstTime = E;
	for(tim = 0; tim <= E; tim ++){
		if(dp[tim] == MaxPeople){
			FirstTime = tim;
			break;
		}
	}
	assert(dp[E] == MaxPeople);
}

int main(){
	int n, t, a;

	scanf("%d%d%d", &n, &C, &T);
	S = GetTime(), E = GetTime();
	while(n --){
		t = GetTime();
		scanf("%d", &a);
		if(t <= S){
			arr[0] += a;
		} else{
			arr[t - S] += a;
		}
	}
	E -= S;
	E -= T;
	T *= 2;
	sum[0] = arr[0];
	for(t = 1; t <= E; t ++){
		sum[t] = sum[t - 1] + arr[t];
	}
	if(E >= 0){
		solve1();
		solve2();
		printf("%d %d\n", MaxPeople, MinCarry);
		PutTime(S + FirstTime + T / 2);
		printf("\n");
	} else{
		printf("0 0\n");
		PutTime(S);
		printf("\n");
	}

	return 0;
}