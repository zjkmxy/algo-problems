/*
算法教室⑨C
给定任意张写有整数的卡牌，每次从中选出任意张，要求他们的和模9为0，求方案数。另有每次修改一张牌的数值，每改一次求一次方案数。
1.如果不算重复：(2,2,3,4,5) => (2,2,5),(2,3,4),(4,5)共3种
2.如果算重复：上例中(2,3,4)有2个所以一共4种
各自应该怎么做？

方法一：DP  9^3 * N  作者PQ
统计模9余数为i的数字个数到cnt[i]里
令i从0到9计算，dp[i][j]表示处理到i为止，得出和模9余数为j的方案数
每个dp[i][j]按cnt[i]的个数递推计算
因为随着计入数字i的卡片的个数递增，余数的变化是有循环节的
所以利用循环节优化处理
这是目标1，如果是目标2，需要用组合数进行打表计算
就是打C[x,a]+C[x,a+d]+C[x,a+2d]+...+C[x,a+?d]
详见PQ代码

方法二：直接计数  9^2 * N  作者MXY
对目标2：
直接令cnt[i]表示当前余数是i的方法数有多少种
每加入一个数j的时候，令所有的cnt[i+j] = cnt[i] + cnt[j]即可
关键是删去一个数的时候，因为⑨是奇数，所以上面的过程是可以求出逆矩阵的
反向操作即可
对目标1：
并没有好的思路

方法三：万能的CDQ分治   9 * N * log N   目前没人写
显然两个目标都可以达成，而且不需要9是奇数

方法四：打表处理    9^3 * N    作者Green
对0..8的每个数字，在只有这一种数字且个数为0..10^5个的时候各自打一张cnt的表
然后每次查询利用预先记录好的每种数字的个数，对所有的表进行组合处理
两个目标都可以达成，不需要9是奇数。

*/

#ifdef PROC_1_1
#include <cstdio>
#include <vector>
using namespace std;
const int maxn = 100005;
const int MOD = (int)1e9 + 9;
int card[maxn], cnt[10], period[10], dp[10][10];
vector <int> V[10];
bool vis[10];
int Solve() {
	memset(dp, 0, sizeof(dp));
	dp[0][0] = cnt[0] + 1;
	for(int i = 1; i <= 9; i++) {
		for(int j = 0; j < V[i].size() && j <= cnt[i]; j++) {
			int u = V[i][j];
			for(int k = 0; k < 9; k++) {
				int &x = dp[i][(k + u) % 9];
				x += (long long)dp[i - 1][k] * ((cnt[i] - j) / period[i] + 1) % MOD;
				if(x >= MOD) x -= MOD;
			}
		}
	}
	return (dp[9][0] - 1 + MOD) % MOD;
}
void init() {
	for(int i = 1; i <= 9; i++) {
		for(int j = 1; j <= 9; j++) {
			if(i * j % 9 == 0) {
				period[i] = j;
				break;
			}
		}
	}
	for(int i = 1; i <= 9; i++) {
		memset(vis, 0, sizeof(vis));
		V[i].push_back(0);
		vis[0] = 1;
		for(int j = 1; j <= 9; j++) {
			int x = i * j % 9;
			if(vis[x]) {
				break;
			} else {
				V[i].push_back(x);
				vis[x] = 1;
			}
		}
	}
}
int main() {
	init();
	int n, x, y, Q;
	scanf("%d%d", &n, &Q);
	for(int i = 1; i <= n; i++) {
		scanf("%d", &card[i]);
		cnt[card[i]]++;
	}
	printf("%d\n", Solve());
	while(Q--) {
		scanf("%d%d", &x, &y);
		cnt[card[x]]--;
		card[x] = y;
		cnt[card[x]]++;
		printf("%d\n", Solve());
	}
	return 0;
}

#endif

#ifdef PROC_1_2
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;
const int maxn = 100005;
const int MOD = (int)1e9 + 9;
int card[maxn], fac[maxn], inv[maxn], cnt[10], period[10], dp[10][10];
vector <int> V[10];
bool vis[10];
int trans(int x) {
	if(x == 1) return 0;
	if(x == 3) return 1;
	if(x == 9) return 2;
	throw 0;
}

int Darr[maxn][9][3];
void PrepareDarr(){
	int x, y, w;
	static const int U[] = {1, 3, 9};
	for(y = 0; y < 9; y ++)
		for(w = 0; w < 3; w ++)
			Darr[0][y][w] = (y == 0) ? 1 : 0;
	for(x = 1; x < maxn; x ++){
		for(w = 0; w < 3; w ++){
			Darr[x][0][w] = Darr[x - 1][0][w] + Darr[x - 1][U[w] - 1][w];
			Darr[x][0][w] %= MOD;
			for(y = 1; y < 9; y ++){
				Darr[x][y][w] = Darr[x - 1][y][w] + Darr[x - 1][y - 1][w];
				Darr[x][y][w] %= MOD;
			}
		}
	}
}

int D(int x, int y, int z, int w) { 
	long long ans = 0;
	int u[3] = {1, 3, 9};
	for(int i = 0; i <= z; i++) {
		ans += (long long)fac[x] * inv[y + i * u[w]] % MOD * inv[x - y - i * u[w]] % MOD;
		if(ans >= MOD) ans -= MOD;
	}
	return ans;
}
int Solve() {
	memset(dp, 0, sizeof(dp));
	dp[0][0] = Darr[cnt[0]][0][0];
	for(int i = 1; i <= 9; i++) {
		for(int j = 0; j < V[i].size() && j <= cnt[i]; j++) {
			int u = V[i][j];
			int v = Darr[cnt[i]][j][trans(period[i])];
			for(int k = 0; k < 9; k++) {
				int &x = dp[i][(k + u) % 9];
				x += (long long)dp[i - 1][k] * v % MOD;
				if(x >= MOD) x -= MOD;
			}
		}
	}
	return (dp[9][0] - 1 + MOD) % MOD;
}
int modExp(int a, int b) {
	long long t = 1LL, y = a;
	while(b) {
		if(b & 1) t = t * y % MOD;
		y = y * y % MOD;
		b >>= 1;
	}
	return (int)t;
}
void init() {
	fac[0] = 1;
	for(int i = 1; i < maxn; i++) fac[i] = (long long)fac[i - 1] * i % MOD;
	for(int i = 0; i < maxn; i++) inv[i] = modExp(fac[i], MOD - 2);

	for(int i = 1; i <= 9; i++) {
		for(int j = 1; j <= 9; j++) {
			if(i * j % 9 == 0) {
				period[i] = j;
				break;
			}
		}
	}
	for(int i = 1; i <= 9; i++) {
		memset(vis, 0, sizeof(vis));
		V[i].push_back(0);
		vis[0] = 1;
		for(int j = 1; j <= 9; j++) {
			int x = i * j % 9;
			if(vis[x]) {
				break;
			} else {
				V[i].push_back(x);
				vis[x] = 1;
			}
		}
	}
}
int main() {
	PrepareDarr();
	init();

	int n, x, y, Q;
	scanf("%d%d", &n, &Q);
	for(int i = 1; i <= n; i++) {
		scanf("%d", &card[i]);
		cnt[card[i]]++;
	}
	printf("%d\n", Solve());
	while(Q--) {
		scanf("%d%d", &x, &y);
		cnt[card[x]]--;
		card[x] = y;
		cnt[card[x]]++;
		printf("%d\n", Solve());
	}
	return 0;
}
#endif

#ifdef PROC_2
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-result"
#include <cstdio>  
#include <cstdlib>
#include <cmath>
#include <algorithm>  
using namespace std;  

//模大整数余数类
struct ul{
	static const unsigned long MODN = 1000000009;
	static const unsigned long DIV2 = 500000005;
	unsigned long n;
	ul(unsigned long n = 0):n(n){};
	inline friend ul operator+(const ul& a, const ul& b){
		unsigned long r = a.n + b.n;
		return (r < a.n || r >= MODN) ? ul(r - MODN) : ul(r);
	}
	inline friend ul operator-(const ul& a, const ul& b){
		return (a.n >= b.n) ? ul(a.n - b.n) : ul(a.n + MODN - b.n);
	}
	inline friend ul operator*(const ul& a, const ul& b){
		unsigned long long r = (unsigned long long)(a.n) * (unsigned long long)(b.n);
		return ul(r % MODN);
	}
	inline ul half()const{
		return (*this) * DIV2;
	}
};

const int MAXN = 100005;
ul cnt[9];				//选取某些牌，使得余数为x的方法数
static ul tab[9][9][9];	//逆运算的矩阵的表
int arr[MAXN];			//每一张的卡牌
int N, M;

//预先打好逆运算的表
void inittab(){
	int i, j, p, ed;
	for(i = 1; i < 9; i ++){
		for(j = 0; j < 9; j ++){
			ed = (j + i) % 9;
			for(p = ed; p != j; p = (p + i + i) % 9)
				tab[i][j][(p + i) % 9] = 2;
			if(i == 3 || i == 6){
				for(p = 0; p < 9; p ++){
					if((p - j + 3) % 3 != 0){
						tab[i][j][p] = 1;
					}
				}
			}
		}
	}
}

//添加某一张牌
void add(int val){
	int i, j;
	ul tmp[9];
	for(i = 0, j = val; i < 9; i ++, j = (j + 1) % 9)
		tmp[j] = cnt[i] + cnt[j];
	tmp[val] = tmp[val] + ul(1);
	copy(tmp, tmp + 9, cnt);
}

//减少某一张牌
void substract(int val){
	int i, j;
	cnt[val] = cnt[val] - 1;
	if(val == 0){
		for(i = 0; i < 9; i ++)
			cnt[i] = cnt[i].half();
	}else{
		ul tmp[9], sum = 0;
		for(i = 0; i < 9; i ++)
			sum = sum + cnt[i];
		fill(tmp, tmp + 9, sum);
		for(i = 0; i < 9; i ++){
			for(j = 0; j < 9; j ++)
				tmp[i] = tmp[i] - tab[val][i][j] * cnt[j];
			tmp[i] = tmp[i].half();
		}
		copy(tmp, tmp + 9, cnt);
	}
}

int main(){
	int i, a, b;
	inittab();
	scanf("%d%d", &N, &M);
	for(i = 1; i <= N; i ++){
		scanf("%u", &arr[i]);
		arr[i] %= 9;
		add(arr[i]);
	}
	printf("%lu\n", cnt[0].n);
	while(M --){
		scanf("%d%d", &a, &b);
		b %= 9;
		if(arr[a] != b){
			substract(arr[a]);
			arr[a] = b;
			add(arr[a]);
		}
		printf("%lu\n", cnt[0].n);
	}
	return 0;
}
#endif

#ifdef PROC_4
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;
const int mod = 1000000009;
#define N 100005
#define LL long long
int dp[N][9][9],save[N];

int ar[N],num[10];

void init(){
     memset(dp,0,sizeof(dp));
     memset(num,0,sizeof(num));
     for(int i = 0;i <= 9;i++){         //当没有卡片时有1种情况
        dp[0][i][0] = 1;
     }
     save[1] = 2;
     for(int i = 2;i < N;i++){  //预处理快速幂
        save[i] = ((LL)save[i-1] * (LL)save[i-1])%mod;
     }
     //预处理每种卡片的数量余数有多少种情况
     for(int i = 1;i <= 9; i++){
        for(int j = 1;j < N; j++){
           for(int k = 0;k < 9; k++){
                int l = (i + k) % 9;
                dp[j][i][k] += dp[j-1][i][k];
                dp[j][i][k] %= mod;
                dp[j][i][l] += dp[j-1][i][k];
                dp[j][i][l] %= mod;
           }
        }
     }
     for(int i = 1;i <= 9; i++){
        for(int j = 0;j < N;j++){
            dp[j][i][0] --;     //减去没有卡片的情况
        }
     }
}

int power(int a){//快速幂
    int ret = 1,cnt = 1;
    while(a){
        if(a&1){
            ret = ((LL)ret * (LL)save[cnt])%mod;
        }
        a >>= 1;
        cnt++;
    }
    return ret;
}

int deal(){
    int ans[10],tmp[10];
    for(int i = 0;i < 9;i++){
        ans[i] = dp[num[1]][1][i];  //把1卡片的情况赋值给ans
    }
    for(int i = 2;i <= 9;i++){      //计算2~9卡片的情况
        for(int k = 0;k < 9;k++) {
            tmp[k] = ans[k];
            ans[k] += dp[num[i]][i][k];
            ans[k] %= mod;
        }
        for(int j = 0;j < 9;j++){   //枚举ans的余数
            for(int k = 0;k < 9;k++){//枚举i卡片的余数
                int l = (j + k) % 9;
                ans[l] =  ((LL)ans[l] + (LL)tmp[j]*(LL)dp[num[i]][i][k])%mod;
            }
        }
    }
    int ret;
    ret = (((LL)ans[0] + 1) * (LL) power(num[0]) - 1) % mod;  //对于0卡片还要加上很多情况  (ret + 1) * 2 ^ n - 1
    return ret;
}


int main(){
    init();
    //freopen("c:/4.in","r",stdin);
    //freopen("c:/3.out","w",stdout);
    int n,q,a,b;
    scanf("%d%d",&n,&q);
    for(int i = 1;i <= n;i++){
        scanf("%d",&ar[i]);
        ar[i] %= 9;
        num[ar[i]]++;
    }
    printf("%d\n",deal());
    for(int i = 0;i < q;i++){
        scanf("%d%d",&a,&b);
        num[ar[a]]--;
        num[b]++;
        ar[a] = b;
        printf("%d\n",deal());
    }
    return 0;
}
#endif