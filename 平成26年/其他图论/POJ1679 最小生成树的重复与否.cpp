#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;

#define MAXN 5005

int map[MAXN][MAXN];
int P, Q;
struct EDGE{
	int a, b, w;
	bool dob;
}tree[MAXN];

long Prim()
{
	long i,j,m,v;
	long min,wealth = 0;
	EDGE tmp;

	for(i=1;i<P;i++)          //构建临时树
	{
		tree[i-1].a = 0;
		tree[i-1].b = i;
		tree[i-1].w = map[0][i];
		tree[i-1].dob = false;
	}
	for(i=1;i<P;i++)
	{
		min = tree[i-1].w;
		m = i;
		for(j=i+1;j<P;j++)     //寻找最小边
		{
			if(tree[j-1].w < min)
			{
				min = tree[j-1].w;
				m = j;
			}
		}

		if(tree[m-1].dob)
			return -1;
		v = tree[m-1].b;       //存入目标树中
		wealth += tree[m-1].w;
		tmp = tree[m-1];
		tree[m-1] = tree[i-1];
		tree[i-1] = tmp;

		for(j=i+1;j<P;j++)     //更新临时树
		{
			min = map[v][tree[j-1].b];
			if(min < tree[j-1].w)
			{
				tree[j-1].w = min;
				tree[j-1].a = v;
				tree[j-1].dob = false;
			} else if(min == tree[j-1].w)
			{
				tree[j-1].dob = true;
			}
		}
	}

	return wealth;
}

void input()
{
	int i, a, b, w;

	scanf("%d%d",&P,&Q);
	for(i=0;i<P;i++)
	{
		for(a=0;a<P;a++)
		{
			map[i][a] = 0x3fffffff;
		}
	}
	for(i=0;i<Q;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		a--;
		b--;
		map[a][b] = w;
		map[b][a] = w;
	}
}

int main()
{
	int t, ret;

	scanf("%d",&t);
	while(t--)
	{
		input();
		ret = Prim();
		if(ret < 0)
			printf("Not Unique!\n");
		else
			printf("%d\n", ret);
	}
	return 0;
}
