#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

typedef unsigned long ll;
const int MAXN = 262144;
int N, M;

inline ll ml(ll a, ll b, ll m){
	ll ret;
	__asm__ __volatile__("\tmull %%ebx\n\tdivl %%ecx\n" :"=d"(ret) : "a"(a), "b"(b), "c"(m));
	return ret;
}

struct Segtree{
	ll val[MAXN];
	int n;

	void build(int N){
		for(n = 1, N ++; N > 0; N >>= 1, n <<= 1);
		for(int i = 1; i < (n << 1); i ++)
			val[i] = 1;
	}
	//°Ñp¸Ä³Év
	void update(int p, int v){
		p += n;
		val[p] = v % M;
		for(p >>= 1; p; p >>= 1)
			val[p] = ml(val[p << 1], val[p << 1 | 1], M);
	}
	inline ll get(){
		return val[1];
	}
}segt;

char B[1 << 15], *S = B, *T = B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
	while(ch = getc(), !isdigit(ch) && ch);
	if(!ch) return 0;
	int aa = ch - '0';
	while(ch = getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
	return aa;
}

int main(){
	int T, cas, i;
	int op, v;

	T = ReadInt();
	for(cas = 1; cas <= T; cas ++){
		printf("Case #%d:\n", cas);
		N = ReadInt(), M = ReadInt();
		segt.build(N);
		for(i = 1; i <= N; i ++){
			op = ReadInt(), v = ReadInt();
			if(op == 1){
				segt.update(i, v);
			} else{
				segt.update(v, 1);
			}
			printf("%lu\n", segt.get());
		}
	}
	return 0;
}