#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXN 10005

typedef struct _DATA
{
	int no;
	int mon;
	struct _DATA *next;
}DATA, *PDATA;

int N, M, Q;
int best[MAXN];		//最优所得
DATA head;

//有序链表的插入
PDATA insert(int no, int mon)
{
	PDATA prev, next;
	PDATA ret;

	prev = &head;
	next = prev->next;
	while(next != NULL)
	{
		if(mon <= next->mon)
			break;
		prev = next;
		next = prev->next;
	}
	ret = (PDATA)malloc(sizeof(DATA));
	ret->no = no;
	ret->mon = mon;
	prev->next = ret;
	ret->next = next;
	return ret;
}

//有序链表的修正和整理
void resort(int cnt)
{
	int i;
	PDATA prev, next, split;

	//加1
	prev = &head;
	next = prev->next;
	i = 0;
	while(next != NULL && i < cnt)
	{
		next->mon++;
		prev = next;
		next = prev->next;
		i++;
	}
	//清零
	split = prev;
	while(next != NULL)
	{
		next->mon = 0;
		prev = next;
		next = prev->next;
	}
	//整理
	if(cnt > 0)
	{
		prev->next = head.next;
		head.next = split->next;
		split->next = NULL;
	}
}

//有序链表的求和
int getsum(int cnt)
{
	int i, ret = 0;
	PDATA prev, next;

	prev = &head;
	next = prev->next;
	i = 0;
	while(next != NULL && i < cnt)
	{
		ret += next->mon + 1;
		prev = next;
		next = prev->next;
		i++;
	}
	return ret;
}

void cleanup()
{
	PDATA prev, next;
	prev = &head;
	next = prev->next;
	while(next != NULL)
	{
		best[next->no] = next->mon;
		prev = next;
		next = prev->next;
		free(prev);
	}
}

void arrange()
{
	int i, j;
	int cnt, ans;

	insert(0, M);
	for(i=1;i<N;i++)	//第i个人的回合
	{
		cnt = (i+1) * Q / 100;
		ans = getsum(cnt);
		if(ans > M)
		{
			insert(i, -1);
			continue;
		}
		resort(cnt);
		insert(i, M - ans);
	}
	cleanup();
}

int main()
{
	while(scanf("%d%d%d",&N,&M,&Q)!=EOF)
	{
		head.next = NULL;
		arrange();
		printf("%d\n",best[N-1]);
	}
	return 0;
}