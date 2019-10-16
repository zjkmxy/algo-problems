/*
HDU4780 Candy Factory
有一个糖果工厂，有M个机器生产N种糖果。你需要把N种糖果都生产出来。
对于每个机器j，都可以生产任意糖果i。每种糖果有一个生产时间的要求(si, ti)，意思是最晚要在si开始生产，在ti这个时间完成生产。
如果实际开始生产的时间是pi(si < pi < ti)，这种糖果依然会在ti完成，不过需要额外花费K*(pi - si)。如果pi >= ti则无法进行生产。
对于每个机器，他们在开始时(时间0)处于初始态。将处于初始态的机器j调整到生产糖果i的状态需要花费Cij的时间和Dij的钱。
将任意一台处于i1态的机器调整i2态需要花费Ei1i2的时间和Fi1i2的钱。询问将N种糖果都生产出来的最小花费，无法完成生产输出-1。

易知机器在生产完第一种糖果就会完全相同(当然状态是不同的)。
由于糖果完成生产的时间是确定的，那么此时我们可以用最后生产的糖果来代表这个机器(这点比较显然吧)。
于是我们将图分为三层(M1, M2, M3)：
        最开始的机器
        被某些机器不是在最开始生产的糖果
        生产完毕的糖果
S ->M1：流量1，花费0，代表每个机器最开始的状态。
S ->M2：流量1，花费0，代表任意机器生产完某糖果的状态。
M1->M3：流量1，花费为切换过去的代价（也许还有晚开始的花费）
M2->M3：根据M2完成的时间和M3需要的开始时间决定流量（0/1）以及花费。
M3->T ：流量1，花费0，表示完成生产。
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
	int st, ed; //起点，终点
	int flow;
	int cost;
	struct _EDGE *next; //下一条链
	struct _EDGE *revs; //逆向边（这里要求边必须成对出现）
};
typedef struct _EDGE EDGE;
typedef struct _EDGE *PEDGE;

struct _NODE{
	PEDGE prev;    //前导边
	int   cost;    //最小到着代价
	bool  inol;    //是否在开表里
	PEDGE edge;
};
typedef struct _NODE NODE;
typedef struct _NODE *PNODE;

NODE point[MAXP];         //顶点表
int np;                   //顶点数

struct DATA
{
	int time;  //开始时间，切换时间
	int cost;  //结束时间，切换费用
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