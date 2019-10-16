//Tag: 暴搜+剪枝
//这是期望写出的算法
#include <cstdio>
#include <queue>
using namespace std;

typedef unsigned long long ull;
ull arr[105];
int N;

ull solve(){
	int i, s;
	ull cur = ~(0ull);
	queue<ull> q;

	q.push(arr[0]);
	q.push(~arr[0]);
	for(i = 1; i < N; i ++){
		s = q.size();
		while(s --){
			cur = q.front();
			if(cur == 0)
				break;
			q.pop();
			q.push(cur ^ arr[i]);
			q.push(cur ^ (~arr[i]));
			q.push(cur & arr[i]);
			q.push(cur & (~arr[i]));
			q.push(cur | arr[i]);
			q.push(cur | (~arr[i]));
		}
		if(cur == 0)
			break;
	}
	if(cur != 0){
		cur = ~(0ull);
		s = q.size();
		while(s --){
			cur = min(cur, q.front());
			q.pop();
		}
	}
	return cur;
}

int main(){
	int i, t = 0;
	while(scanf("%d", &N) != EOF){
		for(i = 0; i < N; i ++)
			scanf("%llu", &arr[i]);
		printf("Case #%d: %llu\n", ++ t, solve());
	}
	return 0;
}