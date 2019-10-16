#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
const int INF = 0x3fffffff;
struct A{
	int D;
	bool used;
}arr[MAXN];
int N, M;
int S[MAXN];

//STL的vector太慢
struct vector{
	int a[MAXN], size;
	inline void clear(){
		size = 0;
	}
	inline int& operator[](int index){
		return a[index];
	}
	inline int* begin(){
		return a;
	}
	inline int* end(){
		return a + size;
	}
	inline void push_back(int x){
		a[size ++] = x;
	}
};

//cnt[l][r]：在已经选择了A[l]和A[r]的基础上，还能选择多少个l..r中间的元素构成一个链，使得选中的所有元素不在S中的个数最多
//last[l][r]：要达到cnt[l][r]，所选择的除l之外最左边的一个元素
int cnt[MAXN][MAXN], last[MAXN][MAXN];
//gogo[i]：假设已经取了i，要取成一个极长链，下一个可能选取的元素。注意必须是单调递减的。
vector gogo[MAXN];
//计算gogo、cnt和last
void build(){
	int len, l, r, cur, k, it;
	//k是最后一个被选择的点

	for(l = 0; l <= N + 1; l ++){
		cur = INF;
		gogo[l].clear();
		for(r = l + 1; r <= N + 1; r ++){
			if(arr[r].D < arr[l].D || arr[r].D >= cur)
				continue;
			cur = arr[r].D;
			gogo[l].push_back(r);
		}
	}

	for(l = 0; l <= N + 1; l ++){
		cnt[l][l] = arr[l].used ? 0 : 1;
		last[l][l] = -1;
	}
	for(len = 2; len <= N + 2; len ++){
		for(l = 0; l <= N - len + 2; l ++){
			r = l + len - 1;
			cnt[l][r] = 0;
			last[l][r] = -1;
			if(arr[l].D > arr[r].D)
				continue;
			for(it = 0; it < gogo[l].size;++ it){
				k = gogo[l][it];
				if(k > r)
					break;
				if(arr[k].D < arr[l].D || arr[k].D > arr[r].D)
					continue;
				cur = cnt[k][r];
				if(!arr[l].used)
					cur ++;	//算上左端点
				if(cur > cnt[l][r]){
					cnt[l][r] = cur;
					last[l][r] = k;
				}
			}
		}
	}
}

//dp[x]：如果是arr[0..S[x]]的话答案最大是多少
//sel[x]：要达到dp[x]，我们要把S均分在A、B中。S[sel[x]]..S[x]同在A（或B）中，而S[sel[x]-1]不同。
int dp[MAXN], sel[MAXN];
//ans：最终答案的A和B
vector ans[2];
int calc(){
	int i, j, cur, l = M + 1;

	build();
	dp[0] = 0;
	sel[0] = 0;
	for(i = 1; i <= l; i ++){
		dp[i] = cnt[0][S[i]];
		sel[i] = 0;
		for(j = 1; j < i; j ++){
			cur = dp[j] + cnt[S[j - 1]][S[i]];
			if(cur > dp[i]){
				dp[i] = cur;
				sel[i] = j;
			}
		}
	}

	int seq = 0, k;
	ans[0].clear();
	ans[1].clear();
	for(i = l; i > 0; i = j){
		j = sel[i];
		for(k = j; k < i; k ++){
			if(S[k] == 0 || S[k] == N + 1)
				continue;
			ans[seq].push_back(S[k]);
		}
		seq ^= 1;
		for(k = j ? S[j - 1] : 0; k >= 0; k = last[k][S[i]]){
			if(arr[k].used)
				continue;
			ans[seq].push_back(k);
		}
	}
	sort(ans[0].begin(), ans[0].end());
	sort(ans[1].begin(), ans[1].end());

	return M + dp[l];
}

bool domain(){
	int i, sz;

	if(scanf("%d%d", &N, &M) == EOF)
		return false;
	for(i = 1; i <= N; i ++){
		scanf("%d", &arr[i].D);
		arr[i].used = false;
	}
	arr[0].D = -1;
	arr[N + 1].D = INF - 1;
	arr[0].used = arr[N + 1].used = true;
	for(i = 1; i <= M; i ++){
		scanf("%d", &S[i]);
		arr[S[i]].used = true;
	}
	S[0] = 0;
	S[M + 1] = N + 1;

	sz = calc();
	if(sz != ans[0].size + ans[1].size)
		throw 0;
	printf("%d %d\n", ans[0].size, ans[1].size);
	for(i = 0; i < ans[0].size; i ++){
		if(i)
			putchar(' ');
		printf("%d", ans[0][i]);
	}
	printf("\n");
	for(i = 0; i < ans[1].size; i ++){
		if(i)
			putchar(' ');
		printf("%d", ans[1][i]);
	}
	printf("\n");
	return true;
}

int main(){
	while(domain());
	return 0;
}