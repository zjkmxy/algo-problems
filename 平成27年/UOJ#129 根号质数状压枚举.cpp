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
int pl[MAXP], npl;	//������
int ps[10], nps;	//С����
vector<int> grp[MAXP];	//ÿ�����
int msk[MAXN];	//����������С����������
int N, P;

void Prepare(){
	int i, j, lim = floor(sqrt(N));

	//Ԥ�����С����
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
	//Ԥ������
	for(i = 2; i <= N; i ++){
		//����
		for(j = npl - 1; j > 0 && i % pl[j] > 0; j --);
		grp[j].push_back(i);
		//��������
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
	static LL dp[2][MAXS][MAXS];	//ABȡ��С����״���µķ�����
	static LL cur[MAXS][MAXS][2];	//��ǰ���£�A��B������ķ�����
	const int ms = (1 << nps);	//״̬����
	int rd = 0, wt = 1;
	int i, sta, stb;
	LL ret = 0;

	memset(dp, 0, sizeof(dp));
	dp[rd][0][0] = 1;
	//��������ֵ
	for(auto v : grp[0]){
		for(sta = ms - 1; sta >= 0; sta --){
			for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){ //����ǿ���ö���Ӽ���
				if((stb & msk[v]) == 0)
					update(dp[rd][sta | msk[v]][stb], dp[rd][sta][stb]);
				if((sta & msk[v]) == 0)
					update(dp[rd][sta][stb | msk[v]], dp[rd][sta][stb]);
			}
		}
	}
	//ö����
	for(i = 1; i < npl; i ++, rd ^= 1, wt ^= 1){
		memset(cur, 0, sizeof(cur));
		memset(dp[wt], 0, sizeof(dp[wt]));

		//ö�����ڵ���
		for(auto v : grp[i]){
			//���ǵ��໥Ӱ�죬״̬��Ҫ����ö��
			for(sta = ms - 1; sta >= 0; sta --){
				for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){
					//����A�����
					if((stb & msk[v]) == 0){
						//֮ǰA���飬���ο�����Ҳ���Բ���
						update(cur[sta | msk[v]][stb][0], cur[sta][stb][0]);
						//֮ǰA���飬���˿��Խ���
						update(cur[sta | msk[v]][stb][0], dp[rd][sta][stb]);
					}
					//����B�����
					if((sta & msk[v]) == 0){
						update(cur[sta][stb | msk[v]][1], cur[sta][stb][1]);
						update(cur[sta][stb | msk[v]][1], dp[rd][sta][stb]);
					}
				}
			}
		}
		//�������ڵ�������dpֵ
		for(sta = 0; sta < ms; sta ++){
			for(stb = (ms - 1) ^ sta; stb >= 0; stb --, stb &= ~sta){
				update(dp[wt][sta][stb], dp[rd][sta][stb]);	//˫��������
				update(dp[wt][sta][stb], cur[sta][stb][0]);	//A����
				update(dp[wt][sta][stb], cur[sta][stb][1]);	//B����
			}
		}
	}
	//ͳ�ƴ�
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