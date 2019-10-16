#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 505
#define MAXC 100005
#define MAXL 10005
#define ALB  128
#define BAS  0
void assert(bool exp)
{
	int a = 0;
	if(!exp) exit(0);
}
class ACA
{
public:
	int strdta[MAXN], indx;
	int ans[MAXN], anscnt;
	struct NODE
	{
		char ch;		//当前字符
		int fail;		//失败指针
		int next[ALB];	//转移指针
		int ed;			//结尾个数
		int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	int cnt;
	void init()
	{
		cnt = 1;
		memset(&node[0], -1, sizeof(NODE));
		indx = 0;
	}
	void insert(char *str)
	{
		int i, cur = 0, len = strlen(str);
		for(i=0;i<len;i++){
			if(node[cur][str[i]-BAS] == -1){
				node[cur][str[i]-BAS] = cnt;
				memset(&node[cnt], -1, sizeof(NODE));
				node[cnt].ch = str[i];
				cnt++;
			}
			cur = node[cur][str[i]-BAS];
		}
		strdta[indx] = node[cur].ed;
		node[cur].ed = indx;
		indx++;
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
					Q.push(node[cur][j]);
				}else
					node[cur][j] = node[p][j];
			}
		}
	}
	void match(char *str)
	{
		int i, p, cur = 0, len = strlen(str);
		int tmp;
		anscnt = 0;
		for(i=0;i<cnt;i++)
			node[i].vis = false;
		for(i=0;i<len;i++){
			p = cur = node[cur][str[i]-BAS];
			while(p > 0 && !node[p].vis){
				node[p].vis = true;
				for(tmp=node[p].ed;tmp>=0;tmp=strdta[tmp])
					ans[anscnt++] = tmp + 1;
				p = node[p].fail;
			}
		}
	}
}aca;

char str[MAXL];
int main()
{
	int T, N, acnt, i, j;
	//while(T--)
	{
		scanf("%d\n",&N);
		aca.init();
		while(N--)
		{
			gets(str);
			aca.insert(str);
		}
		aca.build();
		scanf("%d\n",&N);
		acnt = 0;
		for(j=1;j<=N;j++)
		{
			gets(str);
			aca.match(str);
			if(aca.anscnt > 0)
			{
				printf("web %d:",j);
				sort(aca.ans, aca.ans + aca.anscnt);
				for(i=0;i<aca.anscnt;i++)
				{
					printf(" %d",aca.ans[i]);
				}
				printf("\n");
				acnt++;
			}
		}
		printf("total: %d\n",acnt);
	}
	return 0;
}
