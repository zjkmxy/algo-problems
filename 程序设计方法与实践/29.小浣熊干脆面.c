#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define BOOL	int
#define TRUE	1
#define FALSE	0
#define MAXM	2005
#define MAXN	1000005
int cnt[MAXM], scnt;
int queue[MAXN], head, tail;
int N, M;

BOOL domain()
{
	int a, ret = 0x3fffffff;
	if(scanf("%d%d", &N, &M) == EOF)
		return FALSE;
	head = tail = 0;
	scnt = 0;
	for(a = 0; a <= M; a++)
		cnt[a] = 0;
	while(N--)
	{
		scanf("%d", &a);
		cnt[a]++;
		if(cnt[a] == 1)
			scnt++;
		queue[tail++] = a;
		if(scnt < M)
			continue;
		while(cnt[queue[head]] > 1 && head < tail)
		{
			cnt[queue[head]]--;
			head++;
		}
		if(tail - head < ret)
			ret = tail - head;
	}
	printf("%d\n", ret);
	return TRUE;
}

int main()
{
	while(domain());
	return 0;
}