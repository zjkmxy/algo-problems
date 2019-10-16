//HDU1892
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <algorithm>
using namespace std;

#define MAXN 1005
#define MAXM 1004
#define lowbit(x) (x&(-x))

int block[MAXN][MAXN], arr[MAXN][MAXN];  //所有读入的x,y均+1处理

int getsum(int x, int y)
{
	int sum = 0, i, j;
	for(i=x;i>0;i-=lowbit(i))
	{
		for(j=y;j>0;j-=lowbit(j))
		{
			sum += arr[i][j];
		}
	}
	return sum;
}

void update(int x, int y, int val)
{
	int i, j;
	for(i=x;i<=MAXM;i+=lowbit(i))
	{
		for(j=y;j<=MAXM;j+=lowbit(j))
		{
			arr[i][j] += val;
		}
	}
}

void initall()
{
	int i, j;
	for(i=1;i<=MAXM;i++)
	{
		for(j=1;j<=MAXM;j++)
		{
			block[i][j] = 1;
			arr[i][j] = lowbit(i) * lowbit(j);
		}
	}
}

void domain()
{
	char op[3];
	int x1, y1, x2, y2, n, m, i;

	initall();
	scanf("%d",&m);
	for(i=0;i<m;i++)
	{
		scanf("%s",op);
		switch(op[0])
		{
		case 'S':
			scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
			if(x1 > x2)
				swap(x1, x2);
			if(y1 > y2)
				swap(y1, y2);
			x2++;
			y2++;
			printf("%d\n",getsum(x2, y2) - getsum(x2, y1) - getsum(x1, y2) + getsum(x1, y1));
			break;
		case 'A':
			scanf("%d%d%d",&x1,&y1,&n);
			x1++;
			y1++;
			block[x1][y1] += n;
			update(x1, y1, n);
			break;
		case 'D':
			scanf("%d%d%d",&x1,&y1,&n);
			x1++;
			y1++;
			n = min(n, block[x1][y1]);
			block[x1][y1] -= n;
			update(x1, y1, -n);
			break;
		case 'M':
			scanf("%d%d%d%d%d",&x1,&y1,&x2,&y2,&n);
			x1++;
			y1++;
			x2++;
			y2++;
			n = min(n, block[x1][y1]);
			block[x1][y1] -= n;
			update(x1, y1, -n);
			block[x2][y2] += n;
			update(x2, y2, n);
			break;
		}
	}
}

int main()
{
	int i, t;
	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		printf("Case %d:\n",i);
		domain();
	}
	return 0;
}