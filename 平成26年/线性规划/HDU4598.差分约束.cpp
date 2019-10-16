#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXT 500  //随意取的
#define MAXN 400
#define MAXI 0x0fffffff

int n;
bool map[MAXN][MAXN];  //1起点

struct EDGE{
	int end;
	int val;
	int next;
}edge[MAXN * MAXN];
int ecnt;

struct POINT{
	int color;  //颜色
	int cnt;    //进入次数
	int dist;   //距离(xi的最大可能值)
	int head;   //边
	bool in;    //是否在开表中
}point[MAXN];
bool flag;           //可行标记
deque<int> oplst;

//对每一个连通分支，2-着色只有对称的两种方案，所以随意即可
void DFS(int cur, int col)
{
	int i;

	point[cur].color = col;
	for(i=1;i<=n;i++)
	{
		if(map[cur][i])
		{
			if(point[i].color == 0)
				DFS(i, -col);
			else if(point[i].color == col) 
				flag = false;
		}
	}
}

//A->B:c表示xB-xA<=c
void addedge(int st, int ed, int val)
{
	edge[ecnt].end = ed;
	edge[ecnt].val = val;
	edge[ecnt].next = point[st].head;
	point[st].head = ecnt;
	ecnt++;
}

void SPFA()
{
	int curr,ae;
	int edg;

	oplst.clear();
	oplst.push_back(0);
	point[0].in = true;
	point[0].cnt++;
	point[0].dist = 0;
	while((!oplst.empty())&&flag)
	{
		curr = oplst.front();
		oplst.pop_front();
		point[curr].in = false;
		edg = point[curr].head;
		while(edg >= 0)
		{
			ae = edge[edg].end;
			if(point[ae].dist > point[curr].dist + edge[edg].val)
			{
				point[ae].dist = point[curr].dist + edge[edg].val;
				if(!point[ae].in)
				{
					oplst.push_back(ae);
					point[ae].in = true;
					point[ae].cnt++;
					if(point[ae].cnt > n)
					{
						flag = false;  //存在负权回路
					}
				}
			}
			edg = edge[edg].next;
		}
	}
}

bool calc()
{
	int i, j;

	flag = true;
	for(i=0;i<=n;i++)
	{
		point[i].cnt = 0;
		point[i].color = 0;
		point[i].dist = MAXI;
		point[i].head = -1;
		point[i].in = false;
	}
	for(i=1;i<=n;i++)
	{
		if(point[i].color == 0)
			DFS(i, 1);
	}
	if(!flag)
		return false;

	//下面这一句其实没啥必要，但是加上比不加跑得快，也不知道为啥。
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
			if(map[i][j]&&(point[i].color == point[j].color))
				return false;
	ecnt = 0;
	for(i=1;i<=n;i++)
	{
		if(point[i].color == 1)
		{
			addedge(0, i, MAXT-1);  //xi < T
			addedge(i, 0, 0);       //-xi <= 0
		}else{
			addedge(i, 0, MAXT-1);  //-xi < T
			addedge(0, i, 0);       //xi  <= 0
		}
		for(j=1;j<=n;j++)
		{
			if(map[i][j])
			{
				if(point[i].color == 1)
				{
					addedge(i, j, -MAXT);  //xj - xi <= -T
				}else{
					addedge(j, i, -MAXT);  //xi - xj <= -T
				}
			}else{
				if(point[i].color == 1)
				{
					addedge(j, i, MAXT-1);  //xi - xj < T
				}else{
					addedge(i, j, MAXT-1);  //xj - xi < T
				}
			}
		}
	}

	SPFA();
	return flag;
}

void input()
{
	int i, j, p;

	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			scanf("%1d", &p);
			if(p == 1)
				map[i][j] = true;
			else
				map[i][j] = false;
		}
	}
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		input();
		if(calc())
		{
			printf("Yes\n");
		}else{
			printf("No\n");
		}
	}

	return 0;
}
