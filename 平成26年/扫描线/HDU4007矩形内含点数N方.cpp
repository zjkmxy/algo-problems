/* 
1.N^2�㷨
��ɨ����ɨ���ΪH�����߿�ȵľ��ο�����������У�����ά�������еĵ��ڿ��У�ͬʱ��������Ԫ�غ�����ĵ����ԡ�
Ȼ����һά�ĵ���ջ����⡣

2.�߶���
��ÿһ����Ϊ���½ǽ���һ��W��H�ľ��Σ����о�������า�Ǵ�����
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define MAXN 100005

struct NODE
{
	int x, y;
	int prev;
	int next;
}nodes[MAXN];
struct ARR
{
	int x, y;
}arr[MAXN];
int N, W, H, pos;

int query()
{
	int i, j;
	int maxn = 0, cur = 0;

	i = j = nodes[0].next;
	while(j >= 0)
	{
		cur++;
		while(nodes[i].x + W < nodes[j].x)
		{
			i = nodes[i].next;
			cur--;
		}
		j = nodes[j].next;
		if(maxn < cur)
			maxn = cur;
	}
	return maxn;
}

void update(int p)
{
	int i, j = 0;
	bool flag = false;

	nodes[pos].x = arr[p].x;
	nodes[pos].y = arr[p].y;
	for(i=nodes[0].next;i>=0;i=nodes[i].next)
	{
		while(i > 0 && nodes[i].y + H < arr[p].y)
		{
			j = nodes[i].prev;
			nodes[j].next = nodes[i].next;
			j = nodes[i].next;
			if(j >= 0)
				nodes[j].prev = nodes[i].prev;
			i = j;
		}
		if(!flag && i >= 0 && nodes[i].x > arr[p].x)
		{
			flag = true;
			j = nodes[i].prev;
			nodes[j].next = pos;
			nodes[pos].prev = j;
			nodes[i].prev = pos;
			nodes[pos].next = i;
			pos++;
		}
		j = i;
	}
	if(!flag)
	{
		flag = true;
		nodes[j].next = pos;
		nodes[pos].prev = j;
		nodes[pos].next = -1;
		pos++;
	}
}

int comp(const void*A, const void*B)
{
	ARR *a = (ARR*)A, *b = (ARR*)B;
	return a->y - b->y;
}

bool domain()
{
	int i, maxn = 0, cur;
	if(scanf("%d%d",&N,&W)==EOF)
		return false;
	H = W;
	if(N < 0)
	{
		return false;
	}
	pos = 1;
	nodes[0].next = -1;
	nodes[0].prev = 0;
	for(i=0;i<N;i++)
		scanf("%d%d",&arr[i].x,&arr[i].y);
	qsort(arr, N, sizeof(ARR), comp);
	for(i=0;i<N;i++)
	{
		update(i);
		cur = query();
		if(cur > maxn)
			maxn = cur;
	}
	printf("%d\n",maxn);
	return true;
}

int main(int argc, const char * argv[])
{
	while(domain());
	return 0;
}

