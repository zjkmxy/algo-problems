#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1005
#define MAXI 0x3fffffff
#define MAXD 0.7976931348623158e+308
#define EPS 1e-6

struct _EDGE{
	int st, ed; //起点，终点
	double flow;    //
	double ratio;   //
	struct _EDGE *next; //下一条链
	struct _EDGE *revs; //逆向边（这里要求边必须成对出现）
};
typedef struct _EDGE EDGE;
typedef struct _EDGE *PEDGE;

struct _NODE{
	PEDGE prev;    //前导边
	double flow;   //到着流量
	double ratio;  //到着花费 
	bool  inol;    //是否在开表里
	PEDGE edge;
};
typedef struct _NODE NODE;
typedef struct _NODE *PNODE;

NODE point[MAXP];         //顶点表
int np;                   //顶点数

bool equ(double a, double b)
{
	return ((a <= b + EPS) && (a >= b - EPS));
}

bool Augment(int sp, int tp, double &ret)
{
	queue<int> oplst;
	int a;
	PEDGE eg;
	double aug_flow;

	for(a=0;a<np;a++)
	{
		point[a].prev = NULL;
		point[a].inol = false;
		point[a].flow = 0.0;
		point[a].ratio = 0.0;
	}
	oplst.push(sp);
	point[sp].inol = true;
	point[sp].flow = MAXD;
	point[sp].ratio = 1.0;
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		point[a].inol = false;
		for(eg=point[a].edge;eg!=NULL;eg=eg->next)
		{
			if(eg->flow <= EPS)
				continue;
			if(point[eg->ed].ratio > point[a].ratio * eg->ratio - EPS)
				continue;

			aug_flow = min(point[a].flow, eg->flow) * eg->ratio;
			if(equ(point[eg->ed].ratio, point[a].ratio * eg->ratio) && (point[eg->ed].flow >= aug_flow))
				continue;

			point[eg->ed].prev = eg;
			point[eg->ed].ratio = point[a].ratio * eg->ratio;
			point[eg->ed].flow = aug_flow;
			if(!point[eg->ed].inol)
			{
				point[eg->ed].inol = true;
				oplst.push(eg->ed);
			}
		}
	}
	if(point[tp].prev == NULL)
		return false;

	ret = point[tp].flow;
	eg = point[tp].prev;
	while(eg != NULL)
	{
		eg->revs->flow += ret;
		ret /= eg->ratio;
		eg->flow -= ret;
		eg = point[eg->st].prev;
	}
	return true;
}

void FFCalc(PEDGE ret)  //传入ret->st为源，ret->ed为汇，传出ret->flow为最大流，ret->ratio为最小代价
{
	double ad;

	if(ret == NULL)
		return ;
	ret->flow = 0;
	ret->ratio = 0;
	while(Augment(ret->st, ret->ed, ad))
	{
		ret->ratio += ad;
		ret->flow += ad * point[ret->ed].ratio;
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

void addedge(int a, int b, double f, double r)
{
	PEDGE tmp, tmp2;

	tmp  = (PEDGE)malloc(sizeof(EDGE));
	tmp2 = (PEDGE)malloc(sizeof(EDGE));
	tmp->st = a;
	tmp->ed = b;
	tmp->flow = f;
	tmp->ratio = r;
	tmp->revs = tmp2;
	tmp2->st = b;
	tmp2->ed = a;
	tmp2->flow = 0;
	tmp2->ratio = 1.0 / r;
	tmp2->revs = tmp;
	
	tmp->next = point[a].edge;
	point[a].edge = tmp;
	tmp2->next = point[b].edge;
	point[b].edge = tmp2;
}

bool input()
{
	int n, i, a, b;
	double f, r;

	if(scanf("%d%d",&np,&n)==EOF)
		return false;

	for(i=0;i<n;i++)
	{
		scanf("%d%d%lf%lf",&a,&b,&f,&r);
		a--;
		b--;
		addedge(a, b, f, r);
	}
	return true;
}

int main()
{
	EDGE ret;

	while(input())
	{
		ret.st = 0;
		ret.ed = np - 1;
		FFCalc(&ret);
		printf("%.2lf %.2lf\n",ret.flow,ret.ratio);
		freegp();
	}

	return 0;
}
