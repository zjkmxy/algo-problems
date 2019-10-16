/*
Բ�ı�����

���Ⱥʹ�ͳɨ���ߵķ���һ���Ȱ�ÿ��Բ���Ҳ�x������ɢ�����Ÿ����Ǻý�����Ȼ��ʼɨ�衣
���ȫ�Ǿ��Σ��ͺܺô���Բ���鷳��û�����������������ɢ���Ĺ������½磬���޷���Ԥ����õ���ɢ����������߶�����
�������ǿ���ע�⵽��Ŀ��˵��Բ��Բ�������л��ཻ����ô����ɨ����ɨ��Ĺ����д��ϵ��´�������Բ��˳���ǲ����ģ���
�Կ������ö���������ɨ���߾����ġ��ߡ�����ز��루������set�ͺܷ����Ч�ˣ�������Բ��˵������߾������ϰ�Բ��������
������°�Բ���������ꡣ����ɨ����λ�õı仯������ʱ�����ȽϵĴ����ߡ���������Ҳ��仯������ǰ��˵����ɨ���ߴ���Բ
��˳���ǲ����ģ������µġ��ߡ���Ȼ����뵽��ȷ��λ�ã��������ߡ������λ��Ҳ����ı䡣
�ߵ��������ˣ��Ϳ�������ε�ͼ��������ˣ�
ɨ�赽һ������A���ʱ���Ϸ����·�û�бߣ����������������ࣻ
�Ϸ����·�������ͬһ������B�������A�Ⱦ���B��ȶ�1��
�Ϸ����·��߷ֱ����ھ���B�����C�������A�������Ǹ����������ͬ��
�ɴ˿ɵõ�ÿ�����ε���ȣ����������Ϊ�𰸡�
*/
#include <set>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define MAXN 50005

int X;  //̽����X

struct CIRCLE  //Բ
{
	int x, y, r;
	int cnt;
	double getr()
	{
		return sqrt(r*1.0*r - (X-x)*1.0*(X-x));
		//����һ��WA����ΪX^2�ǳ�int��
		//return sqrt(double(r*r - (X-x)*(X-x)));
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
			if(pri == scn.begin() || nex == scn.end())
			{
				//�������Բ
				circle[sl.id].cnt = 1;
			}else{
				pri--;
				if(pri->id == nex->id)
				{
					//���������Բ
					circle[sl.id].cnt = circle[pri->id].cnt + 1;
				}else{
					//��������һ���ǲ���Բ
					circle[sl.id].cnt = max(circle[pri->id].cnt, circle[nex->id].cnt);
				}
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
