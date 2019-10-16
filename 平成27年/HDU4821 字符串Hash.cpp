#include <cstdio>
#include <cstring>
#include <algorithm>
#include <unordered_map>
using namespace std;

const int MAXN = 100005;
typedef long long ll;
const int MODN = 1000000007;
const int MPN  = 31;
const int MPNR = 129032259;
char str[MAXN];
ll sum[MAXN], pw[MAXN];
int N, M, L, ans;
unordered_map<int, int> cnt;

void pre(){
	int i;
	pw[0] = 1;
	for(i = 1; i < MAXN; i ++)
		pw[i] = pw[i - 1] * MPN % MODN;
}

inline ll get(int l, int len){
	ll ret = sum[l + len - 1] - sum[l - 1] * pw[len] % MODN;
	return ret < 0 ? ret + MODN : ret;
}

void deal(int off){
	int i, l, r, kind, cur;

	cnt.clear();
	kind = 0;
	for(i = 1, r = off; i < M && r + L <= N + 1; i ++, r += L){
		cur = (int)get(r, L);
		cnt[cur] ++;
		if(cnt[cur] == 1){
			kind ++;
		}
	}
	for(l = off; r + L <= N + 1; l += L, r += L){
		//增加r
		cur = (int)get(r, L);
		cnt[cur] ++;
		if(cnt[cur] == 1){
			kind ++;
		}
		//计算答案
		if(kind == M)
			ans ++;
		//减少l
		cur = (int)get(l, L);
		cnt[cur] --;
		if(cnt[cur] == 0){
			kind --;
		}
	}
}

bool domain(){
	int i;

	if(scanf("%d%d%s", &M, &L, str + 1) == EOF)
		return false;
	N = strlen(str + 1);
	ans = 0;

	for(i = 1; i <= N; i ++){
		sum[i] = (sum[i - 1] * MPN + str[i] - 'a') % MODN;
	}
	for(i = 1; i <= L; i ++)
		deal(i);

	printf("%d\n", ans);

	return true;
}

int main(){
	pre();
	while(domain());
	return 0;
}