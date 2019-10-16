//每种基因最多只含一个重复不计数，因此要用到状态压缩
#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXC 1005
#define MAXL 101
#define MAXS 1024
#define MAXN 10
#define ALB  4
#define INF  0x3fffffff

class ACA
{
public:
	struct NODE
	{
		char ch;		//当前字符
		int fail;		//失败指针
		int next[ALB];	//转移指针
		int ed;			//终结状态
		inline int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	bool dp[2][MAXC][MAXS];
	int prv[MAXN];
	int cnt, scnt;
	void init()
	{
		cnt = 1;
		scnt = 0;
		memset(&node[0], -1, sizeof(NODE));
		node[0].ed = 0;
	}
	void insert(char *str, int len, int val)
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
		prv[scnt] = val;
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
	int dp_calc(int len)
	{
		int wt, rt, ans = -INF, cur;
		int i, j, st, r, k, dst;
		memset(dp, 0, sizeof(dp));
		dp[0][0][0] = true;
		for(i=1,rt=0,wt=1;i<=len;i++,rt^=1,wt^=1)
		{
			memset(dp[wt], 0, sizeof(dp[wt]));
			for(j=0;j<cnt;j++)
			{
				for(st=0;st<(1<<scnt);st++)
				{
					if(!dp[rt][j][st])
						continue;
					for(k=0;k<ALB;k++)
					{
						r = node[j][k];
						dst = st | node[r].ed;
						dp[wt][r][dst] = true;
					}
				}
			}
		}
		for(j=0;j<cnt;j++)
		{
			for(st=0;st<(1<<scnt);st++)
			{
				if(!dp[rt][j][st])
					continue;
				cur = 0;
				for(i=0;i<scnt;i++)
				{
					if(st & (1 << i))
						cur += prv[i];
				}
				ans = max(ans, cur);
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
	{
		switch(str[i])
		{
		case 'A':
			str[i] = 0;
			break;
		case 'G':
			str[i] = 1;
			break;
		case 'C':
			str[i] = 2;
			break;
		case 'T':
			str[i] = 3;
			break;
		}
	}
}

int main()
{
	int N, L, i, p;

	while(scanf("%d%d",&N,&L)!=EOF)
	{
		aca.init();
		for(i=0;i<N;i++)
		{
			scanf("%s%d",str,&p);
			deal();
			aca.insert(str, len, p);
		}
		aca.build();
		p = aca.dp_calc(L);
		if(p < 0)
			printf("No Rabbit after 2012!\n");
		else
			printf("%d\n",p);
	}
	return 0;
}
