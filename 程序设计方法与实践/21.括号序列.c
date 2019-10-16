#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXN 500005

char str[MAXN];
int st[MAXN];
int pos, len;
int l, r, cnt;

void calc(int posa, int posb)
{
	int i, c = 0;
	for(i=posa;i<=posb;i++)
	{
		if(str[i] == '[')
			c++;
	}
	if(c < cnt)
		return ;
	if(c == cnt && posb - posa < r - l)
		return ;
	r = posb;
	l = posa;
	cnt = c;
}

void domain()
{
	int i;

	pos = 0;
	len = strlen(str);
	for(i=0;i<len;i++)
	{
		if(pos > 0)
		{
			if((str[st[pos-1]] == '[' && str[i] == ']') || (str[st[pos-1]] == '(' && str[i] == ')'))
			{
				pos--;
				continue;
			}
		}
		st[pos] = i;
		pos++;
	}
	if(pos == len)
	{
		cnt = 0;
		return ;
	}
	st[pos] = len;
	calc(0, st[0]-1);
	for(i=0;i<pos;i++)
		calc(st[i]+1, st[i+1]-1);
}

int main()
{
	scanf("%s",str);
	domain();
	printf("%d\n",cnt);
	if(cnt > 0)
	{
		printf("%d %d\n",l, r);
	}
	return 0;
}