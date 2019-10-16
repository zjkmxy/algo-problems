#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef unsigned long UL;
const int MAXK = 10;
const int MAXN = MAXK * MAXK;
const UL MAXS = (1 << MAXK);
const int INF = 0x3fffffff;
int N, M, K;
int dis[MAXN][MAXN];  //两点距离
int ste[MAXS][MAXN]; //状态，根->Steiner树最小代价

//生成斯坦纳树
int Steiner(){
	const UL maxs = (1 << K);
	UL st, sub;
	int i, j, ans;
	for(st = 0; st < maxs; st ++){
		for(i = 0; i < N; i ++){
			ste[st][i] = INF;
		}
	}
	for(i = 0; i < K; i ++)
		ste[1 << i][i] = 0;
	for(st = 1; st < maxs; st ++){
		//合并双支
		for(i = 0; i < N; i ++){
			for(sub = (st - 1) & st; sub > 0; sub = (sub - 1) & st){
				ste[st][i] = min(ste[st][i], ste[sub][i] + ste[st ^ sub][i]);
			}
		}
		//合并边
		for(i = 0; i < N; i ++){
			for(j = 0; j < N; j ++){
				ste[st][i] = min(ste[st][i], ste[st][j] + dis[i][j]);
			}
		}
	}
	ans = INF;
	for(i = 0; i < N; i ++){
		ans = min(ans, ste[maxs - 1][i]);
	}
	return ans;
}

bool Input(){
	bool cross[MAXK][MAXK];
	int x[MAXK], y[MAXK], xn, yn;
	pair<int, int> pnt[MAXN];

	if(scanf("%d", &K) == EOF)
		return false;
	x[0] = y[0] = 0;
	pnt[0] = make_pair(0, 0);
	for(N = 1; N <= K; N ++){
		scanf("%d%d", &x[N], &y[N]);
		pnt[N] = make_pair(x[N], y[N]);
	}
	sort(x, x + N);
	xn = unique(x, x + N) - x;
	sort(y, y + N);
	yn = unique(y, y + N) - y;
	memset(cross, 0, sizeof(cross));
	for(int i = 0; i <= K; i ++){
		int xp = lower_bound(x, x + xn, pnt[i].first) - x, yp = lower_bound(y, y + yn, pnt[i].second) - y;
		pnt[i] = make_pair(xp, yp);
		cross[xp][yp] = true;
	}
	for(int i = 0; i < xn; i ++){
		for(int j = 0; j < yn; j ++){
			if(cross[i][j])
				continue;
			pnt[N ++] = make_pair(i, j);
		}
	}
	for(int i = 0; i < N; i ++){
		for(int j = 0; j < N; j ++){
			dis[i][j] = abs(x[pnt[i].first] - x[pnt[j].first]) + abs(y[pnt[i].second] - y[pnt[j].second]);
		}
	}
	K ++;
	return true;
}

int main(){
	while(Input()){
		printf("%d\n", Steiner());
	}
	return 0;
}