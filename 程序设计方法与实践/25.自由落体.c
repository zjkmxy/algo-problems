#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define eps 0
double H, S1, V, L, K;
int N, r, l;
double p1, p2;

int calc()
{
	double t, s;

	t = sqrt(0.2 * H);
	s = V * t;
	p1 = S1 - 0.00001 - s;
	t = sqrt(2.0 * (H - K - 0.00001) / 10.0);
	if(H - K <= 0)
		t = 0;
	s = V * t;
	p2 = S1 + L + 0.00001 - s;

	if(p2 > N - 1)
		r = N - 1;
	else
		r = floor(p2 + eps);
	if(p1 < 0)
		l = 0;
	else
		l = ceil(p1 - eps);

	return (r - l + 1);
}

int main()
{
	while(scanf("%lf%lf%lf%lf%lf%d",&H,&S1,&V,&L,&K,&N)!=EOF)
	{
		printf("%d\n",calc());
	}
	return 0;
}