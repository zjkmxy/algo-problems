#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 100005;
int N, M;
ll sum;
vector<int> grp[10];
bool used[MAXN];

bool fin(int i, int bas, int &ptr){
	int j, cnt = 0;

	while(true){
		while(bas > 0){
			//选大的
			if(!used[bas]){
				grp[i].push_back(bas);
				used[bas] = true;
				return true;
			}

			//选小的
			for(j = 1; used[j]; j ++);
			if(bas < j)
				break;
			grp[i].push_back(j);
			used[j] = true;
			bas -= j;
			cnt ++;
		}
		while(cnt --){
			bas += grp[i].back();
			used[grp[i].back()] = false;
			grp[i].pop_back();
		}
		//额外退一个，增加指针
		bas += grp[i].back();
		ptr = max(ptr, grp[i].back());
		used[grp[i].back()] = false;
		grp[i].pop_back();
		cnt = 0;
	}
	return true;
}

void domain(){
	int i, bas, j;
	bool flag = true;

	scanf("%d%d", &N, &M);
	fill(used, used + N + 1, 0);
	for(i = 0; i < M; i ++)
		grp[i].clear();
	sum = ll(1 + N) * ll(N) >> 1;
	if(sum % M != 0 || sum / M < N){
		puts("NO");
		return ;
	}
	for(i = 0, j = N; i < M && flag; i ++){
		bas = sum / M;
		while(bas > 0){
			while(used[j] && j >= 1)
				j --;
			if(bas > j){
				used[j] = true;
				grp[i].push_back(j);
				bas -= j;
			}else{
				flag = flag && fin(i, bas, j);
				bas = 0;
			}
		}
	}
	if(flag){
		puts("YES");
		for(i = 0; i < M; i ++){
			printf("%d", grp[i].size());
			for(j = grp[i].size() - 1; j >= 0; j --)
				printf(" %d", grp[i][j]);
			printf("\n");
		}
	}else{
		puts("NO");
	}
}

int main(){
	int T;
	scanf("%d", &T);
	while(T --){
		domain();
	}
	return 0;
}