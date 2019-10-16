#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
using namespace std;

#define MAXN 100000
#define MAXI 0x3fffffff

struct NODE
{
	char ch;  //当前字符
	int go;   //转移数

	int lchd;
	int lbro;
	int rbro;
	int par;
	int depth;
}node[MAXN];
int tcnt;

int ins_code[MAXN];
int nxtv[MAXN];
char gstr[MAXN];

void get_nextval(char *str, int *nextval)    
{
	int i, j, len = strlen(str);

	nextval[0] = -1;
	for(i=0,j=-1;i<len-1;)
	{
		if(j == -1 || str[i] == str[j])
		{
			i++;
			j++;
			if(str[i] == str[j])
				nextval[i] = nextval[j];
			else
				nextval[i] = j;
		}else{
			j = nextval[j];
		}
	}
}

void insert(char *str,  int *nextval)
{
	int i, pos, len = strlen(str);
	int j, tmp = -1;

	pos = 0;
    for(i=0;i<len;i++)
    {
        j = node[pos].lchd;
        tmp = -1;
        while(j != -1 && node[j].ch < str[i])
        {
            tmp = j;
            j = node[j].rbro;
        }
        if(node[j].ch == str[i])  //找到了
        {
            node[j].depth = min(node[j].depth, len - i - 1);
            pos = j;
			ins_code[i] = pos;
            continue;
        }
        if(tmp == -1)  //无孩子或最左孩子
        {
            node[tcnt].ch = str[i];
            node[tcnt].par = pos;
            node[tcnt].rbro = node[pos].lchd;
            if(node[tcnt].rbro >= 0)
            {
                node[node[tcnt].rbro].lbro = tcnt;
            }
            node[pos].lchd = tcnt;
            node[tcnt].lchd = -1;
            node[tcnt].lbro = -1;
            node[tcnt].depth = len - i - 1;
            pos = tcnt;
            tcnt++;

			ins_code[i] = pos;
			if(nextval[i+1] == -1)
				node[pos].go = 0;
			else
				node[pos].go = node[ins_code[nextval[i+1]]].par;
            continue;
        }
        if(j == -1)  //最右孩子
        {
            node[tcnt].ch = str[i];
            node[tcnt].par = pos;
            node[tmp].rbro = tcnt;
            node[tcnt].lchd = -1;
            node[tcnt].rbro = -1;
            node[tcnt].lbro = tmp;
            node[tcnt].depth = len - i - 1;
            pos = tcnt;
            tcnt++;

			ins_code[i] = pos;
			if(nextval[i+1] == -1)
				node[pos].go = 0;
			else
				node[pos].go = node[ins_code[nextval[i+1]]].par;
            continue;
        }
        if(node[j].ch > str[i])  //中间孩子
        {
            node[tcnt].ch = str[i];
            node[tcnt].par = pos;
            node[tmp].rbro = tcnt;
            node[j].lbro = tcnt;
            node[tcnt].lchd = -1;
            node[tcnt].rbro = j;
            node[tcnt].lbro = tmp;
            node[tcnt].depth = len - i - 1;
            pos = tcnt;
            tcnt++;

			ins_code[i] = pos;
			if(nextval[i+1] == -1)
				node[pos].go = 0;
			else
				node[pos].go = node[ins_code[nextval[i+1]]].par;
            continue;
        }
    }
}

void init()
{
    memset(node, 0, sizeof(node));
    tcnt = 1;
    node[0].ch = '\0';
    node[0].lchd = -1;
    node[0].rbro = -1;
    node[0].lbro = -1;
    node[0].par = 0;
	node[0].depth = MAXI;
	node[0].go = 0;
}

bool match(char *str)
{
	int i, pos, len = strlen(str);
	int j;

	pos = 0;
	for(i=0;i<len && node[pos].depth > 0;i++)
    {
        j = node[pos].lchd;
        while(j != -1 && node[j].ch < str[i])
        {
            j = node[j].rbro;
        }
        if(node[j].ch == str[i])  //找到了
        {
            pos = j;
            continue;
        }else{
			if(pos != 0)
			{
				i--;
				pos = node[pos].go;
			}
		}
	}
	if(node[pos].depth == 0)
		return true;
	else
		return false;
}

int main()
{
	int i, n;

	init();
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%s",&gstr);
		get_nextval(gstr, nxtv);
		insert(gstr, nxtv);
	}
	while(scanf("%s",gstr)!=EOF)
	{
		if(match(gstr))
		{
			printf("True\n");
		}else{
			printf("False\n");
		}
	}
	return 0;
}
