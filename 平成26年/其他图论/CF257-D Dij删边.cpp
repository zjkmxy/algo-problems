/*
在编号为1~n的城市之间有若干条公路和若干条和1相关联的铁道，要求在保证从1出发的最短路不变的情况下尽可能地删除铁道，问能删多少条。
必须用Dijkstra写，不然会TLE
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <queue>
#include <set>
using namespace std;
#define ll int
#define MAXD 0x3fffffff

int head[100050],vis[100050],n,m,k;
int pre[100050];
ll dist[100050];
struct NODE
{
    int to,next;
    int mm;
    int use;
    ll len;
} edge[10000000];
struct STAT
{
	int ed;
	int dist;
	STAT(int e=0, int d=0)
	{
		ed = e;
		dist = d;
	};
	bool operator<(const STAT other) const
	{
		if(dist == other.dist)
			return ed < other.ed;
		return dist < other.dist;
	};
};
int rr,tt;
void spfa(int s)
{
    int i;
    for(i=1; i<=n; i++) dist[i]=MAXD;
    queue<int> S;
    while(!S.empty()) S.pop();
    S.push(s);
    dist[s]=0;
    while(!S.empty())
    {
        int k=S.front();
        S.pop();
        vis[k]=0;
        for(i=head[k]; i!=-1; i=edge[i].next)
        {
            int to=edge[i].to;
            if(dist[to] > dist[k] + edge[i].len)
            {
                if(pre[to] > 0) edge[pre[to]].use=0;
                dist[to]=dist[k]+edge[i].len;
                pre[to]=i;
                edge[i].use=1;
                if(!vis[to])
                {
                    S.push(to);
                    vis[to]=1;
                }
            }
            else if(dist[to] == dist[k] + edge[i].len)
            {
                if(pre[to] > 0)
                {
                    if(edge[pre[to]].mm == 1 && edge[i].mm == 0)
                    {
                        edge[pre[to]].use=0;
                        pre[to]=i;
                        edge[i].use=1;
                    }
                }
            }
        }
    }
}

void Dijkstra(int s)
{
	int j, ed, tmp, tmp2;
	STAT cur(s, 0);
	set<STAT> oplst;

	oplst.insert(cur);
	dist[cur.ed] = 0;
	while(!oplst.empty())
	{
		cur = *oplst.begin();
		oplst.erase(oplst.begin());
		vis[cur.ed] = true;
		for(j=head[cur.ed];j>=0;j=edge[j].next)
		{
			tmp = dist[cur.ed] + edge[j].len;
			ed = edge[j].to;
			
			if(!vis[ed] && dist[ed] > tmp)
			{
				if(pre[ed] > 0)
					edge[pre[ed]].use=0;
				tmp2 = dist[ed];
                dist[ed] = tmp;
                pre[ed]=j;
                edge[j].use=1;
				oplst.erase(STAT(ed, tmp2));
                oplst.insert(STAT(ed, tmp));
			}else if(dist[ed] == tmp)
			{
				if(pre[ed] > 0)
                {
                    if(edge[pre[ed]].mm == 1 && edge[j].mm == 0)
                    {
                        edge[pre[ed]].use=0;
                        pre[ed]=j;
                        edge[j].use=1;
                    }
                }
			}
		}
	}
}

void solve()
{
    int i;
    int a,b;
    ll c;
    tt=0,rr=0;
    memset(vis,0,sizeof(vis));
    memset(pre,-1,sizeof(pre));
    memset(head,-1,sizeof(head));
	for(i=1; i<=n; i++)
		dist[i]=MAXD;
    for(i=0; i<m; i++)
    {
        scanf("%d %d %d",&a,&b,&c);
        edge[tt].to=b,edge[tt].len=c,edge[tt].next=head[a],edge[tt].mm=0,edge[tt].use=0,head[a]=tt++;
        edge[tt].to=a,edge[tt].len=c,edge[tt].next=head[b],edge[tt].mm=0,edge[tt].use=0,head[b]=tt++;
    }
    rr=tt;
    for(i=0; i<k; i++)
    {
        scanf("%d %d",&b,&c);
        edge[rr].to=b,edge[rr].len=c,edge[rr].next=head[1],edge[rr].mm=1,edge[rr].use=0,head[1]=rr++;
        edge[rr].to=1,edge[rr].len=c,edge[rr].next=head[b],edge[rr].mm=1,edge[rr].use=0,head[b]=rr++;
    }
    //spfa(1);
	Dijkstra(1);
    int ans=0;
    memset(vis,0,sizeof(vis));
    for(i=tt; i<rr; i++) if(edge[i].use && !vis[edge[i].to]) ans++,vis[edge[i].to]=1;
    printf("%d\n",k-ans);
}

int main()
{
    scanf("%d %d %d",&n,&m,&k);
    solve();
	return 0;
}
