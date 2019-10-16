#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define eps 1e-11
const double PI = 3.1415926535897932384626;

double x, y, v, r;
double ang, rad, rv;

//坐标系转化
void calc_pos(double x, double y, double v)
{
	ang = atan2(y, x);
	rad = sqrt(x * x + y * y);
	rv = v / rad;
}

//跨立实验
int seg_cir(double x0, double y0, double x1, double y1, double r)
{
	double t, xm, ym;
	double dx, dy;

	dy = y1 - y0;
	dx = x1 - x0;
	t = (y0 * dx - x0 * dy) / (dx * dx + dy * dy);
	xm = -dy * t;
	ym = dx * t;
	x1 -= xm;
	x0 -= xm;
	return (x1 * x0 < 0);
}

int judge(double t)
{
	double xp, yp;
	double dx, dy, dis, dis2;
	double td1, td2;
	double ta1, ta2, ta;

	xp = rad * cos(ang + rv * t);
	yp = rad * sin(ang + rv * t);
	dy = yp - y;
	dx = xp - x;
	dis2 = sqrt(dx * dx + dy * dy);
	dis = fabs(dx * y - dy * x) / dis2;
	if(dis < r && seg_cir(xp, yp, x, y, r))
	{
		td1 = sqrt(x * x + y * y - r * r);
		td2 = sqrt(rad * rad - r * r);
		ta1 = atan(td1 / r);
		ta2 = atan(td2 / r);
		ta = atan2(yp, xp) - atan2(y, x);
		while(ta >= 2 * PI)
			ta -= 2 * PI;
		while(ta < 0)
			ta += 2 * PI;
		if(ta > PI)
			ta = 2 * PI - ta;
		ta -= ta1 + ta2;
		if(ta < -1e-4)
			return *(int*)NULL;
		dis2 = td1 + td2 + r * ta;
	}
	return (dis2 / v) <= t;
}

double bins()
{
	double l, r, mid;

	l = 0.0;
	r = 10000000.0;
	while(r - l >= eps)
	{
		mid = (l + r) / 2.0;
		if(judge(mid))
			r = mid;
		else
			l = mid;
	}
	return mid;
}

int main()
{
	scanf("%lf%lf%lf",&x,&y,&v);
	calc_pos(x, y, v);
	scanf("%lf%lf%lf%lf",&x,&y,&v,&r);
	judge(9.584544);
	printf("%.6lf\n",bins());
	return 0;
}