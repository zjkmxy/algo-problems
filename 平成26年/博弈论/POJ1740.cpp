/*
POJ1740 A New Stone Game
很简单，一开始如果是两两相等那么先手必输（P）。
所以一开始如果是奇数那么先手赢（N），偶数的话要看能不能补齐成两两相等。
推导发现除了一开始两两相等的，一定可以在操作后变成两两相等的。
于是就简单了。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int arr[12];
int n;

int comp(const void *A, const void *B)
{
	int *a, *b;

	a = (int*)A;
	b = (int*)B;
	return (*a - *b);
}

bool calc()
{
	int i, sum = 0;

	if(n % 2 == 1)
		return true;
	qsort(arr, n, sizeof(int), comp);
	
	for(i=1;i<n-1;i++)
	{
		if(i % 2 == 1)
			sum -= arr[i];
		else
			sum += arr[i];
	}
	if(sum < arr[n-1] - arr[0])
		return true;
	else
		return false;
}

int main()
{
	int i;

	while(true)
	{
		memset(arr, 0, sizeof(arr));
		scanf("%d",&n);
		if(n == 0)
			return 0;
		for(i=0;i<n;i++)
		{
			scanf("%d",&arr[i]);
			if(arr[i]==0)
			{
				i--;
				n--;
			}
		}
		if(calc())
			printf("1\n");
		else
			printf("0\n");
	}
	return 0;
}