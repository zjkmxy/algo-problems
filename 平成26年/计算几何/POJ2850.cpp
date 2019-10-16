/*
POJ2850 

n个单位长度的油桶摆在第一排，每一层减少一个挨个排，问最顶上的油桶坐标。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<ctime>
using namespace std;

typedef double LDB;
#define eps 1e-12

struct POINT{
	LDB x, y;
}pts[20];
int n;

bool equ(LDB a, LDB b)
{
	return (fabs(a - b) > eps);
} 

POINT rot(POINT v, LDB deg)
{
	POINT ret;

	ret.x = v.x * cos(deg) - v.y * sin(deg);
	ret.y = v.x * sin(deg) + v.y * cos(deg);
	return ret;
}

LDB len(POINT v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

POINT scale(POINT v, LDB k)
{
	POINT ret;

	ret.x = v.x * k;
	ret.y = v.y * k;
	return ret;
}

POINT operator-(const POINT a, const POINT b)
{
	POINT ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	return ret;
}

POINT operator+(const POINT a, const POINT b)
{
	POINT ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	return ret;
}

int comp(const void *A, const void *B)
{
	POINT *a, *b;
	a = (POINT*)A;
	b = (POINT*)B;
	return (a->x - b->x);
}

void calc(int n)
{
	int i, j;
	POINT vec;

	for(i=1;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			vec = pts[j+1] - pts[j];
			vec = rot(vec, acos(len(vec) / 4.0));
			vec = scale(vec, 2.0 / len(vec));
			pts[j] = pts[j] + vec;
		}
	}
}

int main()
{
	int i;

	while(scanf("%d",&n)!=EOF)
	{
		if(n==0)
			break;
		for(i=0;i<n;i++)
		{
			scanf("%lf",&pts[i].x);
			pts[i].y = 1.0;
		}
		qsort(pts, n, sizeof(POINT), comp);
		calc(n);
		printf("%.4lf %.4lf\n",pts[0].x, pts[0].y);
	}
	return 0;
}
