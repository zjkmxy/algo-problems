#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 300
#define MAXL 200
#define MAXI 0x0fffffff
struct NODE{
	int par;   //����
	int lchd;  //����
	int rbro;  //���ֵ�
	int dp[MAXN]; //Ҫ��iѡƱʱ�����ٶ��ٴ���
	int cost;
	int cnt;   //��Ʊ��
}tree[MAXN];   //0�ǹ�����

char name[MAXN][MAXL];
char str[50000], tmpname[MAXL];
int num, cnt, m;
int pos, len;

int ssearch(char *str)
{
	int i;

	for(i=0;i<cnt;i++)
	{
		if(strcmp(str, name[i]) == 0)
			return i;
	}

	//�������³���
	strcpy(name[cnt], str);
	tree[cnt].lchd = NULL;
	tree[cnt].rbro = NULL;
	tree[cnt].par  = NULL;
	tree[cnt].cnt  = 1;
	cnt++;
	return (cnt - 1);
}

//����cur��Ϊs���ӽڵ�
void insert(int cur, int s)
{
	tree[cur].rbro = tree[s].lchd;
	tree[s].lchd = cur;
	tree[cur].par = s;
}

//������Ʊ��
void count(int cur)
{
	int i;

	for(i=tree[cur].lchd;i!=NULL;i=tree[i].rbro)
	{
		count(i);
		tree[cur].cnt += tree[i].cnt;
	}
}

//dp����
void calc(int cur)
{
	int i, j, k, v;

	//������ֵ
	tree[cur].dp[0] = 0;
	v = min(m, tree[cur].cnt);
	for(j=1;j<=v;j++)
	{
		tree[cur].dp[j] = tree[cur].cost;
	}
	for(;j<=m;j++)
	{
		tree[cur].dp[j] = MAXI;
	}

	//�ӽڵ�ϲ�
	for(i=tree[cur].lchd;i!=NULL;i=tree[i].rbro)
	{
		calc(i);
		for(j=v;j>0;j--)  //һ����Ʒ���������Σ����Ա��뵹����
		{
			for(k=0;k<=j;k++)
			{
				tree[cur].dp[j] = min(tree[cur].dp[j], tree[cur].dp[k] + tree[i].dp[j-k]);
			}
		}
	}
}

bool getstr(char *mst)
{
	int i;

	if(pos == len)
		return false;
	while(str[pos] == ' ')
	{
		pos++;
	}
	for(i=0;(str[pos]!=' ')&&(pos<len);i++,pos++)
	{
		mst[i] = str[pos];
	}
	mst[i] = 0;
	return true;
}

int getnum()
{
	int ret = 0;

	while(str[pos] == ' ')
	{
		pos++;
	}
	while((str[pos]!=' ')&&(pos<len))
	{
		ret *= 10;
		ret += str[pos] - '0';
		pos++;
	}
	return ret;
}

bool input()
{
	int i, cost, a, b;

	if(gets(str)==NULL)
		return false;
	if(str[0]=='#')
		return false;

	sscanf(str, "%d%d", &num, &m);
	cnt = 1;
	tree[0].lchd = tree[0].rbro = tree[0].par = NULL;
	tree[0].cost = MAXI;
	tree[0].cnt  = 0;
	for(i=1;i<=num;i++)
	{
		gets(str);
		len = strlen(str);
		pos = 0;
		getstr(tmpname);
		a = ssearch(tmpname);
		tree[a].cost = getnum();
		while(getstr(tmpname))
		{
			b = ssearch(tmpname);
			insert(b, a);
		}
	}
	for(i=1;i<=num;i++)
	{
		if(tree[i].par==0)
			insert(i, 0);
	}

	return true;
}

int main()
{
	while(input())
	{
		count(0);
		calc(0);
		printf("%d\n",tree[0].dp[m]);
	}
	return 0;
}
