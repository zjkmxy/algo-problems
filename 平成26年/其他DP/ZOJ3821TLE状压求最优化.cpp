#include<cstdio>
#include<cctype>
#include<queue>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXX	8
#define MAXY	15
#define MAXS	65536
#define MAXH	16
//16HP是圣盾
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef BYTE CODE[9];
int dp[MAXY+1][MAXH+1][MAXS];	//危险值[随从个数][剩余体力][攻击状态]
int X, Y, Z, M;
struct MINION{
	int a, h;
	bool ds, t, wf, cg;
	bool operator<(const MINION& other)const{
		if(t != other.t)
			return t > other.t;
		if(a != other.a)
			return a < other.a;
		else
			return h < other.h;
	}
}pl[MAXX+1], en[MAXY+1];	//随从，0开始，最后一个放空
int npl, nen, ntt;	//己方随从，敌方随从，敌方嘲讽随从

void GetMin(MINION &ret)
{
	char str[15];
	scanf("%d/%d", &ret.a, &ret.h);
	ret.ds = ret.t = ret.wf = ret.cg = false;
	while(getchar() == ' '){
		scanf("%s", str);
		if(str[0] == 'D')
			ret.ds = true;
		else if(str[0] == 'W')
			ret.wf = true;
		else if(str[0] == 'C')
			ret.cg = true;
		else if(str[0] == 'T')
			ret.t = true;
		else
			ret.t = *(int*)NULL;
	}
}

WORD encode(CODE a, int n)
{
	int i;
	WORD ret = 0;
	for(i=0;i<n;i++)
		ret |= (a[i] << (i << 1));
	return ret;
}

void decode(WORD c, CODE a, int n)
{
	int i;
	for(i=0;i<n;i++){
		a[i] = (c >> (i << 1)) & 0x03;
	}
}

void dp_sub(MINION *A, MINION *B, int start, int end, WORD mst)
{
	int i, j, k, risk, newhp;
	WORD st, newst;
	for(i=start;i<end;i++){
		for(j=16;j>=0;j--){
			if(j < 16 && j > B[i].h)	//血量超标的不存在
				continue;
			if(j == 16 && !B[i].ds)	//16血没圣盾的随从不存在
				continue;
			for(st=mst;st>=0;st--){
				if(dp[i][j][st] < 0){
					if(st == 0)
						break;
					else
						continue;
				}

				//选择1：承担危险
				risk = B[i].a * (B[i].wf ? 2 : 1);	//风怒要翻倍
				risk = dp[i][j][st] + (j > 0 ? risk : 0);	//空血生物没有危险
				newhp = (B[i+1].ds ? 16 : B[i+1].h);	//新随从的体力，有卫兵
				if(dp[i+1][newhp][st] == -1 || dp[i+1][newhp][st] > risk)
					dp[i+1][newhp][st] = risk;

				//选择2：用k号生物攻击
				for(k=0;j > 0 && k < npl;k++){
					if(!(st & (0x3 << (k << 1))))
						continue;
					if(B[i].a >= A[k].h && !A[k].ds)	//被干死的话就不能继续输出了
						newst = st & (0xffff - (0x3 << (k << 1)));
					else								//不然只是少了一次输出机会而已
						newst = st - (0x1 << (k << 1));
					newhp = (j == 16 ? B[i].h : max(0, j - A[k].a));
					if(dp[i][newhp][newst] < 0 || dp[i][newhp][newst] > dp[i][j][st])
						dp[i][newhp][newst] = dp[i][j][st];
				}//FOR ATTACK MINION

				if(st == 0)
					break;
			}//FOR STATE
		}//FOR HEALTH
	}//FOR ENEMY COUNT
}

void dp_calc(MINION *A, MINION *B, int cnt, int cnt2)
{
	int i, k, tmp, newhp, risk, atk;
	WORD st, mst;
	CODE arr = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	//初始化DP数组
	memset(dp, -1, sizeof(dp));
	for(i=0;i<npl;i++){
		arr[i] = (A[i].wf ? 2 : 1);
	}
	mst = encode(arr, npl);
	newhp = (B[0].ds ? 16 : B[0].h);
	dp[0][newhp][mst] = 0;

	//嘲讽计算
	dp_sub(A, B, 0, cnt, mst);
	
	//嘲讽筛选：只有那些风险为0的，可以参与到下一轮的DP
	risk = 0x3fffffff;	//最少风险
	atk = 0;	//最大输出
	newhp = (B[cnt].ds ? 16 : B[cnt].h);
	for(st=mst;st>=0;st--){
		if(dp[cnt][newhp][st] < 0){
			if(st > 0)
				continue;
			else
				break;
		}
		risk = min(risk, dp[cnt][newhp][st]);
		if(dp[cnt][newhp][st] == 0){
			tmp = 0;
			decode(st, arr, npl);
			for(k=0;k<npl;k++)
				tmp += arr[k] * A[k].a;
			atk = max(atk, tmp);
		}else{
			dp[cnt][newhp][st] = -1;	//嘲讽没打完的，不能继续输出
		}
		if(st == 0)
			break;
	}
	if(atk >= M){	//OTK
		printf("Well played\n");
		return ;
	}
	if(risk > 0){	//破墙失败
		for(i=cnt;i<cnt2;i++)
			risk += B[i].a * (B[i].wf ? 2 : 1);
		printf("%d %d\n", risk, 0);
		return ;
	}

	//普通计算
	dp_sub(A, B, cnt, cnt2, mst);
	risk = 0x3fffffff;	//最少风险
	atk = 0;	//最大输出
	for(st=mst;st>=0;st--){
		if(dp[cnt2][0][st] < 0){
			if(st > 0)
				continue;
			else
				break;
		}
		decode(st, arr, npl);
		tmp = 0;
		for(k=0;k<npl;k++)
			tmp += arr[k] * A[k].a;
		
		if(dp[cnt2][0][st] < risk){
			risk = dp[cnt2][0][st];
			atk = tmp;
		}else if(dp[cnt2][0][st] == risk){
			atk = max(atk, tmp);
		}
		if(st == 0)
			break;
	}
	printf("%d %d\n", risk, atk);
}

bool input()
{
	int i;
	npl = nen = ntt = 0;
	if(scanf("%d %d %d %d", &X, &Y, &Z, &M)==EOF)
		return false;
	for(i=0;i<X;i++){
		GetMin(pl[npl]);
		npl++;
	}
	for(i=0;i<Y;i++){
		GetMin(en[nen]);
		nen++;
	}
	for(i=0;i<Z;i++){
		GetMin(pl[npl]);
		if(pl[npl].cg)
			npl++;
	}
	sort(pl, pl + npl);
	sort(en, en + nen);
	memset(&en[nen], 0, sizeof(MINION));
	for(i=0;i<nen;i++){
		if(en[i].t)
			ntt++;
		else
			break;
	}
	return true;
}

int main()
{
	int T;
	scanf("%d", &T);
	while(T--){
		input();
		dp_calc(pl, en, ntt, nen);
	}
	return 0l;
}
