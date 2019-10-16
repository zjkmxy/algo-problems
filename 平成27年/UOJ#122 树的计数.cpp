#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 200005;
int dfs[MAXN], bfs[MAXN], rk[MAXN], mx[MAXN], N;
double ans, cur;

void Renormal(){
	int i;
	for(i = 1; i <= N; i ++)
		rk[bfs[i]] = i;
	for(i = 1; i <= N; i ++)
		dfs[i] = rk[dfs[i]];
	for(i = 1; i <= N; i ++){
		bfs[i] = i;
		rk[dfs[i]] = i;
		mx[i] = max(mx[i - 1], dfs[i]);
	}
}

void Solve(){
	int i;

	cur = 0.;
	ans = 1.;
	//Ĭ��1->2����3��ʼ����
	//i + 1�ǵ�ǰ�ĵ�
	if(N > 1)
		ans += 1.;
	for(i = 2; i < N; i ++){
		if(rk[i + 1] < rk[i]){
			//i + 1��i����һ��
			ans += 1. + cur;
			cur = 0;
		}else if(rk[i + 1] == rk[i] + 1){
			//���ɵĵ�
			if(mx[rk[i]] <= i)
				cur += 0.5;
		}else{
			//�м��������ĵ�
			if(dfs[rk[i + 1] - 1] < i)
				cur = 0.;
		}
	}
	ans += cur;
}

int main(){
	int i;
	while(scanf("%d", &N) != EOF){
		for(i = 1; i <= N; i ++)
			scanf("%d", &dfs[i]);
		for(i = 1; i <= N; i ++)
			scanf("%d", &bfs[i]);
		Renormal();
		Solve();
		printf("%.3lf\n", ans);
	}
	return 0;
}