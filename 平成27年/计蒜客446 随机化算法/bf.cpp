#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 105;
const LL INF = 0x3fffffffffffffffLL;

struct NODE{
	int S, T, V;	//星，时间，钱
}node[MAXN];
LL ans;
int N, M, ns;
bool used[MAXN];
int strcol[MAXN];	//每种颜色的随机染色

void DFS(int i, int cnt, int m, LL t){
	if(cnt == 7){
		ans = min(ans, t);
		return ;
	}
	if(i >= N)
		return ;
	if(!used[node[i].S] && m >= node[i].V){
		used[node[i].S] = true;
		DFS(i + 1, cnt + 1, m - node[i].V, t + node[i].T);
		used[node[i].S] = false;
	}
	DFS(i + 1, cnt, m, t);
}

//暴搜大法好
void Solve(){
	int i;
	ans = INF;
	for(i = 0; i < N; i ++){
		strcol[ns ++] = node[i].S;
	}
	sort(strcol, strcol + ns);
	ns = unique(strcol, strcol + ns) - strcol;
	for(i = 0; i < N; i ++){
		node[i].S = lower_bound(strcol, strcol + ns, node[i].S) - strcol;
	}
	if(ns < 7){
		return;
	}
	DFS(0, 0, M, 0);
}

void domain(){
	int i;
	scanf("%d%d", &N, &M);
	for(i = 0; i < N; i ++){
		scanf("%d%d%d", &node[i].S, &node[i].V, &node[i].T);
	}
	Solve();
	if(ans >= INF){
		printf("-1\n");
	}else{
		printf("%lld\n", ans);
	}
}

int main(){

	freopen("dat.in", "r", stdin);
	freopen("dat.out", "w", stdout);

	int T;
	scanf("%d", &T);
	while(T --){
		domain();
	}
	return 0;
}
