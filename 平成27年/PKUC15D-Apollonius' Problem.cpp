//求与三个圆同时相切的圆，暴力即可
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

struct CIR{
	double x, y, r;
};
CIR arr[3], ans;
double xp, xr, yp, yr;
bool flag;
const double eps = 1e-8;

double sqr(double x){
	return x * x;
}

inline bool dojudge(double x, double y, double r, double x1, double y1, double r1){
	double fq = max(r, r1);
	return abs(sqrt(sqr(x - x1) + sqr(y - y1)) - abs(r - r1)) <= 1e-6 * fq;
}

void judge(double r){
	double x, y;
	if(flag)
		return ;
	x = xr * r + xp;
	y = yr * r + yp;
	r = abs(r);
	if(!dojudge(arr[0].x, arr[0].y, arr[0].r, x, y, r))
		return;
	if(!dojudge(arr[1].x, arr[1].y, arr[1].r, x, y, r))
		return;
	if(!dojudge(arr[2].x, arr[2].y, arr[2].r, x, y, r))
		return;
	flag = true;
	ans.x = x;
	ans.y = y;
	ans.r = r;
}

//二次方程
void qe(double A, double B, double C){
	if(sqr(B) < 4 * A * C)
		return ;
	if(A < eps && A > -eps){
		judge(- C / B);
	}else{
		double base = - B / A / 2.;
		double dlt = sqrt(sqr(B) - 4 * A * C) / A / 2.;
		judge(base + dlt);
		judge(base - dlt);
	}
}

//问题转化
void solve(){
	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double A, B, C;

	flag = false;
	//做差
	a1 = 2 * (arr[0].x - arr[1].x);
	b1 = 2 * (arr[0].y - arr[1].y);
	c1 = - 2 * (arr[0].r - arr[1].r);
	d1 = (sqr(arr[0].x) + sqr(arr[0].y) - sqr(arr[0].r))
		-(sqr(arr[1].x) + sqr(arr[1].y) - sqr(arr[1].r));
	a2 = 2 * (arr[0].x - arr[2].x);
	b2 = 2 * (arr[0].y - arr[2].y);
	c2 = - 2 * (arr[0].r - arr[2].r);
	d2 = (sqr(arr[0].x) + sqr(arr[0].y) - sqr(arr[0].r))
		-(sqr(arr[2].x) + sqr(arr[2].y) - sqr(arr[2].r));
	//统一变量
	xp = (b2 * d1 - b1 * d2) / (a1 * b2 - a2 * b1);
	xr = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
	yp = (a1 * d2 - a2 * d1) / (a1 * b2 - a2 * b1);
	yr = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
	//整顿方程
	A = sqr(xr) + sqr(yr) - 1;
	B = (2 * (xp - arr[0].x) * xr + 2 * (yp - arr[0].y) * yr + 2 * arr[0].r);
	C = sqr(xp - arr[0].x) + sqr(yp - arr[0].y) - sqr(arr[0].r);

	qe(A, B, C);
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		scanf("%lf%lf%lf", &arr[0].x, &arr[0].y, &arr[0].r);
		scanf("%lf%lf%lf", &arr[1].x, &arr[1].y, &arr[1].r);
		scanf("%lf%lf%lf", &arr[2].x, &arr[2].y, &arr[2].r);
		solve();
		if(flag){
			printf("%.8lf %.8lf %.8lf\n", ans.x, ans.y, ans.r);
		}else{
			printf("no solution\n");
		}
	}
	return 0;
}
