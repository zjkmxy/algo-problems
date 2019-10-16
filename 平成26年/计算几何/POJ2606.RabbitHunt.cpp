//同一条直线上最多有多少点
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<ctime>
#include<algorithm>
using namespace std;

typedef double LDB;
#define EPS 1e-12
#define PI  3.1415926535897932384626

struct POINT{
	LDB x, y;
}pts[300];
struct RATIO{
	LDB deg, r;
}rts[300];
int n;

bool equ(const LDB a, const LDB b)
{
	return (fabs(a - b) < EPS);
} 

POINT rot(const POINT v, const LDB deg)
{
	POINT ret;

	ret.x = v.x * cos(deg) - v.y * sin(deg);
	ret.y = v.x * sin(deg) + v.y * cos(deg);
	return ret;
}

LDB len(const POINT v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

POINT scale(const POINT v,const  LDB k)
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

void conv_r(const POINT p, RATIO &r)
{
	r.r = len(p);
	if(equ(p.x,0))
	{
		if(p.y > 0)
			r.deg = PI / 2.0;
		else
			r.deg = -PI / 2.0;
	}else if(equ(p.y,0))
	{
		if(p.x > 0)
			r.deg = 0;
		else
			r.deg = -PI;
	}else{
		r.deg = atan(p.y / p.x);
		if(p.x < 0)
		{
			if(r.deg > 0)
			{
				r.deg -= PI;
			}else{
				r.deg += PI;
			}
		}
	}
}

int comp(const void *A, const void *B)
{
	RATIO *a, *b;
	a = (RATIO*)A;
	b = (RATIO*)B;
	if(equ(a->deg, b->deg))
		return 0;
	else if(a->deg > b->deg)
		return 1;
	else
		return -1;
}

int calc()
{
	int ret = 0, i, cur; 

	cur = 0;
	for(i=2;i<n;i++)
	{
		if(equ(rts[i-1].deg,rts[i].deg))
		{
			cur++;
		}else{
			if(cur > ret)
				ret = cur;
			cur = 0;
		}
	}
	if(cur > ret)
		ret = cur;
	//这里不计算两端的情况，因为总有一个点使得其在一侧
	if(cur > ret)
		ret = cur;
	return (ret+1);   //返回相同个数
}

int main()
{
	int i, j;
	int ans = 0;

	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%lf%lf",&pts[i].x,&pts[i].y);
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<i;j++)
		{
			conv_r(pts[j] - pts[i], rts[j+1]);
		}
		for(j=i+1;j<n;j++)
		{
			conv_r(pts[j] - pts[i], rts[j]);
		}
		qsort(rts + 1, n - 1, sizeof(RATIO), comp);
		ans = max(ans, calc() + 1);
	}
	printf("%d\n",ans);
	return 0;
}
