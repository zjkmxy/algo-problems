/*
有不同的思路

方程法：
设某一种人招募数量为X[i]
每天的不等式（X[a1]+X[a2]+...+X[ap]>=P[j]）加变量换成等式：
X[a1]+...+X[ap]-Y[j]=P[j]
设P[0] = P[N+1] = 0
差分，可以发现左边的矩阵，不管是X的还是Y的都是有向图关联矩阵
于是直接拿费用流跑即可。

直接构图：
考虑前缀和。
每雇佣第i中志愿者相当于在Si天增加X[i]人，Ti+1天减少X[i]人。
从Si向Ti连一条费用为Ci的边，Si和Ti天为与之前相比增加的人数，那么流量守恒。
注意的是需求的人数可能少于实际供给的人数，也就是说增加的人数可以提起被消耗，也可以转移到增加之后的任何一天。
（如：从始至终招3人，但是需求人数是3 2 3）
于是每一天向之前的一天连无限的边。
跑最小费用可行流即可。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1005
#define MAXQ 15005
#define MAXI 0x7fffffff
#define MAXC 0x0fffffff
#define BIGM MAXC

struct EDGE{
	int st, ed; //起点，终点
	int flow;   //实际流量
	int low;    //流量上界
	int upf;    //流量下界
	int cost;   //线性费用
	int hnext;  //起始树边邻接表
	int tnext;  //终止树边邻接表
	int hprev;  //起始树边邻接表
	int tprev;  //终止树边邻接表

	char set;   //集合(T,L,U,N=L+U)
}edge[MAXQ+MAXP];//0起始边集
int ecnt;   //原始边数目
int ecnt2;  //大M构造边总数目
int sece;   //上一次查找到的边的号
bool search_method;  //查找方法->T最小查找/F离开查找，后者一般较快

struct NODE{
	int head;  //起始边
	int tail;  //终止边
	int d;     //供需量，正源负汇总守恒
	int pi;    //势函数

	int pred;  //前驱边
	int depth; //深度
}node[MAXP]; //1起始顶点集
int ncnt;    //顶点数

struct LOOP{
	int e;
	int prev;
	int next;
}loop[MAXP+3];
int lcnt;

int addedge(int a, int b, int low, int upf, int cost, int &cnt)
{
	edge[cnt].st = a;
	edge[cnt].ed = b;
	edge[cnt].low = low;
	edge[cnt].upf = upf;
	edge[cnt].cost = cost;
	edge[cnt].hnext = -1;
	edge[cnt].tnext = -1;
	edge[cnt].hprev = -1;
	edge[cnt].tprev = -1;
	cnt++;

	return (cnt - 1);
}

void insert_tree(int j)
{
	int a, b;

	a = edge[j].st;
	b = edge[j].ed;

	edge[j].hprev = -1;
	edge[j].hnext = node[a].head;
	if(node[a].head >= 0)
		edge[node[a].head].hprev = j;
	node[a].head = j;

	edge[j].tprev = -1;
	edge[j].tnext = node[b].tail;
	if(node[b].tail >= 0)
		edge[node[b].tail].tprev = j;
	node[b].tail = j;
}

void delete_tree(int j)
{
	int a, b;

	a = edge[j].st;
	b = edge[j].ed;

	if(edge[j].hprev >= 0)
	{
		edge[edge[j].hprev].hnext = edge[j].hnext;
	}else{
		node[a].head = edge[j].hnext;
	}
	if(edge[j].hnext >= 0)
	{
		edge[edge[j].hnext].hprev = edge[j].hprev;
	}
	edge[j].hprev = -1;
	edge[j].hnext = -1;

	if(edge[j].tprev >= 0)
	{
		edge[edge[j].tprev].tnext = edge[j].tnext;
	}else{
		node[b].tail = edge[j].tnext;
	}
	if(edge[j].tnext >= 0)
	{
		edge[edge[j].tnext].tprev = edge[j].tprev;
	}
	edge[j].tprev = -1;
	edge[j].tnext = -1;
}

void M_init()   //构造大M
{
	int i, j;

	node[0].d = 0;
	node[0].head = -1;
	node[0].tail = -1;

	for(i=0;i<ecnt;i++)
	{
		edge[i].flow = edge[i].low;
		edge[i].set = 'L';
		node[edge[i].st].d -= edge[i].low;
		node[edge[i].ed].d += edge[i].low;
	}

	ecnt2 = ecnt;
	for(i=1;i<=ncnt;i++)
	{
		//《网络优化》上写的有问题，照那样构造不出可行流
		if(node[i].d > 0)
		{
			j = addedge(i, 0, 0, MAXC, BIGM, ecnt2); 
			edge[j].flow = node[i].d;
		}else{
			j = addedge(0, i, 0, MAXC, BIGM, ecnt2);
			edge[j].flow = -node[i].d;
		}
		edge[j].set = 'T';
		insert_tree(j);
	}
}

//DFS计算单纯形乘子，深度和前驱
//除第一次调用的场合，可以利用搜索线优化，但考虑到编程复杂度不予实现
//如果维护一个树弧的双向链表，而不是每一次遍历所有边，也可以做到进一步优化。
void calc_pi(int root, int val, int dep, int pred)
{
	int j;

	node[root].pi = val;
	node[root].depth = dep;
	node[root].pred = pred;
	for(j=node[root].head;j!=-1;j=edge[j].hnext)
	{
		if(j != pred)
		{
			calc_pi(edge[j].ed, val - edge[j].cost, dep + 1, j);
		}
	}
	for(j=node[root].tail;j!=-1;j=edge[j].tnext)
	{
		if(j != pred)
		{
			calc_pi(edge[j].st, val + edge[j].cost, dep + 1, j);
		}
	}
}

//计算即约代价，返回违例弧
//1是最小查找，2是离开查找
int calc_dual1()   
{
	int i, ret = -1, dual0 = -1, dual;

	for(i=0;i<ecnt2;i++)
	{
		dual = edge[i].cost - node[edge[i].st].pi + node[edge[i].ed].pi;
		if((edge[i].set == 'L' && dual < 0)||(edge[i].set == 'U' && dual > 0))
		{
			dual = labs(dual);
			if(dual > dual0)  //此时如果不经过比较立刻返回i的话反而会慢
			{
				ret = i;
				dual0 = dual;
			}
		}
	}

	return ret;
}

//计算即约代价，返回违例弧
//1是最小查找，2是离开查找
int calc_dual2()   
{
	int i, dual;

	for(i=sece;i<ecnt2;i++)
	{
		dual = edge[i].cost - node[edge[i].st].pi + node[edge[i].ed].pi;
		if((edge[i].set == 'L' && dual < 0)||(edge[i].set == 'U' && dual > 0))
		{
			sece = i + 1;
			return i;
		}
	}
	for(i=0;i<sece;i++)
	{
		dual = edge[i].cost - node[edge[i].st].pi + node[edge[i].ed].pi;
		if((edge[i].set == 'L' && dual < 0)||(edge[i].set == 'U' && dual > 0))
		{
			sece = i + 1;
			return i;
		}
	}

	return -1;
}

//对违例弧rda执行增广，注意在有上下界的情况下不可能找不到出基弧
//如果用搜索线优化的话，这里需要直接计算出每个顶点的前驱边，修正无根子树顶点势并重构线
//返回值为：出基弧是否在rda增广之前->修正为是否DFS之st
bool augment(int rda)  
{
	int p, q;
	int pl, ql, pp, qp, maxflow;
	bool flagp, flagq, flag, ret;

	lcnt = 0;
	if(edge[rda].set == 'L')
	{
		p = edge[rda].st;
		q = edge[rda].ed;
	}else{
		q = edge[rda].st;
		p = edge[rda].ed;
	}
	loop[lcnt].e = rda;
	loop[lcnt].prev = 1;
	loop[lcnt].next = 2;
	pl = 1;
	ql = 2;
	loop[pl].e = -1;
	loop[pl].next = 0;
	loop[pl].prev = -1;
	loop[ql].e = -1;
	loop[ql].next = -1;
	loop[ql].prev = 0;
	lcnt = 3;

	//寻找环，pp和qp是前驱边，pl和ql是环中的位置
	maxflow = edge[rda].upf - edge[rda].low;
	while(p != q)
	{
		if(node[p].depth > node[q].depth)
		{
			flagp = true;
			flagq = false;
		}else if(node[p].depth < node[q].depth)
		{
			flagp = false;
			flagq = true;
		}else{
			flagp = true;
			flagq = true;
		}

		if(flagp)
		{
			pp = node[p].pred;
			if(edge[pp].ed == p)  //顺流
			{
				maxflow = min(maxflow, edge[pp].upf - edge[pp].flow);
				p = edge[pp].st;
			}else{                //逆流
				maxflow = min(maxflow, edge[pp].flow - edge[pp].low);
				p = edge[pp].ed;
			}
			loop[pl].e = pp;
			loop[pl].prev = lcnt;
			loop[lcnt].e = -1;
			loop[lcnt].next = pl;
			loop[lcnt].prev = -1;
			pl = lcnt;
			lcnt++;
		}

		if(flagq)
		{
			qp = node[q].pred;
			if(edge[qp].st == q)  //顺流
			{
				maxflow = min(maxflow, edge[qp].upf - edge[qp].flow);
				q = edge[qp].ed;
			}else{                //逆流
				maxflow = min(maxflow, edge[qp].flow - edge[qp].low);
				q = edge[qp].st;
			}
			loop[ql].e = qp;
			loop[ql].next = lcnt;
			loop[lcnt].e = -1;
			loop[lcnt].next = -1;
			loop[lcnt].prev = ql;
			ql = lcnt;
			lcnt++;
		}
	}

	//flag是是否找到出基弧,pl是环中的位置，ql是当前需要增广的边
	flag = false;
	while(loop[pl].next >= 0)
	{
		pl = loop[pl].next;
		ql = loop[pl].e;
		if(ql == -1)
			break;
		if(ql == rda)
		{
			ret = (flag == (edge[rda].set == 'L'));
		}

		if(edge[ql].st == p)  //顺流
		{
			edge[ql].flow += maxflow;
			if(!flag && (edge[ql].flow == edge[ql].upf))
			{
				edge[ql].set = 'U';
				flag = true;
				delete_tree(ql);
			}else{
				edge[ql].set = 'T';
			}
			p = edge[ql].ed;
		}else{                //逆流
			edge[ql].flow -= maxflow;
			if(!flag && (edge[ql].flow == edge[ql].low))
			{
				edge[ql].set = 'L';
				flag = true;
				delete_tree(ql);
			}else{
				edge[ql].set = 'T';
			}
			p = edge[ql].st;
		}
	}

	return ret;
}

//网络单纯形计算，无解返回false，否则返回true且cost为最小可行解的代价
bool NSMCalc(int &cost)
{
	int rda, i;

	M_init();
	calc_pi(0, 0, 0, -1);
	rda = (search_method ? calc_dual1() : calc_dual2());
	while(rda >= 0)
	{
		int j;

		insert_tree(rda);
		if(augment(rda))
		{
			i = edge[rda].st;
			j = edge[rda].ed;
			calc_pi(i, node[j].pi + edge[rda].cost, node[j].depth + 1, rda);
		}else{
			i = edge[rda].ed;
			j = edge[rda].st;
			calc_pi(i, node[j].pi - edge[rda].cost, node[j].depth + 1, rda);
		}
		
		rda = (search_method ? calc_dual1() : calc_dual2());
	}

	for(i=ecnt;i<ecnt2;i++)
	{
		if(edge[i].flow > 0)
			return false;
	}
	cost = 0;
	for(i=0;i<ecnt;i++)
	{
		cost += edge[i].flow * edge[i].cost;
	}

	return true;
}

void init_graph(int pointnum, bool method)
{
	int i;

	ncnt = pointnum;
	ecnt = 0;
	sece = 0;
	for(i=0;i<=ncnt;i++)
	{
		node[i].d = 0;
		node[i].head = -1;
		node[i].tail = -1;
		node[i].pred = -1;
		node[i].pi = 0;
		node[i].depth = 0;
	}
	search_method = method;
}

int N, M, pre;
int main(){
	int need, pre, i;
	int s, t, c;
	scanf("%d%d", &N, &M);
	init_graph(N + 1, false);
	for(pre = 0, i = 1; i <= N; i ++){
		scanf("%d", &need);
		node[i].d = need - pre;
		pre = need;
		addedge(i + 1, i, 0, MAXC, 0, ecnt);
	}
	node[N + 1].d = - pre;
	while(M --){
		scanf("%d%d%d", &s, &t, &c);
		addedge(s, t + 1, 0, MAXC, c, ecnt);
	}
	NSMCalc(need);
	printf("%d\n", need);
	return 0;
}