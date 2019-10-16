#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <deque>
using namespace std;
 
//@起点，.可通行，#不可通行。寻找可达点数目。

typedef unsigned long int UINT;

struct _POINT
{
	UINT x,y;
};
typedef _POINT POINT;
typedef POINT* PPOINT;

UINT map[30][30];  //0,white;1,black;
deque<POINT> *openlst, *closelst;

int Find(deque<POINT> *list, POINT pt)
{
	deque<POINT>::iterator ele;
	
	if(list->empty())
		return 0;

	for(ele = list->begin();ele != list->end();ele++)
	{
		if((pt.x == ele->x)&&(pt.y == ele->y))
			return 1;
	}
	return 0;
}

void GetMap(int w, int h)
{
	int i,j;
	char str[30];
	POINT st;

	openlst->clear();
	closelst->clear();
	//memset(map, 0, sizeof(UINT) *30 * 30);
	if((w <= 0)||(h <= 0))
		return;

	for(i=0;i<h;i++)
	{
		gets(str);
		for(j=0;j<w;j++)
		{
			switch(str[j])
			{
			case '.':
				map[i][j] = 1;
				break;
			case '#':
				map[i][j] = 0;
				break;
			case '@':
				map[i][j] = 0;  //加入列表的就不处理
				st.x = i;
				st.y = j;
				openlst->push_back(st);
				break;
			}
		}
	}
}

int AccessBlock(int w, int h)
{
	int ret = 0;
	POINT curr,pt;

	while(!openlst->empty())
	{
		curr = openlst->front();
		openlst->pop_front();
		closelst->push_back(curr);
		ret++;

		if((curr.x > 0)&&(map[curr.x-1][curr.y] > 0))
		{
			pt.x = curr.x-1;
			pt.y = curr.y;
			openlst->push_back(pt);
			map[curr.x-1][curr.y] = 0;
		}
		if((curr.x < h-1)&&(map[curr.x+1][curr.y] > 0))
		{
			pt.x = curr.x+1;
			pt.y = curr.y;
			openlst->push_back(pt);
			map[curr.x+1][curr.y] = 0;
		}
		if((curr.y > 0)&&(map[curr.x][curr.y-1] > 0))
		{
			pt.x = curr.x;
			pt.y = curr.y-1;
			openlst->push_back(pt);
			map[curr.x][curr.y-1] = 0;
		}
		if((curr.y < w-1)&&(map[curr.x][curr.y+1] > 0))
		{
			pt.x = curr.x;
			pt.y = curr.y+1;
			openlst->push_back(pt);
			map[curr.x][curr.y+1] = 0;
		}
	}

	return ret;
}

int main()
{
	int W,H;
	char tmp;
	openlst = new deque<POINT>();
	closelst = new deque<POINT>();

	while(1)
	{
		scanf("%d%d%c",&W,&H,&tmp); //清空格
		if((W == 0)&&(H == 0))
			break;

		GetMap(W, H);
		printf("%d\n",AccessBlock(W, H));
	}

	delete openlst;
	delete closelst;
	return 0;
}