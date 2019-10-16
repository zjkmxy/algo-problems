#include<cstdio>
#include<cstdlib>
#include<cstring>

#define MAXN 2005
#define MAXM 4005
#define MAXI 0x0fffffff

int n, m, ecnt;
int ehead[MAXN], edge[MAXM], enext[MAXM];
int dplamp[MAXN][2], dpsinge[MAXN][2]; //某节点放与不放各自需要的灯数目，单边数目
bool vis[MAXN];

void addedge(int a, int b)
{
	edge[ecnt] = b;
	enext[ecnt] = ehead[a];
	ehead[a] = ecnt;
	ecnt++;

	edge[ecnt] = a;
	enext[ecnt] = ehead[b];
	ehead[b] = ecnt;
	ecnt++;
}

void init()
{
	int i;

	ecnt = 0;
	for(i=0;i<n;i++)
	{
		vis[i] = false;
		ehead[i] = -1;
	}
}

void dp_calc(int pos, int par)
{
	int i, ed;

	vis[pos] = true;
	dplamp[pos][0] = 0;
	dplamp[pos][1] = 1;
	dpsinge[pos][0] = 0;
	dpsinge[pos][1] = 0;
	for(i=ehead[pos];i>=0;i=enext[i])
	{
		ed = edge[i];
		if(ed == par)
			continue;
		if(!vis[ed])
			dp_calc(ed, pos);

		dplamp[pos][0] += dplamp[ed][1];
		dpsinge[pos][0] += dpsinge[ed][1] + 1;

		if((dplamp[ed][1] > dplamp[ed][0]) || (dplamp[ed][1] == dplamp[ed][0] && dpsinge[ed][1] > dpsinge[ed][0] + 1))
		{
			dplamp[pos][1] += dplamp[ed][0];
			dpsinge[pos][1] += dpsinge[ed][0] + 1;
		}else{
			dplamp[pos][1] += dplamp[ed][1];
			dpsinge[pos][1] += dpsinge[ed][1];
		}
	}
}

void input()
{
	int i, a, b;

	scanf("%d%d",&n,&m);
	init();
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&a,&b);
		addedge(a, b);
	}
}

void domain()
{
	int i, cnt = 0, sing = 0;

	for(i=0;i<n;i++)
		if(!vis[i])
		{
			dp_calc(i, -1);
			if((dplamp[i][1] > dplamp[i][0]) || (dplamp[i][1] == dplamp[i][0] && dpsinge[i][1] > dpsinge[i][0]))
			{
				cnt += dplamp[i][0];
				sing += dpsinge[i][0];
			}else{
				cnt += dplamp[i][1];
				sing += dpsinge[i][1];
			}
		}
	printf("%d %d %d\n",cnt,m-sing,sing);
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		input();
		domain();
	}
	return 0;
}
