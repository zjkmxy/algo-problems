#include <stdio.h>
#include <stdlib.h>
#include <queue>
using namespace std;

struct _EDGE{
	int ve;
	int w;
	struct _EDGE* next;
};
typedef struct _EDGE EDGE;
typedef struct _EDGE* PEDGE;

struct _NODE{   //可以加入计数，即当节点多次到达开表中时失败。注意平均次数小于2.
	bool inol;  //是否在开表中
	int dist;   //到着距离
	PEDGE start;
};
typedef struct _NODE NODE;
typedef struct _NODE* PNODE;
const int MAX_DIST = 0x7fffffff;

PNODE graph;      //图的邻接表
queue<int> *oplst; //开表

void inputgp(int n, int m)
{
	int as,ae,w,i;
	PEDGE tmp;

	for(i=0;i<m;i++)
	{
		scanf("%d%d%d",&as,&ae,&w);
		tmp = (PEDGE)malloc(sizeof(EDGE));
		tmp->ve = ae;
		tmp->w = w;
		tmp->next = graph[as].start;
		graph[as].start = tmp;
	}
}

void freegp(int n)
{
	int i;
	PEDGE p,tmp;
	for(i=0;i<n;i++)
	{
		p = graph[i].start;
		while(p != NULL)
		{
			tmp = p->next;
			free(p);
			p = tmp;
		}
	}
}

void SPFA(int n)
{
	int curr,ae;
	PEDGE p;

	while(!oplst->empty())
	{
		curr = oplst->front();
		oplst->pop();
		graph[curr].inol = false;
		p = graph[curr].start;
		while(p != NULL)
		{
			ae = p->ve;
			if(graph[ae].dist > graph[curr].dist + p->w)
			{
				graph[ae].dist = graph[curr].dist + p->w;
				if(!graph[ae].inol)
				{
					oplst->push(ae);
					graph[ae].inol = true;
				}
			}
			p = p->next;
		}
	}
}

int main(int argc, char* argv[])
{
	int n,m,st,i;

	printf("请输入节点数、边数、起始节点：\n");
	scanf("%d%d%d",&n,&m,&st);
	graph = new NODE[n];
	oplst = new queue<int>;
	for(i=0;i<n;i++)
	{
		graph[i].dist = MAX_DIST;
		graph[i].start = NULL;
		graph[i].inol = false;
	}
	graph[st].dist = 0;
	graph[st].inol = true;
	oplst->push(st);
	printf("请顺次输入边： 起点 终点 权值\n");
	inputgp(n,m);
	
	printf("到各边的距离为：\n");
	SPFA(n);
	for(i=0;i<n;i++)
	{
		printf("%d ",graph[i].dist);
	}
	printf("\n");

	freegp(n);
	delete oplst;
	delete[] graph;
	system("pause");
	return 0;
}

