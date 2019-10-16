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
int best[MAXN];		//��������
DATA head;

//��������Ĳ���
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

//�������������������
void resort(int cnt)
{
	int i;
	PDATA prev, next, split;

	//��1
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
	//����
	split = prev;
	while(next != NULL)
	{
		next->mon = 0;
		prev = next;
		next = prev->next;
	}
	//����
	if(cnt > 0)
	{
		prev->next = head.next;
		head.next = split->next;
		split->next = NULL;
	}
}

//������������
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
	for(i=1;i<N;i++)	//��i���˵Ļغ�
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