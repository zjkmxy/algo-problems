/*
ƽ�����Բ��(NlogN ����)

û�б�Ҫ������ö�ٰ뾶Ȼ��ɨ���ߣ������������ˡ�
���ַ���ֱ�Ӽ����ǱȽϺ��ʵġ�
Ψһ���ܵĴ�����ʱ���������Բ�����ļ��������ƽ�������ԡ�
*/
#include <set>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define MAXN 50005
const double MAXD = 1e6;

int X;  //̽����X

struct CIRCLE  //Բ
{
	int x, y, r;
	double getr()
	{
		return sqrt(r*1.0*r - (X-x)*1.0*(X-x));
	};
	bool operator<(const CIRCLE other)const
	{
		return x < other.x;
	};
}circle[MAXN];


struct SLICE  //Բ�����±߽绡
{
	int id;   //����Բ�ı��
	int flag; //���±��
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

set<SLICE> scn;  //Բ������

double dist(int id1, int id2)
{
	double ret = sqrt((circle[id1].x-circle[id2].x)*1.0*(circle[id1].x-circle[id2].x) + (circle[id1].y-circle[id2].y)*1.0*(circle[id1].y-circle[id2].y));
	return (ret - circle[id1].r - circle[id2].r);
}

void add_cir(int x, int y, int r)
{
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

double do_scan()
{
	int i;
	double m = MAXD;
	SLICE sl;
	set<SLICE>::iterator pri, nex;  //ǰ��ĵ�����

	scn.clear();
	for(i=0;i<ncnt;i++)
	{
		X = node[i].x;
		if(node[i].flag == 1)
		{
			//����һ��Բ
			sl.id = node[i].id;
			sl.flag = 1;
			pri = scn.insert(sl).first;
			nex = pri;
			nex++;
			if(nex != scn.end())
			{
				m = min(m, dist(nex->id, sl.id));
			}
			if(pri != scn.begin())
			{
				pri--;
				m = min(m, dist(pri->id, sl.id));
			}
			sl.flag = -1;
			scn.insert(sl);
		}else{
			//ɾȥһ��Բ
			sl.id = node[i].id;
			sl.flag = 1;
			scn.erase(sl);
			sl.flag = -1;
			scn.erase(sl);
		}
	}
	sort(circle, circle + ccnt);
	for(i=0;i<ccnt-1;i++)
	{
		m = min(m, dist(i, i+1));
	}

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
	int t;
	scanf("%d",&t);
	while(t--)
	{
		input();
		printf("%.6lf\n",do_scan());
	}
	return 0;
}
