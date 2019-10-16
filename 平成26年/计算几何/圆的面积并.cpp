#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 1005
#define MAXM (MAXN*MAXN)
const double PI = 3.1415926535897932384626;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

struct NODE
{
	double x, y;
	int next;
	int vis;
}node[MAXM];
int N, cntp;

struct DTA
{
	int id;
	double ang;
	BOOL in;
};

struct CIRCLE
{
	double x, y, r;
	int pcnt;
	struct DTA cross[MAXN];
}cir[MAXN];

int comp(const void *A, const void *B)
{
	struct DTA *a, *b;
	a = (struct DTA *)A;
	b = (struct DTA *)B;
	if(a->ang < b->ang)
		return -1;
	else if(a->ang == b->ang)
		return 0;
	else
		return 1;
}

double sqdist(double x1, double y1, double x2, double y2)
{
	return ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}
double dist(double x1, double y1, double x2, double y2)
{
	return sqrt(sqdist(x1, y1, x2, y2));
}
int MIN(int a, int b)
{
	return (a < b ? a : b);
}

void dta_regular(struct DTA* ptr, int id, double ang, BOOL in)
{
	node[cntp].x = cir[id].x + cir[id].r * cos(ang);
	node[cntp].y = cir[id].y + cir[id].r * sin(ang);
	node[cntp].next = -1;
	node[cntp].vis = FALSE;
	ptr->id = cntp;
	ptr->in = in;
	while(ang < 0)
		ang += 2 * PI;
	while(ang > 2 * PI)
		ang -= 2 * PI;
	ptr->ang = ang;
	cir[id].pcnt++;
	cntp++;
}
void dta_set(struct DTA* ptr, int id, double ang, BOOL in, int pid)
{
	ptr->id = pid;
	ptr->in = in;
	while(ang < 0)
		ang += 2 * PI;
	while(ang > 2 * PI)
		ang -= 2 * PI;
	ptr->ang = ang;
	cir[id].pcnt++;
}

void cir_in(int id1, int id2)
{
	double a, b;
	a = sqdist(cir[id1].x, cir[id1].y, cir[id2].x, cir[id2].y);
	b = cir[id1].r - cir[id2].r;
	if(a > b * b)
		return ;
	if(b < 0)
	{
		cir[id1].pcnt = -1;
	}else{
		cir[id2].pcnt = -1;
	}
}

void input()
{
	int i, j;

	scanf("%d",&N);
	for(i=1;i<=N;i++)
	{
		scanf("%lf%lf%lf", &cir[i].x, &cir[i].y, &cir[i].r);
		cir[i].pcnt = 0;
	}
	for(i=1;i<=N;i++)
	{
		for(j=i+1;j<=N;j++)
		{
			cir_in(i, j);
		}
	}
	i = j = 1;
	while(j <= N)
	{
		if(cir[j].pcnt == 0)
		{
			cir[i].x = cir[j].x;
			cir[i].y = cir[j].y;
			cir[i].r = cir[j].r;
			cir[i].pcnt = cir[j].pcnt;
			i++;
		}
		j++;
	}
	N = i - 1;
	cntp = 0;
}

void cross(int id1, int id2)
{
	double d = dist(cir[id1].x, cir[id1].y, cir[id2].x, cir[id2].y);
	double a1, pa1, pa2, a2;

	if(d >= cir[id1].r + cir[id2].r)
		return ;

	pa1 = acos((cir[id1].r*cir[id1].r + d*d - cir[id2].r*cir[id2].r) / 2.0 / cir[id1].r / d);
	a1 = atan2(cir[id2].y-cir[id1].y, cir[id2].x-cir[id1].x);
	dta_regular(&cir[id1].cross[cir[id1].pcnt], id1, a1 + pa1, FALSE);
	dta_regular(&cir[id1].cross[cir[id1].pcnt], id1, a1 - pa1, TRUE);

	pa2 = acos((cir[id2].r*cir[id2].r + d*d - cir[id1].r*cir[id1].r) / 2.0 / cir[id2].r / d);
	a2 = atan2(cir[id1].y-cir[id2].y, cir[id1].x-cir[id2].x);
	dta_set(&cir[id2].cross[cir[id2].pcnt], id2, a2 + pa2, FALSE, cntp-1);
	dta_set(&cir[id2].cross[cir[id2].pcnt], id2, a2 - pa2, TRUE, cntp-2);
}

double work()
{
	int i, j, crs, mcrs, ida, idb;
	double ret = 0.0, ar, b;

	//计算全部交点
	for(i=1;i<=N;i++)
	{
		for(j=i+1;j<=N;j++)
		{
			cross(i, j);
		}
		qsort(cir[i].cross, cir[i].pcnt, sizeof(struct DTA), comp);
	}
	//计算覆盖弧
	for(i=1;i<=N;i++)
	{
		mcrs = crs = 0;
		if(cir[i].pcnt == 0)
		{
			ret += PI * cir[i].r * cir[i].r;
			continue;
		}
		for(j=0;j<cir[i].pcnt;j++)
		{
			if(cir[i].cross[j].in)
			{
				crs++;
			}else{
				crs--;
			}
			mcrs = MIN(mcrs, crs);
		}
		if(crs != 0)
			return *(double*)NULL;
		for(j=0;j<cir[i].pcnt;j++)
		{
			if(cir[i].cross[j].in)
			{
				crs++;
			}else{
				crs--;
			}
			if(crs == mcrs)
			{
				ida = cir[i].cross[j].id;
				idb = cir[i].cross[(j==cir[i].pcnt-1)?0:(j+1)].id;
				node[ida].next = idb;
				ar = cir[i].cross[(j==cir[i].pcnt-1)?0:(j+1)].ang - cir[i].cross[j].ang;
				while(ar < 0)
					ar += 2 * PI;
				while(ar > 2 * PI)
					ar -= 2 * PI;
				ret += cir[i].r * cir[i].r * (ar - sin(ar)) / 2.0;
			}
		}
	}
	//计算重叠多边形
	for(i=0;i<cntp;i++)
	{
		if(node[i].vis)
			continue;
		j = i;
		do{
			node[j].vis = TRUE;
			crs = node[j].next;
			if(crs == -1)
				break;
			ret += (node[j].x * node[crs].y - node[crs].x * node[j].y) / 2.0;
			j = crs;
		}while(j != i);
	}
	return ret;
}

int main()
{
	input();
	printf("%.3lf\n",work());
	return 0;
}
