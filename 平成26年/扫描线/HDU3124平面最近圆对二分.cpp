/*
二分+双扫描线法
*/
#include <set>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define MAXN 50005
const double eps = 1e-8;

double pls; //额外半径

struct CIRCLE  //圆
{
	double x, y, r;
	int cnt;
	bool insec(const CIRCLE other)
	{
		return ((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) <= (pls * 2.0 + r + other.r)*(pls * 2.0 + r + other.r));
	};
	double dist(const CIRCLE other)
	{
		return sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y)) - (r + other.r);
	}
}circle[MAXN];


struct SLICE
{
	int id;   //所属圆的编号
	SLICE(int i=0)
	{
		id = i;
	};
	bool operator<(const SLICE other)const
	{
		double y1, y2;
		y1 = circle[id].y;
		y2 = circle[other.id].y;
		if(y1 != y2)
			return y1 > y2;
		else
			return circle[id].x > circle[other.id].x;
	};
};

struct NODE
{
	int id;
	double x;
	bool operator<(const NODE other)const
	{
		return x < other.x;
	};
}nl[MAXN], nr[MAXN];
int ccnt, ncnt;

set<SLICE> scn;  //圆弧集合

void add_cir(double x, double y, double r)
{
	circle[ccnt].cnt = 0;
	circle[ccnt].x = x;
	circle[ccnt].y = y;
	circle[ccnt].r = r;
	nl[ncnt].id = ccnt;
	nl[ncnt].x = x - r;
	nr[ncnt].id = ccnt;
	nr[ncnt].x = x + r;
	ncnt++;
	ccnt++;
}

bool insert(int id)
{
	set<SLICE>::iterator pri, nex;  //前后的迭代器

	pri = nex = scn.insert(SLICE(id)).first;
	nex++;
	if(pri != scn.begin())
	{
		pri--;
		if(circle[pri->id].insec(circle[id]))
			return true;
	}
	if(nex != scn.end())
	{
		if(circle[nex->id].insec(circle[id]))
			return true;
	}
	return false;
}

bool do_scan()
{
	int i, j;

	i = j = 0;
	scn.clear();
	while(i < ncnt || j < ncnt)
	{
		if(i == ncnt)
		{
			scn.erase(SLICE(nr[j].id));
			j++;
			continue;
		}
		if(j == ncnt)
		{
			if(insert(nl[i].id))
				return true;
			i++;
			continue;
		}
		if(nl[i].x - pls < nr[j].x + pls)
		{
			if(insert(nl[i].id))
				return true;
			i++;
			continue;
		}else{
			scn.erase(SLICE(nr[j].id));
			j++;
			continue;
		}
	}
	return false;
}

double BiSearch()
{
	double begin, end, mid;

	begin = 0.0;
	end = circle[0].dist(circle[1]);
	while(end - begin > eps)
	{
		mid = (begin + end) / 2.00;
		pls = mid;
		if(do_scan())
			end = mid;
		else
			begin = mid;
	}
	return begin;
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
	sort(nl, nl + ncnt);
	sort(nr, nr + ncnt);

	return true;
}


int main()
{
	int t;
	scanf("%d",&t);
	while(t--)
	{
		input();
		printf("%lf\n",BiSearch() * 2.0);
	}
	return 0;
}
