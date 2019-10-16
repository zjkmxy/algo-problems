#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int n, md;
typedef struct _DTA
{
	int d, v;
}DTA, *PDTA;
DTA arr[10005];
int tmp[10005];

int comp(const void*A, const void*B)
{
	if(((PDTA)A)->v != ((PDTA)B)->v)
		return ((PDTA)B)->v - ((PDTA)A)->v;
	else
		return ((PDTA)B)->d - ((PDTA)A)->d;
}

int main()
{
	int i, ret = 0, p;

	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i].v);
	}
	md = 0;
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i].d);
		md = md<arr[i].d ? arr[i].d : md;
	}
	qsort(arr, n, sizeof(DTA), comp);
	memset(tmp, 0, sizeof(tmp));
	for(i=0;i<n;i++)
	{
		for(p=arr[i].d;p>=1;p--)
		{
			if(tmp[p] == 0)
			{
				tmp[p] = 1;
				ret += arr[i].v;
				break;
			}
		}
	}
	
	printf("%d\n",ret);
	return 0;
}