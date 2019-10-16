#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 500
#define MAXM 50000

int n, m;
int ehead[MAXN], medge[MAXM], enext[MAXM], degs[MAXN];

#define MAXP 500
#define MAXI 0x0fffffff

int dfs[MAXN];
bool vis[MAXN];
int cnt;

////自己的SAP板子有点小问题会RE，改过来了又有够慢的，没办法只好借用别人的版子……
//Dinic倒是能跑但是太慢了……
//这里不能加0作为图中的点
struct EDGE
 {
     int u, v, next;
     double cap;
 }edge[MAXM];

int dis[MAXN], cur[MAXN], gap[MAXN], pre[MAXN];
int num, head[MAXN];
const double INF = 10000000000;

void add(int u, int v, double w)
 {
     edge[num].u = u;
     edge[num].v = v;
     edge[num].cap = w;
     edge[num].next = head[u];
     head[u] = num++;
 }

double SAP(int s, int t)
 {
     memset(gap,0,sizeof(gap));
     memset(dis,0,sizeof(dis));
     int i;
     for(int i = 1;i <= t;i++)
      cur[i] = head[i];
     int top = s;
     gap[s] = t;
     double maxflow = 0,flow = INF;
     while(dis[s] < t)
      {
          for(i = head[top];i != -1;i = edge[i].next)
           {
               if(edge[i].cap > 0&& dis[top] == dis[edge[i].v] + 1)
                 break;
           }
          if(i != -1)
           {
               cur[top] = i;
               int v = edge[i].v;
               if(edge[i].cap < flow)
                flow = edge[i].cap;
               top = v;
               pre[v] = i;
               if(top == t)
               {
                   maxflow += flow;
                   while(top != s)
                    {
                        edge[pre[top]].cap -= flow;
                        edge[pre[top]^1].cap += flow;
                        top = edge[pre[top]^1].v;
                    }
                    flow = INF;
               }
           }
          else
          {
            if(--gap[dis[top]] == 0)
             break;
            dis[top] = t;
            cur[top] = head[top];
            for(int j = head[top];j != -1;j = edge[j].next)
             {
                 if(edge[j].cap > 0&& dis[edge[j].v] + 1 < dis[top])
                  {
                      dis[top] = dis[edge[j].v] + 1;
                      cur[top] = j;
                  }
             }
            gap[dis[top]]++;
            if(top != s)
             {
                 top = edge[pre[top]^1].v;
             }
          }
      }
    return maxflow;
 }

bool input()
{
	int i, a, b;

	if(scanf("%d%d",&n,&m)==EOF)
		return false;

	for(i=0;i<=n;i++)
	{
		ehead[i] = -1;
		degs[i] = 0;
	}
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&a,&b);
		medge[(i<<1)] = b;
		medge[(i<<1)+1] = a;
		enext[(i<<1)] = ehead[a];
		enext[(i<<1)+1] = ehead[b];
		ehead[a] = (i<<1);
		ehead[b] = (i<<1)+1;
		degs[a]++;
		degs[b]++;
	}
	return true;
}

void build_graph(double g)
{
	int i, j;

	num = 0;
	memset(head, -1, sizeof(head));
	for(i=1;i<=n;i++)
	{
		add(n+1, i, m);
		add(i, n+1, 0);
		add(i, n+2, m + 2.0 * g - degs[i]);
		add(n+2, i, 0);
		for(j=ehead[i];j>=0;j=enext[j])
		{
			add(i, medge[j], 1.00);
			add(medge[j], i, 0);
		}
	}
}

double bisearch()
{
	double left = 0, right = m, esp = 1.0 / n / n / 2.0, mid, cur;

	while(right - left > esp)
	{
		mid = (left + right) / 2.0;
		build_graph(mid);
		cur = SAP(n+1, n+2);
		cur = n * m - cur;
		if(cur > 0)
			left = mid;
		else
			right = mid;
	}
	build_graph(left);  //最后盖满流量的点要是选择的点
	cur = SAP(n+1, n+2);

	return cur;
}

void DFS(int cur)
{
	int j, v;

	vis[cur] = true;
	//for(j=ehead[cur];j>=0;j=enext[j])
	for(j=head[cur];j!=-1;j=edge[j].next)
	{
		v = edge[j].v;
		if((edge[j].cap > 0)&&(!vis[v]))
		{
			dfs[cnt] = v;
			cnt++;
			DFS(v);
		}
	}
}

int comp(const void *a, const void *b)
{
	return (*((int*)a) - *((int*)b));
}

int main()
{
	int i;

	while(input())
	{
		if(n == 0)
		{
			printf("0\n");
			continue;
		}
		if(m == 0)
		{
			printf("1\n1\n");
			continue;
		}
		bisearch();
		cnt = 0;
		memset(vis, 0, sizeof(vis));
		DFS(n+1);
		qsort(dfs, cnt, sizeof(int), comp);
		if(cnt <= 0)
		{
			printf("1\n1\n");
		}else{
			printf("%d\n",cnt);
			for(i=0;i<cnt;i++)
			{
				printf("%d\n",dfs[i]);
			}
		}
	}
	return 0;
}