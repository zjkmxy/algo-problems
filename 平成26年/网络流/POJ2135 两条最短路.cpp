/*
POJ 2135
��Ŀ������
��n�����㣬һ����Ҫ��1�ž����ߵ�n�ž��㣬�ٴ�n�ž����ߵ�1�ţ�������·�����ظ�����һ���������о��㣬ֻҪ���1��n���ɣ���
����һЩ����֮���·�ĳ��ȣ�˫�򣩣����������Ҫ�߶���·���ܻ�����
���ⱨ�棺
���ǣ��Һ�ɵB������˵ֱ�����������·������¼��һ�����·����·��������zhc��ţ˵����˵��ģ�һ��������������:
5 7
1 2 4
1 3 1
1 4 6
2 5 1
2 3 2
3 5 4
3 4 5
����һ�����·���󣬵õ���·����1->3->2->5,�ڶ�����1->4->3->5���ܾ�����19���������ѡ��1->2->5����1->3->5�����ܾ������10
���ǣ�ֻ���÷������ˡ�
��С���þ���·�����ȵ��ܺͣ�������������ص�����·��
����ȥ�ͻ������Կ��ɣ�2����1��n�Ĳ�ͬ��·������ת�������1��n��������ͬ��·��
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1005
#define MAXI 0x7fffffff
#define MAXC 0x0fffffff

struct _EDGE{
	int st, ed; //��㣬�յ�
	int flow;
	int cost;
	struct _EDGE *next; //��һ����
	struct _EDGE *revs; //����ߣ�����Ҫ��߱���ɶԳ��֣�
};
typedef struct _EDGE EDGE;
typedef struct _EDGE *PEDGE;

struct _NODE{
	PEDGE prev;    //ǰ����
	int   cost;    //��С���Ŵ���
	bool  inol;    //�Ƿ��ڿ�����
	PEDGE edge;
};
typedef struct _NODE NODE;
typedef struct _NODE *PNODE;

NODE point[MAXP];         //�����
int np;                   //������

int Augment(int sp, int tp)
{
	queue<int> oplst;
	int ret, a;
	PEDGE eg;

	for(a=0;a<np;a++)
	{
		point[a].prev = NULL;
		point[a].inol = false;
		point[a].cost = MAXC;
	}
	oplst.push(sp);
	point[sp].inol = true;
	point[sp].cost = 0;
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		point[a].inol = false;
		for(eg=point[a].edge;eg!=NULL;eg=eg->next)
		{
			if(eg->flow <= 0)
				continue;
			if(point[eg->ed].cost <= point[a].cost + eg->cost)
				continue;

			point[eg->ed].prev = eg;
			point[eg->ed].cost = point[a].cost + eg->cost;
			if(!point[eg->ed].inol)
			{
				point[eg->ed].inol = true;
				oplst.push(eg->ed);
			}
		}
	}
	if(point[tp].prev == NULL)
		return 0;

	ret = MAXI;
	eg = point[tp].prev;
	while(eg != NULL)
	{
		ret = min(ret, eg->flow);
		eg = point[eg->st].prev;
	}
	eg = point[tp].prev;
	while(eg != NULL)
	{
		eg->flow -= ret;
		eg->revs->flow += ret;
		eg = point[eg->st].prev;
	}
	return ret;
}

void FFCalc(PEDGE ret)  //����ret->stΪԴ��ret->edΪ�㣬����ret->flowΪ�������ret->costΪ��С����
{
	int ad;

	if(ret == NULL)
		return ;
	ret->flow = 0;
	ret->cost = 0;
	ad = Augment(ret->st, ret->ed);
	while(ad > 0)
	{
		ret->flow += ad;
		ret->cost += point[ret->ed].cost;
		ad = Augment(ret->st, ret->ed);
	}
}

void freegp()
{
	int i;
	PEDGE p,tmp;
	for(i=0;i<np;i++)
	{
		p = point[i].edge;
		while(p != NULL)
		{
			tmp = p->next;
			free(p);
			p = tmp;
		}
		point[i].edge = NULL;
	}
}

void addedge(int a, int b, int f, int c)  //���ӱ�
{
	PEDGE tmp, tmp2;

	tmp  = (PEDGE)malloc(sizeof(EDGE));
	tmp2 = (PEDGE)malloc(sizeof(EDGE));
	tmp->st = a;
	tmp->ed = b;
	tmp->flow = f;
	tmp->cost = c;
	tmp->revs = tmp2;
	tmp2->st = b;
	tmp2->ed = a;
	tmp2->flow = 0;
	tmp2->cost = -c;
	tmp2->revs = tmp;
	
	tmp->next = point[a].edge;
	point[a].edge = tmp;
	tmp2->next = point[b].edge;
	point[b].edge = tmp2;
}

bool input()
{
	int m, i, a, b, w;

	if(scanf("%d%d",&np,&m)==EOF)
		return false;

	for(i=0;i<m;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		addedge(a, b, 1, w);
		addedge(b, a, 1, w);
	}
	addedge(0, 1, 2, 0);
	addedge(np, np+1, 2, 0);
	np += 2;

	return true;
}

int main()
{
	EDGE ret;

	while(input())
	{
		ret.st = 0;
		ret.ed = np - 1;
		FFCalc(&ret);
		printf("%d\n",ret.cost);
		freegp();
	}

	return 0;
}