/*
Steiner森林，策略有二：
1.求出Steiner树后用DP合并解成森林
2.求Steiner树的时候就判断一下，如果两边各自对应好了，就不加边的代价了
这里是用2做的
*/
#include <cstdio>
#include <algorithm>
using namespace std;

typedef unsigned long UL;
const int MAXN = 51;
const int MAXK = 10;
const UL MAXS = (1 << MAXK);
const int INF = 0x3fffffff;
int N, M, K;
int dis[MAXN][MAXN];  //两点距离
int ste[MAXS][MAXN]; //状态，根->Steiner树最小代价
//检查是否配对
bool check(UL st){
	int i;
	int ret = 0;
	for(i = 0; i < K; i ++){
		ret += (st >> i) & 1;
	}
	for(; i < (K << 1); i ++){
		ret -= (st >> i) & 1;
	}
	return (ret == 0);
}
void Floyd(){
	int i, j, k;
	for(k = 1; k <= N; k ++){
		for(i = 1; i <= N; i ++){
			for(j = 1; j <= N; j ++){
				dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
			}
		}
	}
}
void Input(){
	int i, j, x, y, z;
	scanf("%d%d%d", &N, &M, &K);
	for(i = 0; i <= N; i ++){
		for(j = 0; j <= N; j ++){
			dis[i][j] = (i == j) ? 0 : INF;
		}
	}
	for(i = 0; i < M; i ++){
		scanf("%d%d%d", &x, &y, &z);
		dis[x][y] = min(dis[x][y], z);
		dis[y][x] = min(dis[y][x], z);
	}
}
//生成斯坦纳树
int Steiner(){
	const UL maxs = (1 << (K << 1));
	const int base = N - (K << 1);
	UL st, sub;
	int i, j, ans;
	bool paired;
	for(st = 0; st < maxs; st ++){
		for(i = 0; i <= N; i ++){
			ste[st][i] = INF;
		}
	}
	for(i = 0; i < K; i ++)
		ste[1 << i][i + 1] = 0;
	for(; i < (K << 1); i ++)
		ste[1 << i][base + i + 1] = 0;
	for(st = 1; st < maxs; st ++){
		paired = check(st);
		//合并双支
		for(i = 1; i <= N; i ++){
			for(sub = (st - 1) & st; sub > 0; sub = (sub - 1) & st){
				ste[st][i] = min(ste[st][i], ste[sub][i] + ste[st ^ sub][i]);
			}
		}
		//合并边
		for(i = 1; i <= N; i ++){
			for(j = 1; j <= N; j ++){
				if(paired)
					ste[st][i] = min(ste[st][i], ste[st][j]);
				else
					ste[st][i] = min(ste[st][i], ste[st][j] + dis[i][j]);
			}
		}
	}
	ans = INF;
	for(i = 1; i <= N; i ++){
		ans = min(ans, ste[maxs - 1][i]);
	}
	return ans;
}
int main(){
	int t, ans;
	scanf("%d", &t);
	while(t --){
		Input();
		Floyd();
		ans = Steiner();
		if(ans >= INF){
			printf("No solution\n");
		}else{
			printf("%d\n", ans);
		}
	}
	return 0;
}