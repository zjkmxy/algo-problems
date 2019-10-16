/*
圆的保序性

首先和传统扫描线的方法一样先把每个圆左右侧x坐标离散出来排个序标记好进出，然后开始扫描。
如果全是矩形，就很好处理。圆就麻烦在没有像矩形那样可以离散化的规则上下界，便无法用预处理好的离散编号来构建线段树。
但是我们可以注意到题目中说了圆与圆不会相切或相交，那么对于扫描线扫描的过程中从上到下穿过各个圆的顺序是不会变的，所
以可以利用二叉树，把扫描线经过的“边”有序地插入（这里用set就很方便高效了），对于圆来说，这个边就是与上半圆交点纵坐
标和与下半圆交点纵坐标。由于扫描线位置的变化，插入时用来比较的代表“边”的纵坐标也会变化，但是前面说过，扫描线穿过圆
的顺序是不会变的，所以新的“边”依然会插入到正确的位置，其他“边”的相对位置也不会改变。
边的问题解决了，就可以像矩形的图那样理解了：
扫描到一个矩形A左侧时，上方或下方没有边，则这个矩形在最外侧；
上方与下方边属于同一个矩形B，则矩形A比矩形B深度多1；
上方与下方边分别属于矩形B与矩形C，则矩形A与较深的那个矩形深度相同。
由此可得到每个矩形的深度，更新最深的为答案。
*/
#include <set>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define MAXN 50005

int X;  //探查点的X

struct CIRCLE  //圆
{
	int x, y, r;
	int cnt;
	double getr()
	{
		return sqrt(r*1.0*r - (X-x)*1.0*(X-x));
		//下面一句WA，因为X^2是超int的
		//return sqrt(double(r*r - (X-x)*(X-x)));
	};
}circle[MAXN];


struct SLICE  //圆的上下边界弧
{
	int id;   //所属圆的编号
	int flag; //上下标号
	bool operator<(const SLICE other)const
	{
		double y1, y2;
		y1 = (double)circle[id].y + (double)flag * circle[id].getr();
		y2 = (double)circle[other.id].y + (double)other.flag * circle[other.id].getr();
		if(y1 != y2)
			return y1 > y2;
		else
			return flag > other.flag;
	};
};

struct NODE
{
	int id;
	int x;
	int flag;
	bool operator<(const NODE other)const
	{
		if(x != other.x)
			return x < other.x;
		else
			return flag > other.flag;
	};
}node[MAXN<<1];
int ccnt, ncnt;

set<SLICE> scn;  //圆弧集合

void add_cir(int x, int y, int r)
{
	circle[ccnt].cnt = 0;
	circle[ccnt].x = x;
	circle[ccnt].y = y;
	circle[ccnt].r = r;
	node[ncnt].id = ccnt;
	node[ncnt].x = x - r;
	node[ncnt].flag = 1;
	ncnt++;
	node[ncnt].id = ccnt;
	node[ncnt].x = x + r;
	node[ncnt].flag = -1;
	ncnt++;
	ccnt++;
}

int do_scan()
{
	int i, m;
	SLICE sl;
	set<SLICE>::iterator pri, nex;  //前后的迭代器

	scn.clear();
	for(i=0;i<ncnt;i++)
	{
		X = node[i].x;
		if(node[i].flag == 1)
		{
			//插入一个圆
			sl.id = node[i].id;
			sl.flag = 1;
			pri = scn.insert(sl).first;
			nex = pri;
			nex++;
			if(pri == scn.begin() || nex == scn.end())
			{
				//最外面的圆
				circle[sl.id].cnt = 1;
			}else{
				pri--;
				if(pri->id == nex->id)
				{
					//上下是外包圆
					circle[sl.id].cnt = circle[pri->id].cnt + 1;
				}else{
					//上下至少一个是并列圆
					circle[sl.id].cnt = max(circle[pri->id].cnt, circle[nex->id].cnt);
				}
			}
			sl.flag = -1;
			scn.insert(sl);
		}else{
			//删去一个圆
			sl.id = node[i].id;
			sl.flag = 1;
			scn.erase(sl);
			sl.flag = -1;
			scn.erase(sl);
		}
	}

	m = 0;
	for(i=0;i<ccnt;i++)
		m = max(m, circle[i].cnt);
	return m;
}

bool input()
{
	int i, n, x, y, r;

	if(scanf("%d",&n)==EOF)
		return false;

	ccnt = 0;
	ncnt = 0;
	for(i=0;i<n;i++)
	{
		scanf("%d%d%d",&x,&y,&r);
		add_cir(x, y, r);
	}
	sort(node, node + ncnt);

	return true;
}


int main()
{
	while(input())
	{
		printf("%d\n",do_scan());
	}
	return 0;
}
