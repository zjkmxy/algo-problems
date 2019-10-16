//每攒够2K个元素，取前K个，删去后K个。最后排序，O(N)的。
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXK = 400005;
typedef long long ll;
int xp, x0, a, b, c, N, K;
int buf[MAXK], top;

int gen(){
	ll x = (ll(xp) * ll(a)) & 0x7fffffff;
	x += (ll(x0) * ll(b)) & 0x7fffffff;
	x += c;
	x = x & 0x7fffffff;
	xp = x0;
	x0 = x;
	return x;
}

int main(){
	int i;
	scanf("%d%d%d%d%d%d%d", &N, &K, &xp, &x0, &a, &b, &c);
	for(i = 1; i <= N; i ++){
		buf[top ++] = gen();
		if(top >= (K << 1) || i == N){
			nth_element(buf, buf + K, buf + top);
			copy(buf + K, buf + top, buf);
			top = K;
		}
	}
	sort(buf, buf + K);
	for(i = K - 1; i >= 0; i --){
		printf(i > 0 ? "%d " : "%d\n", buf[i]);
	}
	return 0;
}