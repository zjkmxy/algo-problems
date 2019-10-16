#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
using namespace std;

#define MAXN 20

struct DATA
{
	int deg;
	int lab;
}arr[MAXN];
typedef DATA *PDATA;

int n;
int mat[MAXN][MAXN];

int comp(const void *A, const void *B)
{
	return PDATA(B)->deg - PDATA(A)->deg;
}

bool domain()
{
	int i, j;

	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		arr[i].lab = i;
		scanf("%d",&arr[i].deg);
	}
	memset(mat, 0, sizeof(mat));
	qsort(arr, n, sizeof(DATA), comp);
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n && arr[i].deg>0;j++)
		{
			arr[j].deg--;
			arr[i].deg--;
			if(arr[j].deg < 0)
				return false;
			mat[arr[i].lab][arr[j].lab] = 1;
			mat[arr[j].lab][arr[i].lab] = 1;
		}
		if(arr[i].deg > 0)
			return false;
		qsort(arr+i+1, n-i-1, sizeof(DATA), comp);
	}
	return true;
}

int main()
{
	int t, i, j;

	scanf("%d",&t);
	while(t--)
	{
		if(!domain())
		{
			printf("NO\n");
			if(t>0)
				printf("\n");
			continue;
		}
		printf("YES\n");
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				printf(j==0?"%d":" %d",mat[i][j]);
			}
			printf("\n");
		}
		if(t>0)
			printf("\n");
	}
	return 0;
}