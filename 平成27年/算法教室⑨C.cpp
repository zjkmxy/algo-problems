/*
�㷨���Ң�C
����������д�������Ŀ��ƣ�ÿ�δ���ѡ�������ţ�Ҫ�����ǵĺ�ģ9Ϊ0���󷽰���������ÿ���޸�һ���Ƶ���ֵ��ÿ��һ����һ�η�������
1.��������ظ���(2,2,3,4,5) => (2,2,5),(2,3,4),(4,5)��3��
2.������ظ���������(2,3,4)��2������һ��4��
����Ӧ����ô����

����һ��DP  9^3 * N  ����PQ
ͳ��ģ9����Ϊi�����ָ�����cnt[i]��
��i��0��9���㣬dp[i][j]��ʾ����iΪֹ���ó���ģ9����Ϊj�ķ�����
ÿ��dp[i][j]��cnt[i]�ĸ������Ƽ���
��Ϊ���ż�������i�Ŀ�Ƭ�ĸ��������������ı仯����ѭ���ڵ�
��������ѭ�����Ż�����
����Ŀ��1�������Ŀ��2����Ҫ����������д�����
���Ǵ�C[x,a]+C[x,a+d]+C[x,a+2d]+...+C[x,a+?d]
���PQ����

��������ֱ�Ӽ���  9^2 * N  ����MXY
��Ŀ��2��
ֱ����cnt[i]��ʾ��ǰ������i�ķ������ж�����
ÿ����һ����j��ʱ�������е�cnt[i+j] = cnt[i] + cnt[j]����
�ؼ���ɾȥһ������ʱ����Ϊ������������������Ĺ����ǿ������������
�����������
��Ŀ��1��
��û�кõ�˼·

�����������ܵ�CDQ����   9 * N * log N   Ŀǰû��д
��Ȼ����Ŀ�궼���Դ�ɣ����Ҳ���Ҫ9������

�����ģ������    9^3 * N    ����Green
��0..8��ÿ�����֣���ֻ����һ�������Ҹ���Ϊ0..10^5����ʱ����Դ�һ��cnt�ı�
Ȼ��ÿ�β�ѯ����Ԥ�ȼ�¼�õ�ÿ�����ֵĸ����������еı������ϴ���
����Ŀ�궼���Դ�ɣ�����Ҫ9��������

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

//ģ������������
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
ul cnt[9];				//ѡȡĳЩ�ƣ�ʹ������Ϊx�ķ�����
static ul tab[9][9][9];	//������ľ���ı�
int arr[MAXN];			//ÿһ�ŵĿ���
int N, M;

//Ԥ�ȴ��������ı�
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

//���ĳһ����
void add(int val){
	int i, j;
	ul tmp[9];
	for(i = 0, j = val; i < 9; i ++, j = (j + 1) % 9)
		tmp[j] = cnt[i] + cnt[j];
	tmp[val] = tmp[val] + ul(1);
	copy(tmp, tmp + 9, cnt);
}

//����ĳһ����
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
     for(int i = 0;i <= 9;i++){         //��û�п�Ƭʱ��1�����
        dp[0][i][0] = 1;
     }
     save[1] = 2;
     for(int i = 2;i < N;i++){  //Ԥ���������
        save[i] = ((LL)save[i-1] * (LL)save[i-1])%mod;
     }
     //Ԥ����ÿ�ֿ�Ƭ�����������ж��������
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
            dp[j][i][0] --;     //��ȥû�п�Ƭ�����
        }
     }
}

int power(int a){//������
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
        ans[i] = dp[num[1]][1][i];  //��1��Ƭ�������ֵ��ans
    }
    for(int i = 2;i <= 9;i++){      //����2~9��Ƭ�����
        for(int k = 0;k < 9;k++) {
            tmp[k] = ans[k];
            ans[k] += dp[num[i]][i][k];
            ans[k] %= mod;
        }
        for(int j = 0;j < 9;j++){   //ö��ans������
            for(int k = 0;k < 9;k++){//ö��i��Ƭ������
                int l = (j + k) % 9;
                ans[l] =  ((LL)ans[l] + (LL)tmp[j]*(LL)dp[num[i]][i][k])%mod;
            }
        }
    }
    int ret;
    ret = (((LL)ans[0] + 1) * (LL) power(num[0]) - 1) % mod;  //����0��Ƭ��Ҫ���Ϻܶ����  (ret + 1) * 2 ^ n - 1
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