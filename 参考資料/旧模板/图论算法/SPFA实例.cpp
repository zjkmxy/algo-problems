/*
HDU2112 SPFA邻接表手动队列版
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 2000

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
const int MAX_DIST = 0x0fffffff;

NODE graph[MAXP];      //图的邻接表
//queue<int> *oplst; //开表
int oplst[MAXP];
int posa, posb;

char name[200][100];  //记地名
int cnt;             //地方个数
int stp,edp;

void init_queue()
{
    posb = posa = 0;
}

void push(int x)
{
    oplst[posb] = x;
    posb++;
    if(posb == MAXP)
        posb = 0;
}

int pop()
{
    posa++;
    if(posa == MAXP)
    {
        posa = 0;
        return oplst[MAXP-1];
    }
    return oplst[posa-1];
}

int se_str(char *str)
{
    int i;

    for(i=0;i<cnt;i++)
    {
        if(strcmp(str, name[i]) == 0)
            return i;
    }

    //加入了站
    strcpy(name[cnt], str);
    graph[cnt].dist = MAX_DIST;
    graph[cnt].inol = false;
    graph[cnt].start = NULL;
    cnt++;
    return (cnt - 1);
}

void SPFA()
{
    int curr,ae;
    PEDGE p;

    while(posb != posa)
    {
        curr = pop();
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
                    push(ae);
                    graph[ae].inol = true;
                }
            }
            p = p->next;
        }
    }
}

bool input()
{
    int n, i, a, b, d;
    char stra[100],strb[100];
    PEDGE temp;

    if(scanf("%d",&n)==EOF)
    {
        return false;
    }
    if(n == -1)
        return false;

    cnt = 0;
    scanf("%s%s", stra, strb);
    stp = se_str(stra);
    edp = se_str(strb);

    for(i=0;i<n;i++)
    {
        scanf("%s%s%d", stra, strb, &d);
        a = se_str(stra);
        b = se_str(strb);
        temp = (PEDGE)malloc(sizeof(EDGE));
        temp->ve = b;
        temp->w = d;
        temp->next = graph[a].start;
        graph[a].start = temp;
        temp = (PEDGE)malloc(sizeof(EDGE));
        temp->ve = a;
        temp->w = d;
        temp->next = graph[b].start;
        graph[b].start = temp;
    }

    return true;
}

void freegp()
{
    int i;
    PEDGE p,tmp;
    for(i=0;i<cnt;i++)
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

int main()
{
    while(input())
    {
        graph[stp].dist = 0;
        graph[stp].inol = true;
        init_queue();
        push(stp);
        SPFA();
        if(graph[edp].dist < MAX_DIST)
            printf("%d\n",graph[edp].dist);
        else
            printf("-1\n");
        freegp();
    }
    return 0;
}