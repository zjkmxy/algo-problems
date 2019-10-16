#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 501;
const int MAXP = 100;
const int MAXS = 256;

bool isnprime[MAXN];
int pl[MAXP], npl;	//大质数
int ps[10], nps;	//小质数
vector<int> grp[MAXP];	//每组的数
int msk[MAXN];	//所有数关于小质数的掩码
int N, P;

void Prepare(){
	int i, j, lim = floor(sqrt(N));

	//预处理大小素数
	npl = nps = 0;
	pl[npl ++] = 1;
	for(i = 2; i <= N; i ++){
		if(isnprime[i])
			continue;
		if(i <= lim)
			ps[nps ++] = i;
		else
			pl[npl ++] = i;
		for(j = i + i; j <= N; j += i)
			isnprime[j] = true;
	}
	//预处理数
	for(i = 2; i <= N; i ++){
		//分组
		for(j = npl - 1; j > 0 && i % pl[j] > 0; j --);
		grp[j].push_back(i);
		//计算掩码
		msk[i] = 0;
		for(j = 0; j < nps; j ++){
			if(i % ps[j] == 0)
				msk[i] |= (1 << j);
		}
	}
};

inline void update(LL& ori, const LL val){
	ori = (ori + val) % P;
}

LL Solve(){
	static LL dp[2][MAXS][MAXS];	//AB取得小质数状况下的方案数
	static LL cur[MAXS][MAXS][2];	//当前组下，A或B持有组的方案数
	const int ms = (1 << nps);	//状态总数
	int rd = 0, wt = 1;
	int i, sta, stb;
	LL ret = 0;

	memset(dp, 0, sizeof(dp));
	dp[rd][0][0] = 1;
	//处理组外值
	for(auto v : grp[0]){
		for(sta = ms - 1; sta >= 0; sta --){
			for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){ //这个是快速枚举子集的
				if((stb & msk[v]) == 0)
					update(dp[rd][sta | msk[v]][stb], dp[rd][sta][stb]);
				if((sta & msk[v]) == 0)
					update(dp[rd][sta][stb | msk[v]], dp[rd][sta][stb]);
			}
		}
	}
	//枚举组
	for(i = 1; i < npl; i ++, rd ^= 1, wt ^= 1){
		memset(cur, 0, sizeof(cur));
		memset(dp[wt], 0, sizeof(dp[wt]));

		//枚举组内的数
		for(auto v : grp[i]){
			//考虑到相互影响，状态需要逆序枚举
			for(sta = ms - 1; sta >= 0; sta --){
				for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){
					//处理A的情况
					if((stb & msk[v]) == 0){
						//之前A有组，本次可以拿也可以不拿
						update(cur[sta | msk[v]][stb][0], cur[sta][stb][0]);
						//之前A无组，拿了可以进组
						update(cur[sta | msk[v]][stb][0], dp[rd][sta][stb]);
					}
					//处理B的情况
					if((sta & msk[v]) == 0){
						update(cur[sta][stb | msk[v]][1], cur[sta][stb][1]);
						update(cur[sta][stb | msk[v]][1], dp[rd][sta][stb]);
					}
				}
			}
		}
		//利用组内的数更新dp值
		for(sta = 0; sta < ms; sta ++){
			for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){
				update(dp[wt][sta][stb], dp[rd][sta][stb]);	//双方都无组
				update(dp[wt][sta][stb], cur[sta][stb][0]);	//A有组
				update(dp[wt][sta][stb], cur[sta][stb][1]);	//B有组
			}
		}
	}
	//统计答案
	ret = 0;
	for(sta = 0; sta < ms; sta ++){
		for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){
			update(ret, dp[rd][sta][stb]);
		}
	}
	return ret;
}

int main(){
	scanf("%d%d", &N, &P);
	Prepare();
	printf("%lld\n", Solve());
	return 0;
}