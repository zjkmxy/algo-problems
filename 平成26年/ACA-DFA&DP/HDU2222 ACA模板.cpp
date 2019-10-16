#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 10005
#define MAXC 500005
#define MAXL 1000005
#define ALB  26
#define BAS  'a'
void assert(bool exp)
{
	int a = 0;
	if(!exp) exit(0);
}
class ACA
{
public:
	struct NODE
	{
		char ch;        //当前字符
		int fail;        //失败指针
		int next[ALB];    //转移指针
		int ed;            //结尾个数
		int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	int cnt;
	void init()
	{
		cnt = 1;
		memset(&node[0], -1, sizeof(NODE));
		node[0].ed = 0;
	}
	void insert(char *str)
	{
		int i, cur = 0, len = strlen(str);
		for(i=0;i<len;i++){
			assert(str[i] >= 'a' && str[i] <= 'z');
			if(node[cur][str[i]-BAS] == -1){
				node[cur][str[i]-BAS] = cnt;
				memset(&node[cnt], -1, sizeof(NODE));
				node[cnt].ch = str[i];
				node[cnt].ed = 0;
				cnt++;
			}
			cur = node[cur][str[i]-BAS];
		}
		node[cur].ed++;
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
	int match(char *str)
	{
		int i, p, cur = 0, ret = 0, len = strlen(str);
		for(i=0;i<cnt;i++)
			node[i].vis = false;
		for(i=0;i<len;i++){
			assert(str[i] >= 'a' && str[i] <= 'z');
			p = cur = node[cur][str[i]-BAS];
			while(p > 0 && !node[p].vis){
				ret += node[p].ed;
				node[p].vis = true;
				p = node[p].fail;
			}
		}
		return ret;
	}
}aca;

char str[MAXL];
int main()
{
	int T, N;
	scanf("%d",&T);
	while(T--)
	{
		scanf("%d",&N);
		aca.init();
		while(N--)
		{
			scanf("%s",str);
			aca.insert(str);
		}
		aca.build();
		scanf("%s",str);
		printf("%d\n",aca.match(str));
	}
	return 0;
}
