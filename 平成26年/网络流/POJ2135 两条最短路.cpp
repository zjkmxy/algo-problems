/*
POJ 2135
题目描述：
有n个景点，一个人要从1号景点走到n号景点，再从n号景点走到1号（回来的路不能重复，不一定走完所有景点，只要求从1到n即可），
给你一些景点之间的路的长度（双向），问你最短需要走多少路才能回来？
解题报告：
于是，我很傻B地想着说直接求两次最短路径，记录第一次最短路径的路径。但是zhc大牛说这样说错的，一个反例让我醒了:
5 7
1 2 4
1 3 1
1 4 6
2 5 1
2 3 2
3 5 4
3 4 5
当第一次最短路径后，得到的路径是1->3->2->5,第二次是1->4->3->5，总距离是19，如果我们选择1->2->5还有1->3->5那样总距离就是10
于是，只能用费用流了。
最小费用就是路径长度的总和，最大流就是来回的两条路。
由于去和回来可以看成：2条从1到n的不同的路。所以转化成求从1到n的两条不同的路。
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

void FFCalc(PEDGE ret)  //传入ret->st为源，ret->ed为汇，传出ret->flow为最大流，ret->cost为最小代价
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

void addedge(int a, int b, int f, int c)  //增加边
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