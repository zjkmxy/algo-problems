/*
HDU1760  A New Tetris Game

因为0的个数太少了，所以直接DFS状态即可。
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

#define MAXN 55
int map[MAXN][MAXN];
int n, m;

bool is_p()
{
	int i, j;
	bool ret;
	for(i=1;i<n;i++)
	{
		for(j=1;j<m;j++)
		{
			if(map[i-1][j-1] + map[i-1][j] + map[i][j-1] + map[i][j] > 0)
				continue;

			map[i-1][j-1] = map[i-1][j] = map[i][j-1] = map[i][j] = 1;
			ret = is_p();
			map[i-1][j-1] = map[i-1][j] = map[i][j-1] = map[i][j] = 0;
			if(ret)
				return false;
		}
	}
	return true;
}

int main()
{
	int i, j;

	while(scanf("%d%d",&n,&m)!=EOF)
	{
		for(i=0;i<n;i++)
		{
			for(j=0;j<m;j++)
			{
				scanf("%1d",&map[i][j]);
			}
		}
		if(is_p())
			printf("No\n");
		else
			printf("Yes\n");
	}
	return 0;
}
