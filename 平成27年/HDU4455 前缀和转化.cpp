#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 1000005;

//////////////////////////////////////////////////////////////////////////
//读入挂
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

//只管值为v的元素
void DealVal(int v){
	static ll curans[MAXN];
	int i, last = -1;
	ll sum;

	fill(curans, curans + N + 2, 0);
	for(i = 0; i < N; i ++){
		if(arr[i] == v)
			last = i;
		if(last >= 0){
			//实际上是i-last以及更小的w什么都无法获得，但是此上一直到i+1都可以获得v这种元素的增益
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

//集体大Deal
void Deal(){
	static ll curans[MAXN];
	static int last[MAXN];
	int i, v;
	ll sum;

	fill(last, last + M + 1, -1);
	fill(curans, curans + N + 2, 0);
	for(i = 0; i < N; i ++){
		if(last[arr[i]] != -1){
			//处理上次last直到现在的值
			//包括last+2..i+1全部的-1
			curans[last[arr[i]] + 2] --;
			curans[i + 2] ++;
			//以及1..i-last全部的+1
			curans[1] ++;
			curans[i - last[arr[i]] + 1] --;
		}
		last[arr[i]] = i;
	}
	//最后的最后，解决全部的last到N
	for(v = 1; v <= M; v ++){
		if(last[v] != -1){
			//处理上次last直到现在的值
			//包括last+2..N+1全部的-1
			curans[last[v] + 2] --;
			curans[N + 2] ++;
			//以及1..N-last全部的+1
			curans[1] ++;
			curans[N - last[v] + 1] --;
		}
		last[v] = -1;
	}

	//得到先前的curans
	sum = 0;
	for(i = 0; i <= N + 1; i ++){
		sum += curans[i];
		curans[i] = sum;
	}

	//根据先前乱搞的更新ans
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
