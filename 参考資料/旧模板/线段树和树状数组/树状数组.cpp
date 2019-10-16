#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define lowbit(x) (x&(-x))

int scan[10000];
int n;

int getsum(int i)
{
	int sum = 0;
	for(;i>0;i-=lowbit(i))
		sum += tarr[i];
	return sum;
}

void update(int i, int val)
{
	for(;i<=n;i+=lowbit(i))
		tarr[i] += val;
}
