#include<cstdio>
#include<cstring>
#include<cmath>
#include<cctype>
#include<algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 1000005;
const ll MODN = 1000000007;
char str[MAXN];
int nxt[MAXN], num[MAXN], cnt[MAXN], nxt2[MAXN];
int len;

void getnxt(){
	int i, k, t;

	i = 0;
	k = t = -1;
	nxt[0] = -1;
	cnt[0] = -1;
	while(i < len){
		while(k >= 0 && str[i] != str[k])
			k = nxt[k];
		while(t >= 0 && (str[i] != str[t] || (t << 1) >= i))
			t = nxt[t];
		++ i;
		nxt[i] = ++ k;
		nxt2[i] = ++ t;
		cnt[i] = cnt[k] + 1;
		num[i] = cnt[t] + 1;
	}
}

void domain(){
	int i;
	ll ans = 1;

	gets(str);
	len = strlen(str);
	getnxt();
	for(i = 1; i <= len; i ++)
		ans = ans * (num[i] + 1) % MODN;
	printf("%lld\n", ans);
}

int main(){
	int T;
	scanf("%d\n", &T);
	while(T --){
		domain();
	}
	return 0;
}
