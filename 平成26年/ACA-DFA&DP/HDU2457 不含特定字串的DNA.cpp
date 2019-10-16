#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXC 1005
#define MAXL 1005
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
		bool ed;
		inline int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	int dp[MAXL][MAXC];
	int cnt;
	void init()
	{
		cnt = 1;
		memset(&node[0], -1, sizeof(NODE));
		node[0].ed = false;
	}
	void insert(char *str, int len)
	{
		int i, cur = 0;
		for(i=0;i<len;i++){
			if(node[cur][str[i]] == -1){
				node[cur][str[i]] = cnt;
				memset(&node[cnt], -1, sizeof(NODE));
				node[cnt].ch = str[i];
				node[cnt].ed = false;
				cnt++;
			}
			cur = node[cur][str[i]];
		}
		node[cur].ed = true;
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
					if(node[node[p][j]].ed)
						node[node[cur][j]].ed = true;    //重要：重叠的判定
					Q.push(node[cur][j]);
				}else
					node[cur][j] = node[p][j];
			}
		}
	}
	int dp_calc(char *str, int len)
	{
		int i, j, r, k;
		for(i=0;i<=len;i++)
		{
			for(j=0;j<cnt;j++)
			{
				dp[i][j] = INF;
			}
		}
		dp[0][0] = 0;
		for(i=1;i<=len;i++)
		{
			for(j=0;j<cnt;j++)
			{
				if(node[j].ed || dp[i-1][j]==INF)
					continue;
				for(k=0;k<4;k++)
				{
					r = node[j][k];
					if(node[r].ed)
						continue;
					dp[i][r] = min(dp[i][r], dp[i-1][j] + (str[i-1] != k ? 1 : 0));
				}
			}
		}
		r = INF;
		for(j=0;j<cnt;j++)
			r = min(r, dp[len][j]);
		return (r < INF ? r : -1);
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
	int N, cas = 0;

	while(true)
	{
		scanf("%d",&N);
		if(N == 0)
			break;
		aca.init();
		while(N--)
		{
			scanf("%s",str);
			deal();
			aca.insert(str, len);
		}
		aca.build();
		scanf("%s",str);
		deal();
		printf("Case %d: %d\n",++cas,aca.dp_calc(str, len));
	}
	return 0;
}
