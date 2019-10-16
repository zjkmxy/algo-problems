#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef struct _POINT
{
	WORD x, y;
} POINT, *PPOINT;
POINT port[4000005];
POINT queue[4000005];
const WORD dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
char map[2005][2005];
int step[2005][2005];
int N, M, top, back;
int nport;


POINT point(int x, int y)
{
	POINT ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

int comp(PPOINT A, PPOINT B)
{
	return (A->x==B->x && A->y==B->y);
}

int BFS(POINT st, POINT fed)
{
	POINT cur, ed;
	int i, ported = 0;

	back = 0;
	queue[0] = st;
	top = 1;
	step[st.x][st.y] = 0;
	while(back < top)
	{
		cur = queue[back];
		back++;
		if(comp(&cur, &fed))
			return step[cur.x][cur.y];
		for(i=0;i<4;i++)
		{
			ed = cur;
			ed.x += dir[i][0];
			ed.y += dir[i][1];

			if(map[ed.x][ed.y] == 'M')
				continue;
			if(step[cur.x][cur.y] + 1 >= step[ed.x][ed.y])
				continue;
			step[ed.x][ed.y] = step[cur.x][cur.y] + 1;
			queue[top] = ed;
			top++;
		}
		if(map[cur.x][cur.y] == 'E' && !ported)
		{
			ported = 1;
			for(i=0;i<nport;i++)
			{
				ed = port[i];
				if(step[cur.x][cur.y] >= step[ed.x][ed.y])
					continue;
				step[ed.x][ed.y] = step[cur.x][cur.y];
				queue[top] = ed;
				top++;
			}
		}
	}
	return -1;
}

int main()
{
	int i, j;
	POINT st, ed;

	scanf("%d%d",&N,&M);
	for(i=0;i<=N+1;i++)
	{
		for(j=0;j<=M+1;j++)
		{
			step[i][j] = 0x3fffffff;
			if(i==0 || i==N+1 || j==0 || j==M+1)
				map[i][j] = 'M';
		}
	}
	scanf("%*c");
	for(i=1;i<=N;i++)
	{
		for(j=1;j<=M;j++)
		{
			scanf("%1c",&map[i][j]);
			if(map[i][j] == 'N')
			{
				st.x = i;
				st.y = j;
			}
			if(map[i][j] == 'C')
			{
				ed.x = i;
				ed.y = j;
			}
			if(map[i][j] == 'E')
			{
				port[nport].x = i;
				port[nport].y = j;
				nport++;
			}
		}
		scanf("%*c");
	}

	j = BFS(st, ed);
	if(j >= 0)
	{
		printf("%d\n",j);
	}else{
		printf("Bad Eureka\n");
	}

	return 0;
}