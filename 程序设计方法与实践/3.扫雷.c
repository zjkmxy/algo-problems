#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 101
int n, m;

char map[MAXN][MAXN];
char buf[MAXN];
int dirs[8][2] = 
{
	{ 1, 0},
	{-1, 0},
	{ 0,  1},
	{ 0, -1},
	{ 1,  1},
	{-1,  1},
	{ 1, -1},
	{-1, -1},
};

void domain()
{
	int i, j, p;

	for(i=0;i<n;i++)
	{
		scanf("%s", buf);
		for(j=0;j<m;j++)
		{
			map[i][j] = buf[j];
			if(map[i][j] == '.')
				map[i][j] = '0';
		}
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(map[i][j] == '*')
			{
				for(p=0;p<8;p++)
				{
					if(i+dirs[p][0] < 0 || i+dirs[p][0] >= n || j+dirs[p][1] < 0 || j+dirs[p][1] >= m)
						continue;
					if(map[i+dirs[p][0]][j+dirs[p][1]] == '*')
						continue;
					map[i+dirs[p][0]][j+dirs[p][1]]++;
				}
			}
		}
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	int t = 0;

	while(scanf("%d %d", &n, &m) != EOF)
	{
		if(n == 0)
			break;
		t++;
		if(t > 1)
			printf("\n");
		printf("Field #%d:\n", t);
		domain();
	}
	return 0;
}