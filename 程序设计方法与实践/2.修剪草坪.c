#include <stdio.h>
#include <stdlib.h>

#define MAXN 101
#define MIN(a, b)	((a)<(b)?(a):(b))
#define MAX(a, b)	((a)>(b)?(a):(b))
int n, m;
int row[MAXN], col[MAXN];
int map[MAXN][MAXN];

int domain()
{
	int i, j;

	for(i=0;i<n;i++)
	{
		row[i] = 0;
		for(j=0;j<m;j++)
		{
			row[i] = MAX(row[i], map[i][j]);
		}
	}
	for(j=0;j<m;j++)
	{
		col[j] = 0;
		for(i=0;i<n;i++)
		{
			col[j] = MAX(col[j], map[i][j]);
		}
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(map[i][j] != MIN(row[i], col[j]))
				return 0;
		}
	}
	return 1;
}

int main()
{
	int i, j;
	scanf("%d%d", &n, &m);
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			scanf("%d", &map[i][j]);
		}
	}
	if(domain())
	{
		printf("YES\n");
	}else{
		printf("NO\n");
	}
	return 0;
}