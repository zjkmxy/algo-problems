//POJ1269
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<ctime>
using namespace std;

typedef double LDB;
#define eps 1e-12

struct POINT{
	LDB x, y;
}pts[4];
int n;

bool equ(LDB a, LDB b)
{
	return (fabs(a - b) < eps);
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

//差
POINT operator-(const POINT a, const POINT b)
{
	POINT ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	return ret;
}

//和
POINT operator+(const POINT a, const POINT b)
{
	POINT ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	return ret;
}

//数性积
LDB operator*(const POINT a, const POINT b)
{
	return (a.x * b.x + a.y * b.y);
}

//矢性积（仅长度）
LDB operator^(const POINT a, const POINT b)
{
	return (a.x * b.y - a.y * b.x);
}

int inters(POINT a, POINT b, POINT c, POINT d, POINT &x)
{
	LDB t, t1, t2, t3;
	t1 = (b-a)^(d-c);
	t2 = (b-a)^(c-a);
	t3 = (d-c)^(a-c);
	if(equ(t1,0))
	{
		if(equ(t2,0))
		{
			return -1;  //重叠
		}else{
			return 1;   //平行
		}
	}
	t = t3 / t1;
	x = a + scale(b - a, t);
	return 0;
}

int main()
{
	int i;
	POINT r;

	scanf("%d",&n);
	printf("INTERSECTING LINES OUTPUT\n");
	while(n--)
	{
		scanf("%lf%lf%lf%lf%lf%lf%lf%lf",&pts[0].x,&pts[0].y,&pts[1].x,&pts[1].y,&pts[2].x,&pts[2].y,&pts[3].x,&pts[3].y);
		switch(inters(pts[0], pts[1], pts[2], pts[3], r))
		{
		case 0:
			printf("POINT %.2lf %.2lf\n",r.x,r.y);
			break;
		case 1:
			printf("NONE\n");
			break;
		case -1:
			printf("LINE\n");
			break;
		}
	}
	printf("END OF OUTPUT\n");
	
	return 0;
}
