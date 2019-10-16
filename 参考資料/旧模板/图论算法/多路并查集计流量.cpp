/*
三种砖头：
A  左连上，右连下
B  左连下，右连上
C  白板
给出一符拼图，可以将最多两块C变成A或B，求最长联通线长度
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct BCSET{
	BCSET *pnt;
	int cnt;
};

struct BLOCK{
	BCSET up,down;
	char TYPE;
};

BLOCK map[40][40];
char orimap[40][40];

BCSET *root(BCSET *x, int cnt)
{
	BCSET *p;
	if(x->pnt == NULL)
	{
		x->cnt += cnt;
		return x;
	}else
	{
		p = root(x->pnt, cnt + x->cnt);
		x->cnt = 0;
		x->pnt = p;
		return p;
	}
}

int merge(BCSET *a, BCSET *b)
{
	a = root(a, 0);
	b = root(b, 0);
	if(a != b)
	{
		a->pnt = b;
		b->cnt += a->cnt;
		a->cnt = 0;
	}
	return b->cnt;
}

int count(BCSET *x)
{
	x = root(x, 0);
	return x->cnt;
}

int domap(int m, int n)
{
	int i,j,max,cur;
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			map[i][j].TYPE = orimap[i][j];
			map[i][j].up.cnt = 1;
			map[i][j].down.cnt = 1;
			map[i][j].up.pnt = NULL;
			map[i][j].down.pnt = NULL;
			switch(map[i][j].TYPE)
			{
			case 'A':
				if((i > 1)&&(map[i-1][j].TYPE != 'C')&&(map[i-1][j].TYPE != 'D'))
				{
					merge(&map[i][j].up, &map[i-1][j].down);
				}
				if((j > 1)&&(map[i][j-1].TYPE != 'C')&&(map[i-1][j].TYPE != 'D'))
				{
					if(map[i][j-1].TYPE == 'A')
					{
						merge(&map[i][j].up, &map[i][j-1].down);
					}else{
						merge(&map[i][j].up, &map[i][j-1].up);
					}
				}
				break;
			case 'B':
				if((i > 1)&&(map[i-1][j].TYPE != 'C')&&(map[i-1][j].TYPE != 'D'))
				{
					merge(&map[i][j].up, &map[i-1][j].down);
				}
				if((j > 1)&&(map[i][j-1].TYPE != 'C')&&(map[i-1][j].TYPE != 'D'))
				{
					if(map[i][j-1].TYPE == 'A')
					{
						merge(&map[i][j].down, &map[i][j-1].down);
					}else{
						merge(&map[i][j].down, &map[i][j-1].up);
					}
				}
				break;
			}
		}
	}
	max = 0;
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			cur = count(&map[i][j].up);
			if(cur > max)
				max = cur;
			cur = count(&map[i][j].down);
			if(cur > max)
				max = cur;
		}
	}
	return max;
}

int chg(int m, int n,int i)
{
	int max,pi,pj,cur;

	max = domap(m,n);
	for(pi=1;pi<=m;pi++)
	{
		for(pj=1;pj<=m;pj++)
		{
			if(orimap[pi][pj] != 'C')
				continue;

			orimap[pi][pj] = 'A';
			if(i == 0)
			{
				cur = domap(m, n);
			}else{
				cur = chg(m, n, 0);
			}
			if(cur > max)
				max = cur;

			orimap[pi][pj] = 'B';
			if(i == 0)
			{
				cur = domap(m, n);
			}else{
				cur = chg(m, n, 0);
			}
			if(cur > max)
				max = cur;

			orimap[pi][pj] = 'C';
		}
	}

	return max;
}

int main()
{
	int m,n,i,j;
	char ch;
	scanf("%d%d%c",&m,&n,&ch);
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%c%c",&orimap[i][j],&ch);
		}
	}
	printf("%d\n",chg(m,n,1));
	//system("pause");
	return 0;
}