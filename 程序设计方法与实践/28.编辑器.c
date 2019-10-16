#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAXN	1000005
#define BOOL	int
#define TRUE	1
#define FALSE	0
typedef struct _NODE
{
	int val, sum;
	struct _NODE *prev, *next;
}NODE, *PNODE;
struct POOL{
	NODE arr[MAXN];
	int cnt;
} pool;
PNODE head, cur;
int stsum[MAXN], stpos[MAXN], sp, pos;	//单调栈

inline PNODE alloc()
{
	PNODE ret = pool.arr + pool.cnt;
	++pool.cnt;
	return ret;
}

inline void freeall()
{
	pool.cnt = 1;
	cur = head = pool.arr;
        pool.arr[0].next = 0;
	sp = pos = 0;
}

void SHL()
{
	if(pos == 0)	//光标前已没有元素
		return ;
	if(stpos[sp-1] == pos)	//出栈
		sp--;
	pos--;
	cur = cur->prev;
	if(cur == NULL)		//应该是栈头
		pos = *(int*)NULL;
}

void SHR()
{
	if(cur->next == NULL)
		return;
	++pos;
	cur->next->sum = cur->sum + cur->next->val;
	cur = cur->next;
	if(sp == 0 || cur->sum > stsum[sp-1])	//入栈
	{
		stsum[sp] = cur->sum;
		stpos[sp] = pos;
		sp++;
	}
}

int QUE(int k)
{
	int l = 0, r = sp - 1, mid;
	if(sp == 0 || k <= 0)
		return 0;	//无效查询
	while(r - l > 1)
	{
		mid = (l + r) >> 1;
		if(stpos[mid] == k)
			return stsum[mid];
		if(stpos[mid] < k)
			l = mid;
		else
			r = mid;
	}
	if(stpos[r] <= k)
		return stsum[r];
	else
		return stsum[l];
}

void DEL()
{
	if(pos == 0)
		pos = *(int*)NULL;	//光标在最开头：非法删除

	cur->prev->next = cur->next;	//不可能不存在
	if(cur->next)
		cur->next->prev = cur->prev;
	if(stpos[sp-1] == pos)
		sp--;
	cur = cur->prev;
	pos--;
}

void INS(int val)
{
	PNODE ret = alloc();
	ret->val = val;
	ret->next = cur->next;
	ret->prev = cur;
	if(cur->next)
		cur->next->prev = ret;
	cur->next = ret;
	SHR();
}

BOOL domain()
{
	int Q, a;
	char op[3];
	if(scanf("%d",&Q)==EOF)
		return FALSE;
	freeall();
	while(Q--)
	{
		scanf("%s",op);
		switch(op[0])
		{
		case 'I':
			scanf("%d",&a);
			INS(a);
			break;
		case 'D':
			DEL();
			break;
		case 'L':
			SHL();
			break;
		case 'R':
			SHR();
			break;
		case 'Q':
			scanf("%d",&a);
			printf("%d\n",QUE(a));
			break;
		}
	}
	return TRUE;
}

int main()
{
	while(domain());
	return 0;
}