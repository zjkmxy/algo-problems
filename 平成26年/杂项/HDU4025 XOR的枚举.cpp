#include<cstdio>
#include<algorithm>
#include<map>
using namespace std;

typedef long long int LL;
#define MAXM 23
#define MAXN 31
int K[MAXM], S[MAXM][4];
LL arg[MAXM];
LL var[MAXM];
map<LL, LL> cnt;
int A[MAXN][MAXM];
int N, M;
int st, ed;	//枚举的起终点，终结值
LL ans, cv;

void calc(int pos)
{
	int cur, i, j;
	if(pos > ed){
		cnt[0]++;
		return ;
	}
	for(j=0;j<K[pos];j++){
		cur = S[pos][j];
		var[pos] = 0;
		for(i=0;i<N;i++)
			var[pos] |= LL(cur) << LL(i << 1);
		if(pos == ed){
			cv = 0;
			for(i=st;i<=ed;i++)
				cv ^= (arg[i] & var[i]);
			if(ed < M){
				cnt[cv]++;
			}else{
				ans += cnt[cv];
			}
		}else{
			calc(pos + 1);
		}
	}
}

void build()
{
	int i, j, tmp;
	memset(arg, 0, sizeof(arg));
	cnt.clear();
	ans = 0;
	for(i=1;i<=M;i++){
		for(j=1;j<=N;j++){
			tmp = (j - 1) << 1;
			arg[i] |= LL(A[j][i]) << LL(tmp);
			++tmp;
			arg[i] |= LL(A[j][i]) << LL(tmp);
		}
	}
}

void domain()
{
	int i, j;
	scanf("%d %d", &N, &M);
	for(i=1;i<=N;i++)
		for(j=1;j<=M;j++)
			scanf("%d",&A[i][j]);
	for(i=1;i<=M;i++)
	{
		scanf("%d",&K[i]);
		for(j=0;j<K[i];j++)
			scanf("%d",&S[i][j]);
	}
	build();
	st = 1;
	ed = M >> 1;
	calc(st);
	st = ed + 1;
	ed = M;
	calc(st);
	printf("%I64d\n",ans);
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
		domain();
	return 0;
}
