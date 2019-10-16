#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned long int UINT;

struct _POINT
{
	UINT x,y;
};
typedef _POINT POINT;
typedef POINT* PPOINT;

UINT map[150][150];  //0,¿Õ;1,ÓÍÂ·;

void DFS(int i, int j, int w, int h)
{
	map[i][j] = 0;
	if((i > 0)&&(map[i-1][j] > 0))
		DFS(i-1,j,w,h);
	if((i < h-1)&&(map[i+1][j] > 0))
		DFS(i+1,j,w,h);
	if((j > 0)&&(map[i][j-1] > 0))
		DFS(i,j-1,w,h);
	if((j < w-1)&&(map[i][j+1] > 0))
		DFS(i,j+1,w,h);
	if((i > 0)&&(j > 0)&&(map[i-1][j-1] > 0))
		DFS(i-1,j-1,w,h);
	if((i < h-1)&&(j > 0)&&(map[i+1][j-1] > 0))
		DFS(i+1,j-1,w,h);
	if((i > 0)&&(j < w-1)&&(map[i-1][j+1] > 0))
		DFS(i-1,j+1,w,h);
	if((i < h-1)&&(j < w-1)&&(map[i+1][j+1] > 0))
		DFS(i+1,j+1,w,h);
}

int Deposits(int w, int h)
{
	int i,j;
	UINT branch;

	branch = 0;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			if(map[i][j])
			{
				DFS(i, j, w, h);
				branch++;
			}
		}
	}

	return branch;
}

void GetMap(int w, int h)
{
	int i,j;
	char str[150];

	if((w <= 0)||(h <= 0))
		return;

	for(i=0;i<h;i++)
	{
		gets(str);
		for(j=0;j<w;j++)
		{
			switch(str[j])
			{
			case '*':
				map[i][j] = 0;
				break;
			case '@':
				map[i][j] = 1;
				break;
			}
		}
	}
}

int main()
{
	int W,H;
	char tmp;

	while(1)
	{
		scanf("%d%d%c",&H,&W,&tmp); //Çå¿Õ¸ñ
		if((W == 0)&&(H == 0))
			break;

		GetMap(W, H);
		printf("%d\n",Deposits(W, H));
	}

	return 0;
}