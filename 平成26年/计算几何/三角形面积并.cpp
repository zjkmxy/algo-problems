#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 1005
#define MAXM (MAXN*MAXN)
const double PI = 3.1415926535897932384626;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

typedef struct _POINT
{
	double x, y;
}POINT, *PPOINT;
typedef struct _TRI
{
	int id1, id2, id3;
}TRI, *PTRI;
typedef struct _EVENT
{
	double y;
	BOOL in;
}EVENT, *PEVENT;

POINT pnts[MAXM];
TRI tri[MAXN];
EVENT eve[MAXN*2];
double xs[MAXM];
int npnt, N, neve, nxs;

int comp(const void *A, const void *B)
{
	double *a = (double*)A, *b = (double*)B;
	if(*a < *b)
		return -1;
	else if(*a == *b)
		return 0;
	else
		return 1;
}

int comp2(const void *A, const void *B)
{
	PEVENT a = (PEVENT)A, b = (PEVENT)B;
	if(a->y < b->y)
		return -1;
	else if(a->y == b->y)
		return (b->in - a->in);
	else
		return 1;
}

BOOL insec(int a, int b, int c, int d)
{
	double aabc, aabd, acda, t;

	aabc = (pnts[a].x - pnts[c].x) * (pnts[b].y - pnts[c].y) - (pnts[a].y - pnts[c].y) * (pnts[b].x - pnts[c].x);  
	aabd = (pnts[a].x - pnts[d].x) * (pnts[b].y - pnts[d].y) - (pnts[a].y - pnts[d].y) * (pnts[b].x - pnts[d].x); 
	if(aabc * aabd >= 0.0)
		return FALSE;
	acda = (pnts[c].x - pnts[a].x) * (pnts[d].y - pnts[a].y) - (pnts[c].y - pnts[a].y) * (pnts[d].x - pnts[a].x);
	if(acda * (acda + aabc - aabd) >= 0.0)
		return FALSE;
	t = acda / ( aabd- aabc );
	pnts[npnt].x = pnts[a].x + t * (pnts[b].x - pnts[a].x);
	pnts[npnt].y = pnts[a].y + t * (pnts[b].y - pnts[a].y);
	return TRUE;
}

void deal_tri()
{
	int i, j;
	for(i=0;i<N;i++)
	{
		for(j=i+1;j<N;j++)
		{
			if(insec(tri[i].id1, tri[i].id2, tri[j].id1, tri[j].id2)) npnt++;
			if(insec(tri[i].id2, tri[i].id3, tri[j].id1, tri[j].id2)) npnt++;
			if(insec(tri[i].id3, tri[i].id1, tri[j].id1, tri[j].id2)) npnt++;;

			if(insec(tri[i].id1, tri[i].id2, tri[j].id2, tri[j].id3)) npnt++;
			if(insec(tri[i].id2, tri[i].id3, tri[j].id2, tri[j].id3)) npnt++;
			if(insec(tri[i].id3, tri[i].id1, tri[j].id2, tri[j].id3)) npnt++;

			if(insec(tri[i].id1, tri[i].id2, tri[j].id3, tri[j].id1)) npnt++;
			if(insec(tri[i].id2, tri[i].id3, tri[j].id3, tri[j].id1)) npnt++;
			if(insec(tri[i].id3, tri[i].id1, tri[j].id3, tri[j].id1)) npnt++;
		}
	}
	for(i=0;i<npnt;i++)
	{
		xs[i] = pnts[i].x;
	}
	qsort(xs, npnt, sizeof(double), comp);
	i = j = 0;
	while(j < npnt)
	{
		xs[i] = xs[j];
		do{
			j++;
		}while(xs[i] == xs[j] && j < npnt);
		i++;
	}
	nxs = i;
}

BOOL cuty(int a, int b, double x, double *ret)
{
	double k;
	if(pnts[a].x > pnts[b].x)
	{
		a = a + b;
		b = a - b;
		a = a - b;
	}
	if(x < pnts[a].x || x > pnts[b].x || pnts[a].x == pnts[b].x)
		return FALSE;
	k = (pnts[b].y - pnts[a].y) / (pnts[b].x - pnts[a].x);
	*ret = k * (x - pnts[a].x) + pnts[a].y;
	return TRUE;
}

void split(int id, double x, BOOL in)
{
	double tmp, y[3], ppx;
	int flg = 0;

	flg += cuty(tri[id].id1, tri[id].id2, x, &y[flg]);
	flg += cuty(tri[id].id2, tri[id].id3, x, &y[flg]);
	flg += cuty(tri[id].id3, tri[id].id1, x, &y[flg]);
	if(flg == 0)
		return ;
	if(flg == 3 && y[0] == y[1])
	{
		tmp = y[0];
		y[0] = y[2];
		y[2] = tmp;
	}

	flg = 0;
	if(pnts[tri[id].id1].x == x)
	{
		flg++;
	}else ppx = pnts[tri[id].id1].x;
	if(pnts[tri[id].id2].x == x)
	{
		flg++;
	}else ppx = pnts[tri[id].id2].x;
	if(pnts[tri[id].id3].x == x)
	{
		flg++;
	}else ppx = pnts[tri[id].id3].x;
	if(flg >= 2)
	{
		if((ppx < x && in) || (ppx > x) && !in)
			return ;
	}
	if(y[0] > y[1])
	{
		tmp = y[1];
		y[1] = y[0];
		y[0] = tmp;
	}

	eve[neve].y = y[0];
	eve[neve].in = TRUE;
	neve++;
	eve[neve].y = y[1];
	eve[neve].in = FALSE;
	neve++;
}

double calc(int p, BOOL in)
{
	int i, cls;
	double ret = 0.0;

	neve = 0;
	for(i=0;i<N;i++)
	{
		split(i, xs[p], in);
	}
	if(neve == 0)
		return ret;
	qsort(eve, neve, sizeof(EVENT), comp2);
	cls = 1;
	for(i=1;i<neve;i++)
	{
		if(cls > 0)
			ret += eve[i].y - eve[i-1].y;
		if(eve[i].in)
			cls++;
		else
			cls--;
	}
	if(cls != 0)
		cls = *(int*)NULL;
	return ret;
}

double calc_area()
{
	int i;
	double pf, cur, ret = 0.0;

	deal_tri();
	for(i=1;i<nxs;i++)
	{
		if(xs[i] == 5)
			i = i;
		pf = calc(i-1, TRUE);
		cur = calc(i, FALSE);
		ret += (cur + pf) * (xs[i] - xs[i-1]) / 2.0;
		//printf("%.3lf + %.3lf * (%.3lf - %.3lf)\n",pf,cur,xs[i],xs[i-1]);
	}
	return ret;
}

void input()
{
	int i;

	npnt = 0;
	scanf("%d",&N);
	for(i=0;i<N;i++)
	{
		scanf("%lf%lf",&pnts[npnt].x,&pnts[npnt].y);
		tri[i].id1 = npnt;
		npnt++;
		scanf("%lf%lf",&pnts[npnt].x,&pnts[npnt].y);
		tri[i].id2 = npnt;
		npnt++;
		scanf("%lf%lf",&pnts[npnt].x,&pnts[npnt].y);
		tri[i].id3 = npnt;
		npnt++;
	}
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		input();
		printf("%.2lf\n",calc_area());
	}
	return 0;
}
