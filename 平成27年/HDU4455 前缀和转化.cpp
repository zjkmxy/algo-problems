#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 1000005;

//////////////////////////////////////////////////////////////////////////
//�����
inline char getc(){
	static const int BUFLEN = 1 << 15;
	static char B[BUFLEN], *S = B, *T = B;
	if(S == T){
		S = B;
		T = S + fread(B, 1, BUFLEN, stdin);
	}
	return (S == T) ? 0 : *(S ++);
}
int ReadInt(){
	char ch;
	do ch = getc(); while(!isdigit(ch));
	int aa = ch - '0';
	for(ch = getc(); isdigit(ch); ch = getc())
		aa = aa * 10 + ch - '0';
	return aa;
}
//////////////////////////////////////////////////////////////////////////

int arr[MAXN];
ll ans[MAXN];
int N, M, Q;

//ֻ��ֵΪv��Ԫ��
void DealVal(int v){
	static ll curans[MAXN];
	int i, last = -1;
	ll sum;

	fill(curans, curans + N + 2, 0);
	for(i = 0; i < N; i ++){
		if(arr[i] == v)
			last = i;
		if(last >= 0){
			//ʵ������i-last�Լ���С��wʲô���޷���ã����Ǵ���һֱ��i+1�����Ի��v����Ԫ�ص�����
			curans[i + 2] --;
			curans[i - last + 1] ++;
		}
	}
	sum = 0;
	for(i = 0; i <= N; i ++){
		sum += curans[i];
		curans[i] = sum;
		ans[i] += curans[i];
	}
}

//�����Deal
void Deal(){
	static ll curans[MAXN];
	static int last[MAXN];
	int i, v;
	ll sum;

	fill(last, last + M + 1, -1);
	fill(curans, curans + N + 2, 0);
	for(i = 0; i < N; i ++){
		if(last[arr[i]] != -1){
			//�����ϴ�lastֱ�����ڵ�ֵ
			//����last+2..i+1ȫ����-1
			curans[last[arr[i]] + 2] --;
			curans[i + 2] ++;
			//�Լ�1..i-lastȫ����+1
			curans[1] ++;
			curans[i - last[arr[i]] + 1] --;
		}
		last[arr[i]] = i;
	}
	//������󣬽��ȫ����last��N
	for(v = 1; v <= M; v ++){
		if(last[v] != -1){
			//�����ϴ�lastֱ�����ڵ�ֵ
			//����last+2..N+1ȫ����-1
			curans[last[v] + 2] --;
			curans[N + 2] ++;
			//�Լ�1..N-lastȫ����+1
			curans[1] ++;
			curans[N - last[v] + 1] --;
		}
		last[v] = -1;
	}

	//�õ���ǰ��curans
	sum = 0;
	for(i = 0; i <= N + 1; i ++){
		sum += curans[i];
		curans[i] = sum;
	}

	//������ǰ�Ҹ�ĸ���ans
	sum = 0;
	for(i = 0; i <= N + 1; i ++){
		sum += curans[i];
		ans[i] = sum;
	}
}

bool domain(){
	int i, a;

	N = ReadInt();
	if(N == 0)
		return false;
	for(i = 0; i < N; i ++){
		arr[i] = ReadInt() + 1;
	}
	M = MAXN - 2;

	fill(ans, ans + N + 2, 0);
	//for(i = 1; i <= M; i ++)
	//	DealVal(i);
	Deal();

	Q = ReadInt();
	while(Q --){
		a = ReadInt();
		printf("%lld\n", ans[a]);
	}

	return true;
}

int main(){
	while(domain());
	return 0;
}
