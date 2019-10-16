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
//16HP��ʥ��
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef BYTE CODE[9];
int dp[MAXY+1][MAXH+1][MAXS];	//Σ��ֵ[��Ӹ���][ʣ������][����״̬]
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
}pl[MAXX+1], en[MAXY+1];	//��ӣ�0��ʼ�����һ���ſ�
int npl, nen, ntt;	//������ӣ��з���ӣ��з��������

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
			if(j < 16 && j > B[i].h)	//Ѫ������Ĳ�����
				continue;
			if(j == 16 && !B[i].ds)	//16Ѫûʥ�ܵ���Ӳ�����
				continue;
			for(st=mst;st>=0;st--){
				if(dp[i][j][st] < 0){
					if(st == 0)
						break;
					else
						continue;
				}

				//ѡ��1���е�Σ��
				risk = B[i].a * (B[i].wf ? 2 : 1);	//��ŭҪ����
				risk = dp[i][j][st] + (j > 0 ? risk : 0);	//��Ѫ����û��Σ��
				newhp = (B[i+1].ds ? 16 : B[i+1].h);	//����ӵ�������������
				if(dp[i+1][newhp][st] == -1 || dp[i+1][newhp][st] > risk)
					dp[i+1][newhp][st] = risk;

				//ѡ��2����k�����﹥��
				for(k=0;j > 0 && k < npl;k++){
					if(!(st & (0x3 << (k << 1))))
						continue;
					if(B[i].a >= A[k].h && !A[k].ds)	//�������Ļ��Ͳ��ܼ��������
						newst = st & (0xffff - (0x3 << (k << 1)));
					else								//��Ȼֻ������һ������������
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

	//��ʼ��DP����
	memset(dp, -1, sizeof(dp));
	for(i=0;i<npl;i++){
		arr[i] = (A[i].wf ? 2 : 1);
	}
	mst = encode(arr, npl);
	newhp = (B[0].ds ? 16 : B[0].h);
	dp[0][newhp][mst] = 0;

	//�������
	dp_sub(A, B, 0, cnt, mst);
	
	//����ɸѡ��ֻ����Щ����Ϊ0�ģ����Բ��뵽��һ�ֵ�DP
	risk = 0x3fffffff;	//���ٷ���
	atk = 0;	//������
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
			dp[cnt][newhp][st] = -1;	//����û����ģ����ܼ������
		}
		if(st == 0)
			break;
	}
	if(atk >= M){	//OTK
		printf("Well played\n");
		return ;
	}
	if(risk > 0){	//��ǽʧ��
		for(i=cnt;i<cnt2;i++)
			risk += B[i].a * (B[i].wf ? 2 : 1);
		printf("%d %d\n", risk, 0);
		return ;
	}

	//��ͨ����
	dp_sub(A, B, cnt, cnt2, mst);
	risk = 0x3fffffff;	//���ٷ���
	atk = 0;	//������
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
