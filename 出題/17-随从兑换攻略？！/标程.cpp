#include<cstdio>
#include<cctype>
#include<queue>
using namespace std;

#define MAXN	18
#define MAXC	145
#define MAXS	262144
typedef pair<int, int> STATE;
struct MINION{
	char name[MAXC];
	int a, h;
	bool ds;
}mins[MAXN];
int pre[MAXN][MAXS];	//前一步的位置
int dp[MAXN][MAXS];		//最优的结果
int rate[MAXN][MAXN];	//兑换比率（限正向）

int bitcnt(unsigned int x)
{
	int j, t;
	unsigned int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};
	for(j = 0, t = 1; j < 5; j ++, t <<= 1)
		x = (x & b[j]) + ((x >> t) & b[j]);
	return x;
}

int CalcRate(int A, int B)
{
	int ret = 0, h1 = mins[A].h, h2 = mins[B].h;
	bool ds1 = mins[A].ds, ds2 = mins[B].ds;
	while(h1 > 0){
		if(ds1)
			ds1 = false;
		else
			h1 -= mins[B].a;
		if(ds2)
			ds2 = false;
		else
			h2 -= mins[A].a;
		if(h2 <= 0){
			ret++;
			h2 = mins[B].h;
		}
	}
	return ret;
}

queue<STATE> oplst;
STATE arr[MAXN];
int main()
{
	int i, j, N, maxv = 0, val, tmp, st;
	STATE cur, ans;
	char ch[3];

	scanf("%d",&N);
	for(i=0;i<N;i++){
		scanf("%s %d %d %s", mins[i].name, &mins[i].a, &mins[i].h, ch);
		mins[i].ds = (ch[0] == 'D');
	}
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			rate[i][j] = CalcRate(i, j);
	memset(dp, 0, sizeof(dp));
	memset(pre, -1, sizeof(pre));
	for(i=0;i<N;i++){
		dp[i][1<<i] = 1;
		oplst.push(make_pair(i, 1<<i));
	}
	while(!oplst.empty()){
		cur = oplst.front();
		oplst.pop();
		val = dp[cur.first][cur.second];
		if(val > maxv || (val == maxv && bitcnt(cur.second) > bitcnt(ans.second))){
			maxv = val;
			ans = cur;
		}
		for(j=0;j<N;j++){
			st = cur.second | (1 << j);
			if(st == cur.second || rate[cur.first][j] <= 0)
				continue;
			tmp = val * rate[cur.first][j];
			if(tmp <= dp[j][st])
				continue;
			dp[j][st] = tmp;
			pre[j][st] = cur.first;
			oplst.push(make_pair(j, st));
		}
	}
	for(j=0;ans.first>=0;j++){
		arr[j] = ans;
		ans = make_pair(pre[ans.first][ans.second], ans.second & ~(1 << ans.first));
	}
	for(j--; j >= 0; j--){
		printf("%d * %s", dp[arr[j].first][arr[j].second], mins[arr[j].first].name);
		if(j > 0)
			printf(" =");
		printf("\n");
	}
	return 0;
}