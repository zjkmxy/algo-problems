#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

typedef long long int LL;
#define MAXC 105
#define MAXL 11
#define MAXS 1024
#define MAXN 10
#define ALB  26
#define MODN 20090717
#define INF  0x3fffffff

int bitcnt(unsigned int x)
{
	int j, t;
	unsigned int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};
	for(j = 0, t = 1; j < 5; j ++, t <<= 1)
		x = (x & b[j]) + ((x >> t) & b[j]);
	return x;
}

class ACA
{
public:
	struct NODE
	{
		char ch;		//µ±Ç°×Ö·û
		int fail;		//Ê§°ÜÖ¸Õë
		int next[ALB];	//×ªÒÆÖ¸Õë
		int ed;			//ÖÕ½á×´Ì¬
		inline int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	LL dp[30][MAXC][MAXS];
	int cnt, scnt;
	void init()
	{
		cnt = 1;
		scnt = 0;
		memset(&node[0], -1, sizeof(NODE));
		node[0].ed = 0;
	}
	void insert(char *str, int len)
	{
		int i, cur = 0;
		for(i=0;i<len;i++){
			if(node[cur][str[i]] == -1){
				node[cur][str[i]] = cnt;
				memset(&node[cnt], -1, sizeof(NODE));
				node[cnt].ch = str[i];
				node[cnt].ed = 0;
				cnt++;
			}
			cur = node[cur][str[i]];
		}
		node[cur].ed = (1 << scnt);
		scnt++;
	}
	void build()
	{
		queue<int> Q;
		int cur, j, p;
		for(j=0;j<ALB;j++){
			if(node[0][j] >= 0){
				Q.push(node[0][j]);
				node[node[0][j]].fail = 0;
			}else
				node[0][j] = 0;
		}
		while(!Q.empty()){
			cur = Q.front();
			Q.pop();
			p = node[cur].fail;
			for(j=0;j<ALB;j++){
				if(node[cur][j] >= 0){
					node[node[cur][j]].fail = node[p][j];
					node[node[cur][j]].ed |= node[node[p][j]].ed;
					Q.push(node[cur][j]);
				}else
					node[cur][j] = node[p][j];
			}
		}
	}
	LL dp_calc(int len, int mk)
	{
		int ans = 0, cur;
		int i, j, st, r, k, dst;
		//memset(dp, 0, sizeof(dp));
		for(i=0;i<=len;i++)
			for(j=0;j<cnt;j++)
				for(st=0;st<(1<<scnt);st++)
					dp[i][j][st] = 0;
		dp[0][0][0] = 1;
		for(i=1;i<=len;i++)
		{
			for(j=0;j<cnt;j++)
			{
				for(st=0;st<(1<<scnt);st++)
				{
					if(!dp[i-1][j][st])
						continue;
					for(k=0;k<ALB;k++)
					{
						r = node[j][k];
						dst = st | node[r].ed;
						dp[i][r][dst] = (dp[i][r][dst] + dp[i-1][j][st]);
						if(dp[i][r][dst] >= MODN)
							dp[i][r][dst] -= MODN;
					}
				}
			}
		}
		for(st=0;st<(1<<scnt);st++)
		{
			if(bitcnt(st) < mk)
				continue;
			for(j=0;j<cnt;j++)
			{
				ans = (ans + dp[len][j][st]) % MODN;
				if(ans >= MODN)
					ans -= MODN;
			}
		}
		return ans;
	}
}aca;

char str[MAXL];
int len;
void deal()
{
	int i;
	len = strlen(str);
	for(i=0;i<len;i++)
		str[i] -= 'a';
}

int main()
{
	int N, L, K, i;
	LL ans;

	while(scanf("%d%d%d",&L,&N,&K)!=EOF)
	{
		if(L == 0)
			break;
		aca.init();
		for(i=0;i<N;i++)
		{
			scanf("%s",str);
			deal();
			aca.insert(str, len);
		}
		aca.build();
		ans = aca.dp_calc(L, K);
		printf("%I64d\n",ans);
	}
	return 0;
}
