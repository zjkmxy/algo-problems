#include <iostream>
#include <cmath>
#include <cstdio>
#include<cstring>
#include<algorithm>
#include<cstdlib>
#include<vector>
#include<cmath>
using namespace std;

double x0, z0, v;
const double g = 9.8;
const double PI = 3.1415926535897932384626;
const double eps = 1e-10;

double calc(double ang)
{
	double y = x0 * tan(ang) - g * x0 * x0 / 2 / (v * v) / (cos(ang) * cos(ang));
	return fabs(y - z0);
}

double TriSearch()
{
	double st = 0.0, ed = PI / 2, mid, mid2;

	while(ed - st > eps)
	{
		mid = (ed - st) / 3.0;
		mid2 = st + mid * 2;
		mid += st;
		if(calc(mid) > calc(mid2))
			st = mid;
		else
			ed = mid2;
	}
	return st;
}

int main()
{
	int t;
	double ret;

	scanf("%d",&t);
	while(t--)
	{
		scanf("%lf%lf%lf",&x0,&z0,&v);
		ret = TriSearch();
		if(calc(ret) < 1e-7)
			printf("%lf\n",ret);
		else
			printf("-1\n");
	}
	return 0;
}
