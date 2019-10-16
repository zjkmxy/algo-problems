/*
一天GG正在和他的后宫之一的MM在外面溜达，MM突然说了一句，“我想吃鸡蛋灌饼”……当他们吃的正high的时候，城管出现了！作为传说中的最强军事力量，卖鸡蛋灌饼的小贩在他们面前也只算是战力为的5的渣滓，一秒钟就被秒杀了……

在这场屠杀中，GG和他的后宫本来只是围观群众，但是不幸的是，城管看到了GG胃里的鸡蛋灌饼，他们要逮捕GG！但是GG显然不能让他们如愿，于是GG带着后宫开始了往大运村的逃亡之旅。

整个地图有n个路口，灌饼摊在0号路口，大运村在n-1号路口。有m条只能单向通过的道路连接这n个路口，每条道路用一个正整数表示走过需要的时间。整个地图没有环路，但两个路口之间可能有多条通路。现在GG希望以最短的时间到大运村，但不幸的是，城管为了抓住他动用了卫星对他进行空中跟踪，并且会在某一时刻空降到某一条道路上进行封锁（封锁会在瞬间完成，可惜动静太大了GG也能在第一时间知道哪条道路被封锁了），之后这条路就无法通过了。在整个行动中只会出现一次空降，而且不会在GG经过这条道路的时候进行封锁，也就是说，不会在GG在某条路上走了一半的时候封锁这条路。而且，城管们希望尽可能的延缓GG到达大运村的时间。

现在GG希望知道，自己多久能到达大运村，方便安排之后和其他后宫的约会。

注意双方是以博弈的思想来进行选择，即GG希望时间最短，城管希望时间最长，而且他们都非常聪明会做出最佳的选择。



输入第一行为数据组数T（T<=30）。

每组数据第一行包含两个整数n,m（2<=n<=10000，1<=m<=100000），表示路口数和道路数。之后m行描述了所有的道路，每行有三个整数u,v,w（0<=u,v<n，0<w<=1000），表示路口u到路口v有一条需要w时间走过的道路。

对于每组数据输出一个整数，表示GG最后到达大运村需要的时间。如果GG无法到达大运村，输出-1。


*/

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
#include<stack>
using namespace std;

#define MAXP 10005
#define MAXI 0x0fffffff
#define MAXQ 100005

struct NODE
{
	int dis;  //最短距离；dis=min{dis, dis'+w}
	int dis2; //次短距离
	int fin;  //最终的结果；fin=max{dis2,all_min{fin'+w}}

	int tail;
	int head;
	bool inol;
	int indeg;
}node[MAXP];

struct EDGE
{
	int st;
	int ed;
	int w;
	int next;
	int ne;
}edge[MAXQ];

int N, M;
int oplst[MAXP];
int cnt;

void input()
{
	int i,a,b,w;

	scanf("%d%d",&N,&M);
	cnt = 0;
	for(i=0;i<N;i++)
	{
		node[i].dis = MAXI;
		node[i].fin = MAXI;
		node[i].dis2 = MAXI;
		node[i].tail = -1;
		node[i].head = -1;
		node[i].indeg = 0;
		node[i].inol = false;
	}
	for(i=0;i<M;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		edge[i].st = a;
		edge[i].ed = b;
		edge[i].w = w;
		edge[i].next = node[b].tail;
		node[b].tail = i;
		edge[i].ne = node[a].head;
		node[a].head = i;
		node[a].indeg++;
	}
}

void topo()
{
	queue<int> cq;
	int cur, j, st;

	for(j=0;j<N;j++)
	{
		if(node[j].indeg == 0)
		{
			oplst[cnt] = j;
			cnt++;
			cq.push(j);
		}
	}
	while(!cq.empty())
	{
		cur = cq.front();
		cq.pop();
		for(j=node[cur].tail;j>=0;j=edge[j].next)
		{
			st = edge[j].st;
			node[st].indeg--;
			if(!node[st].inol && node[st].indeg == 0)
			{
				oplst[cnt] = st;
				cnt++;
				cq.push(st);
				node[st].inol = true;
			}
		}
	}
}

void calc1()
{
	int cur, j, st;
	int ptr;

	node[N-1].dis = 0;
	node[N-1].dis2 = 0;
	for(ptr=0;ptr<cnt;ptr++)
	{
		cur = oplst[ptr];
		for(j=node[cur].tail;j>=0;j=edge[j].next)
		{
			st = edge[j].st;

			if(node[cur].dis + edge[j].w <= node[st].dis)
			{
				node[st].dis2 = node[st].dis;
				node[st].dis = node[cur].dis + edge[j].w;
			}else if(node[cur].dis + edge[j].w <= node[st].dis2)
			{
				node[st].dis2 = node[cur].dis + edge[j].w;
			}
		}
	}
}

int calc2()
{
	int cur, j, ed, tmp;
	int ptr;

	node[N-1].fin = 0;
	for(ptr=0;ptr<cnt;ptr++)
	{
		cur = oplst[ptr];
		if(cur == N-1)
			continue;
		node[cur].fin = node[cur].dis2;
		tmp = MAXI;
		for(j=node[cur].head;j>=0;j=edge[j].ne)
		{
			ed = edge[j].ed;
			tmp = min(tmp, node[ed].fin + edge[j].w);
		}
		node[cur].fin = max(node[cur].fin, tmp);
	}
	if(node[0].fin >= MAXI)
		node[0].fin = -1;
	return node[0].fin;
}

int main()
{
	int T;

	scanf("%d",&T);
	while(T--)
	{
		input();
		topo();
		calc1();
		printf("%d\n",calc2());
	}
	return 0;
}
