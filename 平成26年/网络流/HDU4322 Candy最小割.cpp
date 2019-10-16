/*
hdu 4322 candy
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1005
#define MAXI 0x7fffffff
#define MAXC 0x0fffffff

struct _EDGE{
	int st, ed; //起点，终点
	int flow;
	int cost;
	struct _EDGE *next; //下一条链
	struct _EDGE *revs; //逆向边（这里要求边必须成对出现）
};
typedef struct _EDGE EDGE;
typedef struct _EDGE *PEDGE;

struct _NODE{
	PEDGE prev;    //前导边
	int   cost;    //最小到着代价
	bool  inol;    //是否在开表里
	PEDGE edge;
};
typedef struct _NODE NODE;
typedef struct _NODE *PNODE;

NODE point[MAXP];         //顶点表
int np;                   //顶点数

int  k, n, m;
int  boy[MAXP];
bool like[MAXP][MAXP];

int Augment(int sp, int tp)
{
	queue<int> oplst;
	int ret, a;
	PEDGE eg;

	for(a=0;a<np;a++)
	{
		point[a].prev = NULL;
		point[a].inol = false;
		point[a].cost = MAXC;
	}
	oplst.push(sp);
	point[sp].inol = true;
	point[sp].cost = 0;
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		point[a].inol = false;
		for(eg=point[a].edge;eg!=NULL;eg=eg->next)
		{
			if(eg->flow <= 0)
				continue;
			if(point[eg->ed].cost <= point[a].cost + eg->cost)
				continue;

			point[eg->ed].prev = eg;
			point[eg->ed].cost = point[a].cost + eg->cost;
			if(!point[eg->ed].inol)
			{
				point[eg->ed].inol = true;
				oplst.push(eg->ed);
			}
		}
	}
	if(point[tp].prev == NULL)
		return 0;

	ret = MAXI;
	eg = point[tp].prev;
	while(eg != NULL)
	{
		ret = min(ret, eg->flow);
		eg = point[eg->st].prev;
	}
	eg = point[tp].prev;
	while(eg != NULL)
	{
		eg->flow -= ret;
		eg->revs->flow += ret;
		eg = point[eg->st].prev;
	}
	return ret;
}

void FFCalc(PEDGE ret)  //传入ret->st为源，ret->ed为汇，传出ret->flow为最大流，ret->cost为最小代价
{
	int ad;

	if(ret == NULL)
		return ;
	ret->flow = 0;
	ret->cost = 0;
	ad = Augment(ret->st, ret->ed);
	while(ad > 0)
	{
		ret->flow += ad;
		ret->cost += point[ret->ed].cost;
		ad = Augment(ret->st, ret->ed);
	}
}

void freegp()
{
	int i;
	PEDGE p,tmp;
	for(i=0;i<np;i++)
	{
		p = point[i].edge;
		while(p != NULL)
		{
			tmp = p->next;
			free(p);
			p = tmp;
		}
		point[i].edge = NULL;
	}
}

void addedge(int a, int b, int f, int c)
{
	PEDGE tmp, tmp2;

	tmp  = (PEDGE)malloc(sizeof(EDGE));
	tmp2 = (PEDGE)malloc(sizeof(EDGE));
	tmp->st = a;
	tmp->ed = b;
	tmp->flow = f;
	tmp->cost = c;
	tmp->revs = tmp2;
	tmp2->st = b;
	tmp2->ed = a;
	tmp2->flow = 0;
	tmp2->cost = -c;
	tmp2->revs = tmp;
	
	tmp->next = point[a].edge;
	point[a].edge = tmp;
	tmp2->next = point[b].edge;
	point[b].edge = tmp2;
}

void input()
{
	int i, j, tmp;

	scanf("%d%d%d",&n,&m,&k);
	for(i=1;i<=m;i++)
	{
		scanf("%d",&boy[i]);
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%d",&like[i][j]);
		}
	}
}

void calc()
{
	int i, j, sum, left;
	EDGE ret;

	np = m + n + 2;
	ret.st = 0;
	ret.ed = m + n + 1;
	for(j=1;j<=n;j++)
	{
		addedge(0, j, 1, 0);
	}
	sum = 0;
	for(i=1;i<=m;i++)
	{
		sum += boy[i];
		addedge(n+i, ret.ed, boy[i] / k, 0);
		if(boy[i] % k > 1)
			addedge(n+i, ret.ed, 1, k - boy[i] % k);
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(like[i][j])
				addedge(j, n+i, 1, 0);
		}
	}
	FFCalc(&ret);
	left = n - ret.flow;
	sum -= ret.flow * k - ret.cost;
	if(left >= sum)
		printf("YES\n");
	else
		printf("NO\n");
	freegp();
}

int main()
{
	int t, i;

	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		input();
		printf("Case #%d: ",i);
		calc();
	}

	return 0;
}