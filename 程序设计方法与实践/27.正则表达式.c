#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef int BOOL;
#define TRUE	1
#define FALSE	0
#define MAXN	105
#define MAXM	25

BOOL dp[MAXM][MAXN];//前i位匹配目标j位可否
typedef struct _TOKEN
{
	char ch;	//ch,^,$,.
	char ty;	//?,+,*
}TOKEN, *PTOKEN;
TOKEN pmat[MAXM];
char pstr[MAXM], str[MAXN];	//均为1起始，但^是0，$是plen+1
int plen, len;

BOOL ismatch(char ch, int tk)
{
	if(ch == pmat[tk].ch)
		return TRUE;
	if(pmat[tk].ch == '.')
		return TRUE;
	return FALSE;
}

BOOL dp_calc(char *str, int len)
{
	int i, j;
	memset(dp, 0, sizeof(dp));
	dp[0][0] = TRUE;
	if(pmat[0].ch != '^')
		for(j=1;j<=len;j++)
			dp[0][j] = TRUE;
	for(j=1;j<=len;j++)
	{
		for(i=1;i<=plen;i++)
		{
			if(pmat[i].ty == '*' || pmat[i].ty == '?')	//空匹配
				dp[i][j] |= dp[i-1][j];
			if(pmat[i].ty == '*' || pmat[i].ty == '+')	//多匹配
			{
				if(str[j] == pmat[i].ch || pmat[i].ch == '.')
					dp[i][j] |= dp[i][j-1];
			}
			if(!dp[i-1][j-1])
				continue;
			dp[i][j] |= ismatch(str[j], i);
		}
		if(dp[plen][j])
		{
			if(pmat[plen+1].ch != '$')
				return TRUE;
		}
	}
	return dp[plen][len];
}

void build()
{
	int i = 0;
	if(pstr[0] == '^')
	{
		pmat[0].ch = '^';
		i = 1;
	}
	for(plen=1;pstr[i]>0;i++)
	{
		pmat[plen++].ch = pstr[i];
		if(pstr[i+1] == '?' || pstr[i+1] == '+' || pstr[i+1] == '*')
		{
			pmat[plen-1].ty = pstr[i+1];
			i++;
		}
	}
	plen--;
	if(pmat[plen].ch == '$')
		plen--;
}

int main()
{
	int i;
	gets(pstr);
	build();
	while(gets(str+1)!=NULL)
	{
		len = strlen(str+1);
		str[0] = '^';
		str[len+1] = '$';
		str[len+2] = 0;
		if(dp_calc(str, len))
		{
			printf("YES\n");
			continue;
		}
		printf("NO\n");
	}
	return 0;
}