//HDU2196
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

#define MAXN 10005
struct NODE{
	int par;   //���ڵ�
	int down;  //�����·
	int sub;   //�����·;���ӽڵ�
	int down2; //������sub�������·
	int up;    //��������һ�����·
	int head;  //��ʼ��
	int back;  //�ط���
}tree[MAXN];
struct EDGE{
	int end;   //�ӽڵ��
	int len;   //·������
	int next;  //��һ����
}edge[MAXN];

int n;

bool input()
{
	int i, a, w;

	if(scanf("%d",&n)==EOF)
		return false;

	memset(tree, -1, sizeof(NODE) * (n + 1));
	for(i=1;i<=n-1;i++)
	{
		scanf("%d%d", &a, &w);
		edge[i].end = i + 1;
		edge[i].len = w;
		edge[i].next = tree[a].head;
		tree[a].head = i;
		tree[i+1].par = a;
		tree[i+1].back = i;
	}

	return true;
}

void DFSd(int i)
{
	int e, j;

	tree[i].down = 0;
	tree[i].down2 = 0;
	for(e=tree[i].head;e!=-1;e=edge[e].next)
	{
		j = edge[e].end;
		DFSd(j);
		if(tree[j].down + edge[e].len >= tree[i].down)
		{
			tree[i].down2 = tree[i].down;
			tree[i].down = tree[j].down + edge[e].len;
			tree[i].sub = j;
		}else if(tree[j].down + edge[e].len >= tree[i].down2)
		{
			tree[i].down2 = tree[j].down + edge[e].len;
		}
	}
}

void DFSu(int i)
{
	int e, j;

	j = tree[i].par;
	e = tree[i].back;
	if(j != -1)
	{
		tree[i].up = tree[j].up + edge[e].len;
		if(tree[j].sub == i)
		{
			tree[i].up = max(tree[i].up, tree[j].down2 + edge[e].len);
		}else{
			tree[i].up = max(tree[i].up, tree[j].down + edge[e].len);
		}
	}else{
		tree[i].up = 0;
	}
	for(e=tree[i].head;e!=-1;e=edge[e].next)
	{
		DFSu(edge[e].end);
	}
}

int main()
{
	int i;

	while(input())
	{
		DFSd(1);
		DFSu(1);
		for(i=1;i<=n;i++)
		{
			printf("%d\n",max(tree[i].down, tree[i].up));
		}
	}
	return 0;
}