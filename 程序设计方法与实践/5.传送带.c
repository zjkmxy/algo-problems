#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int len[101];
int x, r, w;
double t;

double calc()
{
	int v;
	double ret = 0.0, tmp;

	len[0] = x;
	for(v=1;v<=100;v++)
	{
		len[0] -= len[v];
	}
	for(v=0;v<=100;v++)
	{
		tmp = (double)len[v] / (double)(r + v);
		if(tmp <= t)
		{
			t -= tmp;
			ret += tmp;
		}else{
			tmp = len[v] - t * (r + v);
			ret += t + tmp / (double)(w + v);
			t = 0;
		}
	}
	return ret;
}

void input()
{
	int n, a, b, v;
	scanf("%d%d%d%lf%d",&x,&w,&r,&t,&n);
	memset(len, 0, sizeof(len));
	while(n--)
	{
		scanf("%d%d%d",&a,&b,&v);
		len[v] += b-a;
	}
}

int main()
{
	int cas;
	scanf("%d",&cas);
	while(cas--)
	{
		input();
		printf("%.6lf\n",calc());
	}
	return 0;
}