/*
HDU4780 Candy Factory
��һ���ǹ���������M����������N���ǹ�������Ҫ��N���ǹ�������������
����ÿ������j�����������������ǹ�i��ÿ���ǹ���һ������ʱ���Ҫ��(si, ti)����˼������Ҫ��si��ʼ��������ti���ʱ�����������
���ʵ�ʿ�ʼ������ʱ����pi(si < pi < ti)�������ǹ���Ȼ����ti��ɣ�������Ҫ���⻨��K*(pi - si)�����pi >= ti���޷�����������
����ÿ�������������ڿ�ʼʱ(ʱ��0)���ڳ�ʼ̬�������ڳ�ʼ̬�Ļ���j�����������ǹ�i��״̬��Ҫ����Cij��ʱ���Dij��Ǯ��
������һ̨����i1̬�Ļ�������i2̬��Ҫ����Ei1i2��ʱ���Fi1i2��Ǯ��ѯ�ʽ�N���ǹ���������������С���ѣ��޷�����������-1��

��֪�������������һ���ǹ��ͻ���ȫ��ͬ(��Ȼ״̬�ǲ�ͬ��)��
�����ǹ����������ʱ����ȷ���ģ���ô��ʱ���ǿ���������������ǹ��������������(���Ƚ���Ȼ��)��
�������ǽ�ͼ��Ϊ����(M1, M2, M3)��
        �ʼ�Ļ���
        ��ĳЩ�����������ʼ�������ǹ�
        ������ϵ��ǹ�
S ->M1������1������0������ÿ�������ʼ��״̬��
S ->M2������1������0�������������������ĳ�ǹ���״̬��
M1->M3������1������Ϊ�л���ȥ�Ĵ��ۣ�Ҳ������ʼ�Ļ��ѣ�
M2->M3������M2��ɵ�ʱ���M3��Ҫ�Ŀ�ʼʱ�����������0/1���Լ����ѡ�
M3->T ������1������0����ʾ���������
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1005
#define MAXN 105
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

struct DATA
{
	int time;  //��ʼʱ�䣬�л�ʱ��
	int cost;  //����ʱ�䣬�л�����
};
DATA candy[MAXN];
DATA mach[MAXN][MAXN];
DATA chg[MAXN][MAXN];
int m, n, k;

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

void FFCalc(PEDGE ret)
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

void addedge(int a, int b, int f, int c)
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
	int i, j;

	if(scanf("%d%d%d",&n,&m,&k)==EOF)
		return false;
	if((m == 0)&&(n == 0))
		return false;

	for(i=1;i<=n;i++)
	{
		scanf("%d%d", &candy[i].time, &candy[i].cost);
	}
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			scanf("%d", &mach[j][i].time);
		}
	}
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			scanf("%d", &mach[j][i].cost);
		}
	}
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%d", &chg[i][j].time);
		}
	}
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%d", &chg[i][j].cost);
		}
	}

	return true;
}

int calc()
{
	EDGE ret;
	int i, j;

	np = m + n * 2 + 2;
	for(i=1;i<=m+n;i++)
	{
		addedge(0, i, 1, 0);
	}
	for(i=m+n+1;i<=m+2*n;i++)
	{
		addedge(i, m+2*n+1, 1, 0);
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(mach[i][j].time >= candy[j].cost)
				continue;

			if(mach[i][j].time <= candy[j].time)
			{
				addedge(i, m+n+j, 1, mach[i][j].cost);
			}else{
				addedge(i, m+n+j, 1, mach[i][j].cost+k*(mach[i][j].time-candy[j].time));
			}
		}
	}
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(i == j)
				continue;
			if(candy[i].cost + chg[i][j].time >= candy[j].cost)
				continue;

			if(candy[i].cost + chg[i][j].time <= candy[j].time)
			{
				addedge(m+i, m+n+j, 1, chg[i][j].cost);
			}else{
				addedge(m+i, m+n+j, 1, chg[i][j].cost+k*(candy[i].cost+chg[i][j].time-candy[j].time));
			}
		}
	}

	ret.st = 0;
	ret.ed = m + 2 * n + 1;
	FFCalc(&ret);
	freegp();
	if(ret.flow < n)
		return -1;
	else
		return ret.cost;
}

int main()
{
	while(input())
	{
		printf("%d\n",calc());
	}

	return 0;
}