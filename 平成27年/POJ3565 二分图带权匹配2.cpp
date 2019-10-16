#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
#include <cmath>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#define MAXP 1005
#define MAXI 0x7fffffff
#define MAXC 0x0fffffff

struct _EDGE{
	int st, ed; //起点，终点
	int flow;
	double cost;
	struct _EDGE *next; //下一条链
	struct _EDGE *revs; //逆向边（这里要求边必须成对出现）
};
typedef struct _EDGE EDGE;
typedef struct _EDGE *PEDGE;

struct _NODE{
	PEDGE prev;    //前导边
	double   cost;    //最小到着代价
	bool  inol;    //是否在开表里
	PEDGE edge;
};
typedef struct _NODE NODE;
typedef struct _NODE *PNODE;

NODE point[MAXP];         //顶点表
int np;                   //顶点数

int Augment(int sp, int tp)
{
	queue<int> oplst;
	int ret, a;
	PEDGE eg;

	for(a = 0; a < np; a++)
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
		for(eg = point[a].edge; eg != NULL; eg = eg->next)
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

void FFCalc(PEDGE ret)  //传入ret->st为源，ret->ed为汇，传出ret->flow为最大流，ret->cost*ret->flow为最小代价（线性）
{
	int ad;

	if(ret == NULL)
		return;
	ret->flow = 0;
	ret->cost = 0;
	ad = Augment(ret->st, ret->ed);
	while(ad > 0)
	{
		ret->flow += ad;
		ret->cost += point[ret->ed].cost * ad;
		ad = Augment(ret->st, ret->ed);
	}
}

void freegp()
{
	int i;
	PEDGE p, tmp;
	for(i = 0; i < np; i++)
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

void addedge(int a, int b, int f, double c)
{
	PEDGE tmp, tmp2;

	tmp = (PEDGE)malloc(sizeof(EDGE));
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
//////////////////////////////////////////////////////////////////////////

int N;
pair<double, double> A[105], B[105];
void domain(){
	int i, j, ans;
	double dis;
	EDGE ret;

	scanf("%d", &N);
	np = N * 2 + 2;
	for(i = 1; i <= N; i ++){
		scanf("%lf %lf", &A[i].first, &A[i].second);
		addedge(0, i, 1, 0);
	}
	for(i = 1; i <= N; i ++){
		scanf("%lf %lf", &B[i].first, &B[i].second);
		addedge(i + N, N * 2 + 1, 1, 0);
	}
	for(i = 1; i <= N; i ++){
		for(j = 1; j <= N; j ++){
			dis = sqrt((A[i].first - B[j].first) * (A[i].first - B[j].first) + (A[i].second - B[j].second) * (A[i].second - B[j].second));
			addedge(i, j + N, 1, dis);
		}
	}
	ret.st = 0, ret.ed = N * 2 + 1;
	FFCalc(&ret);

	for(i = 1; i <= N; i ++){
		for(PEDGE j = point[i].edge; j; j = j->next){
			if(j->flow == 0){
				ans = j->ed - N;
				break;
			}
		}
		printf("%d\n", ans);
	}
}

int main(){
	domain();
	return 0;
}
