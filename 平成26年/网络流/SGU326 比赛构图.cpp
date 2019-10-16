/*
SGU 326 Perspective
所有队伍打几场比赛，每场比赛都有一支队伍赢和另一支队伍输掉。
所有队伍被分成几个小组，一些比赛是在同一小组队伍中进行的较量，而另一些比赛是在小组与小组的较量。
给出现在球队的分数和你们组中所有队伍参加的比赛数目，和每个队伍两两之间剩下的比赛数量。确定球队是否能在小组赛中得第一。

分为2层为比赛和球队。
源向比赛流场数那么多的流量。
比赛向2只参赛队伍各流比赛场数那么多的流量。
队伍向汇流第一支队伍的最大胜场数减去自己当前胜场数的流量。
最后的流量为总比赛数（网络流满流）则可构造出。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1000
#define MAXI 0x7fffffff

int graph[MAXP][MAXP]; //流量邻接矩阵
int dist[MAXP];        //反向节点层次数组
int gap[MAXP];         //dist=i的节点个数
int pprev[MAXP];       //前驱结点
int lowf[MAXP];        //最小载流量（因为要回溯，所以必须用数组记）
int sp,tp,np;          //源，汇，顶点数

struct MATCH{
	int st;
	int ed;
	int cnt;
};

int w[30],r[30],s[30]; //已经胜利的比赛，残留的比赛，残留的组内赛
MATCH mat[MAXP];       //剩下的组内比赛
int cnt;
int N;

int build_dist()
{
	queue<int> oplst;
	int a, b;

	memset(dist, -1, sizeof(int) * np);
	memset(gap, 0, sizeof(int) * np);
	dist[tp] = 0;
	gap[0]++;
	oplst.push(tp);
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		for(b=0;b<np;b++)
		{
			if((dist[b] >= 0) || (graph[b][a] <= 0))
				continue;

			dist[b] = dist[a] + 1;
			gap[dist[b]]++;
			oplst.push(b);
		}
	}

	return dist[sp];
}

int SAPCalc()
{
	int a, b, ret, mindis;

	ret = 0;
	if(build_dist() == -1)
		return 0;
	a = sp;
	memset(lowf, 0, sizeof(int) * np);
	memset(pprev, -1, sizeof(int) * np);
	while(dist[sp] < np)
	{
		lowf[sp] = MAXI;
		for(b=0;b<np;b++)
		{
			if((graph[a][b] > 0)&&(dist[a] == dist[b] + 1)&&(dist[b] >= 0))
			{
				break;
			}
		}
		if(b < np)  //找到允许边
		{
			lowf[b] = min(lowf[a], graph[a][b]);
			pprev[b] = a;
			a = b;
			if(a == tp) //找到增广路
			{
				ret += lowf[tp];
				while(pprev[a] != -1)
				{
					graph[pprev[a]][a] -= lowf[tp];
					graph[a][pprev[a]] += lowf[tp];
					a = pprev[a];
				}//开始下一次的寻找
				a = sp;
				memset(lowf, 0, sizeof(int) * np);
				memset(pprev, -1, sizeof(int) * np);
			}
		}else{      //没有允许边
			//重标号dist
			mindis = MAXI;
			for(b=0;b<np;b++)
			{
				if((graph[a][b] > 0)&&(mindis > dist[b] + 1)&&(dist[b] >= 0))
				{
					mindis = dist[b] + 1;
				}
			}
			gap[dist[a]]--;
			if(gap[dist[a]] == 0)
				break;
			dist[a] = mindis;
			gap[mindis]++;
			if(a != sp)
				a = pprev[a];
		}
	}

	return ret;
}

bool input()
{
	int i, j, tmp;

	if(scanf("%d",&N)==EOF)
		return false;
	for(i=0;i<N;i++)
	{
		scanf("%d",&w[i]);
	}
	for(i=0;i<N;i++)
	{
		scanf("%d",&r[i]);
	}
	cnt = 0;
	//第0队的组内比赛全部直接赢
	for(j=0;j<N;j++)
	{
		scanf("%d",&tmp);
		w[0] += tmp;
		r[0] -= tmp;
		r[j] -= tmp;
	}
	w[0] += r[0];
	r[0] = 0;
	for(i=1;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			scanf("%d",&tmp);
			//记录过的比赛忽略
			if((j <= i)||(tmp == 0))
				continue;
			mat[cnt].st = i;
			mat[cnt].ed = j;
			mat[cnt].cnt = tmp;
			cnt++;
		}
	}

	return true;
}

bool calc()
{
	int i, sum, flow;

	memset(graph, 0, sizeof(graph));
	sum = 0;
	np = cnt + N + 1;
	sp = 0;
	tp = cnt + N;
	for(i=0;i<cnt;i++)
	{
		graph[sp][N+i] = mat[i].cnt;
		graph[N+i][mat[i].st] = mat[i].cnt;
		graph[N+i][mat[i].ed] = mat[i].cnt;
		sum += mat[i].cnt;
	}
	for(i=1;i<N;i++)
	{
		graph[i][tp] = w[0] - w[i];
		if(graph[i][tp] < 0)
			return false;
	}
	flow = SAPCalc();
	if(flow == sum)
		return true;
	else
		return false;
}

int main()
{
	while(input())
	{
		if(calc())
			printf("YES\n");
		else
			printf("NO\n");
	}

	return 0;
}