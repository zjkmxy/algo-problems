/*
HDU1524 A Chess Game

SG÷µºÚµ•º∆À„°≠°≠
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXN 1005

int edge_cnt[MAXN];
int edges[MAXN][MAXN];
int sg[MAXN];
int n;

int calc_sg(int i)
{
	bool visit[MAXN];
	int j;

	if(sg[i] > -1)
		return sg[i];

	if(edge_cnt[i] == 0)
	{
		sg[i] = 0;
		return 0;
	}

	memset(visit, 0, sizeof(visit));
	for(j=0;j<edge_cnt[i];j++)
	{
		if(sg[edges[i][j]] == -1)
			calc_sg(edges[i][j]);
		visit[sg[edges[i][j]]] = true;
	}
	for(j=0;j<MAXN;j++)
	{
		if(!visit[j])
			break;
	}
	sg[i] = j;
	return j;
}

bool domain()
{
	int m, sum, i, a;

	scanf("%d",&m);
	if(m == 0)
		return false;

	sum = 0;
	for(i=0;i<m;i++)
	{
		scanf("%d",&a);
		calc_sg(a);
		sum ^= sg[a];
	}
	if(sum != 0)
	{
		printf("WIN\n");
	}else{
		printf("LOSE\n");
	}
	return true;
}

bool input()
{
	int i, j;

	memset(sg, -1, sizeof(sg));
	if(scanf("%d",&n)==EOF)
		return false;

	for(i=0;i<n;i++)
	{
		scanf("%d",&edge_cnt[i]);
		for(j=0;j<edge_cnt[i];j++)
		{
			scanf("%d",&edges[i][j]);
		}
	}
	return true;
}

int main()
{
	while(input())
	{
		while(domain());
	}
	return 0;
}
