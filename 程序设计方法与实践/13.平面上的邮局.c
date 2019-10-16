#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef long long int LLI;
typedef struct _DATA
{
	int x, y;
}DATA, *PDATA;
DATA arr[100005];
LLI xsum, ysum;
int n;

int compx(const void *A, const void *B)
{
	return ((PDATA)A)->x - ((PDATA)B)->x;
}

int compy(const void *A, const void *B)
{
	return ((PDATA)A)->y - ((PDATA)B)->y;
}

int main()
{
	int i, p;

	scanf("%d", &n);
	for(i = 0; i<n; i++)
	{
		scanf("%d%d", &arr[i].x, &arr[i].y);
	}
	qsort(arr, n, sizeof(DATA), compx);
	p = n / 2;
	xsum = 0;
	for(i = 0; i<n; i++)
	{
		xsum += labs(arr[i].x - arr[p].x);
	}
	qsort(arr, n, sizeof(DATA), compy);
	p = n / 2;
	ysum = 0;
	for(i = 0; i<n; i++)
	{
		ysum += labs(arr[i].y - arr[p].y);
	}
	printf("%lld\n", xsum + ysum);
	return 0;
}