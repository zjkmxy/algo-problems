#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<queue>
using namespace std;

typedef int COST;
#define MAXP 555
#define MAXQ 10005
#define INF  0x3fffffff
struct EDGE
{
    int u, v;
    COST w;
}edge[MAXQ];
int pcnt, ecnt;
int pre[MAXP], id[MAXP], vis[MAXP];
COST minin[MAXP];

bool DMST(int root, COST& ret)
{
    int i, u, v, cnt;
    ret = 0;
    while(true)
    {
        //找每个顶点的最小入弧
        for(i=0;i<pcnt;i++)
            minin[i] = INF;
        for(i=0;i<ecnt;i++)
        {
            u = edge[i].u;
            v = edge[i].v;
            if(edge[i].w < minin[v] && u != v)
            {
                pre[v] = u;
                minin[v] = edge[i].w;
            }
        }
        for(i=0;i<pcnt;i++)
        {
            if(i != root && minin[i] == INF)
                return false;
        }
        //找环
        cnt = 0;
        for(i=0;i<pcnt;i++)
        {
            id[i] = -1;
            vis[i] = -1;
        }
        minin[root] = 0;
        for(i=0;i<pcnt;i++)
        {
            ret += minin[i];
            v = i;
            while(vis[v] != i && id[v] == -1 && v != root)//每个点寻找其前序点，要么最终寻找至根部，要么找到一个环
            {
                vis[v] = i;
                v = pre[v];
            }
            if(v != root && id[v] == -1)  //缩点。因为目标环中点的前驱肯定不需要走，所以减去其权值。
            {
                for(u=pre[v];u!=v;u=pre[u])
                    id[u] = cnt;
                id[v] = cnt++;
            }
        }
        if(cnt == 0)
            break;
        //重构图
        for(i=0;i<pcnt;i++)
            if(id[i] == -1)
                id[i] = cnt++;
        for(i=0;i<ecnt;i++)
        {
            u = edge[i].u;
            v = edge[i].v;
            edge[i].u = id[u];
            edge[i].v = id[v];
            if(id[u] != id[v])
                edge[i].w -= minin[v];
        }
        pcnt = cnt;
        root = id[root];
    }
    return true;
}

void addedge(int u, int v, COST w)
{
    edge[ecnt].u = u;
    edge[ecnt].v = v;
    edge[ecnt].w = w;
    ecnt++;
}

int arr[MAXP], sum[MAXP];
struct LESSON
{
    int c, l1, d, l2, m;
}les[MAXQ];
int N, M, pos;

bool domain()
{
    int i, j;
    int ret;
    scanf("%d%d",&N,&M);
    if(N == 0 && M == 0)
        return false;
    pos = 1;
    ecnt = 0;
    sum[0] = 1;
    for(i=1;i<=N;i++)
    {
        scanf("%d",&arr[i]);
        arr[i]++;
        sum[i] = sum[i-1] + arr[i];
    }
    pcnt = sum[N];
    for(i=1;i<=N;i++)
    {
        addedge(0, sum[i-1], 0);
        for(j=sum[i-1]+1;j<sum[i];j++)
        {
            addedge(j, j-1, 0);
        }
    }
    for(i=1;i<=M;i++)
    {
        scanf("%d%d%d%d%d",&les[i].c,&les[i].l1,&les[i].d,&les[i].l2,&les[i].m);
        addedge(sum[les[i].c-1]+les[i].l1, sum[les[i].d-1]+les[i].l2, les[i].m);
    }
    if(!DMST(0, ret))
        ret = -1;
    printf("%d\n",ret);
    return true;
}

int main()
{
    while(domain());
    return 0;
}