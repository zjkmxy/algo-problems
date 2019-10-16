#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 200003
#define LOGMN 30

int n, q;
//表示某顶点在欧拉序列中初次和最后出现的位置
int pfirst[MAXN], plast[MAXN];
//顶点权值，边集，1起始，
int weight[MAXN], ehead[MAXN], edge[MAXN<<1], enext[MAXN<<1];
//对应边是不是出边
bool eout[MAXN<<1];
//欧拉序列，伪深度序列（父小于所有子），深度对应节点，1起始
int euler[MAXN<<1], depth[MAXN<<1], anc[MAXN<<1], cnt, sig;
//RMQ数组，1起始
int rmqarr[MAXN][LOGMN];
double lg2;
//倍增思想：父亲节点，i到第2^j个父亲之间的最大权值，最小权值，最大取得，最小取得
int fa[MAXN][LOGMN], dpmax[MAXN][LOGMN], dpmin[MAXN][LOGMN], dpmaxmake[MAXN][LOGMN], dpminmake[MAXN][LOGMN], deep[MAXN];

//得到欧拉序列
void DFS(int st, int par, int dep)
{
	int i, now;

	cnt++;
	euler[cnt] = st;
	pfirst[st] = cnt;
	deep[st] = dep;
	fa[st][0] = par;
	sig++;
	now = sig;
	anc[now] = st;
	depth[cnt] = now;
	for(i=ehead[st];i>0;i=enext[i])
	{
		if(edge[i] == par)
			continue;
		eout[i] = true;
		DFS(edge[i], st, dep + 1);
		cnt++;
		euler[cnt] = st;
		depth[cnt] = now;
	}
	plast[st] = cnt;
}

//倍增计算
void EXP(int st)
{
	int i;

	if(fa[st][0] >= 0)
	{
		dpmax[st][0] = max(weight[st], weight[fa[st][0]]);
		dpmin[st][0] = min(weight[st], weight[fa[st][0]]);
		dpmaxmake[st][0] = weight[fa[st][0]] - weight[st];
		dpminmake[st][0] = weight[fa[st][0]] - weight[st];
	}else{
		dpmax[st][0] = weight[st];
		dpmin[st][0] = weight[st];
		dpmaxmake[st][0] = 0;
		dpminmake[st][0] = 0;
	}
	for(i=1;i<LOGMN;i++)
	{
		if(fa[st][i-1] >= 0)
		{
			fa[st][i]=fa[fa[st][i-1]][i-1];
		}
		if(fa[st][i] == -1)
			continue;
		dpmax[st][i] = max(dpmax[st][i-1], dpmax[fa[st][i-1]][i-1]);
		dpmin[st][i] = min(dpmin[st][i-1], dpmin[fa[st][i-1]][i-1]);
		dpmaxmake[st][i] = max(dpmaxmake[st][i-1], dpmaxmake[fa[st][i-1]][i-1]);
		dpmaxmake[st][i] = max(dpmaxmake[st][i], dpmax[fa[st][i-1]][i-1] - dpmin[st][i-1]);
		dpminmake[st][i] = min(dpminmake[st][i-1], dpminmake[fa[st][i-1]][i-1]);
		dpminmake[st][i] = min(dpminmake[st][i], dpmin[fa[st][i-1]][i-1] - dpmax[st][i-1]);
	}
	for(i=ehead[st];i>0;i=enext[i])
	{
		if(!eout[i])
			continue;
		EXP(edge[i]);
	}
}

//RMQ初始化
void rmqst_init()
{
	int i,j,m;

	lg2 = log(2.00);
	m = (int)(floor(log((double)cnt)/lg2));
	for(i=1;i<=cnt;i++)
	{
		rmqarr[i][0] = depth[i];
	}
	for(j=1;j<=m;j++)
	{
		for(i=1;i<=cnt-(1<<(j-1));i++)
		{
			rmqarr[i][j]=min(rmqarr[i][j-1],rmqarr[i + (1<<(j-1))][j-1]);
		}
	}
}

//RMQ计算
int rmq_get(int a, int b)
{
	int m, tmp;

	if(b < a)
	{
		tmp = a;
		a = b;
		b = tmp;
	}
	m = (int)(floor(log((double)(b-a+1))/lg2));
	return min(rmqarr[a][m], rmqarr[b-(1<<m)+1][m]);
}

void get_max_min(int a, int b, int &Max, int &Min, int &maxmake, int &minmake)
{
	int bb[2][LOGMN], i, j;
    bool flag[LOGMN];

	Max = weight[a];
	Min = weight[a];
	maxmake = weight[b] - weight[a];
	minmake = weight[b] - weight[a];
	memset(flag,0,sizeof(flag));
	for(int i=deep[a]-deep[b],j=0;i;i>>=1,j++)
	{
		if(i & 1)
		{
			Max = max(Max, dpmax[a][j]);
			Min = min(Min, dpmin[a][j]);
			maxmake = max(maxmake, dpmaxmake[a][j]);
			minmake = min(minmake, dpminmake[a][j]);
			bb[1][j]=dpmin[a][j];
			bb[0][j]=dpmax[a][j];
			flag[j]=true;
			a=fa[a][j];
		}
	}
	for(int i=LOGMN-1;i>0;i--)
	{
        for(int j=0;j<i;j++)
		{
			if(flag[i]&&flag[j])
			{
				maxmake = max(maxmake, bb[0][i]-bb[1][j]);
				minmake = min(minmake, bb[1][i]-bb[0][j]);
			}
		}
	}
}

//计算结果
int calc(int a, int b)
{
	int lca;
	int aa = pfirst[a], bb = pfirst[b];
	int Min[2], Max[2], maxmake[2], minmake[2], ans = 0;

	if(a == b)
		return 0;
	if(aa > bb)
	{
		//swap(a, b);
		swap(aa, bb);
	}
	lca = anc[rmq_get(aa, bb)];
	get_max_min(a, lca, Max[0], Min[0], maxmake[0], minmake[0]);
	get_max_min(b, lca, Max[1], Min[1], maxmake[1], minmake[1]);
	ans = max(ans, maxmake[0]);
	ans = max(ans, -minmake[1]);
	ans = max(ans, Max[1] - Min[0]);
	return ans;
}

int main()
{
	int i, a, b;

	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		scanf("%d",&weight[i]);
	}
	for(i=1;i<n;i++)
	{
		scanf("%d%d",&a,&b);
		edge[i<<1] = a;
		edge[(i<<1)-1] = b;
		enext[i<<1] = ehead[b];
		enext[(i<<1)-1] = ehead[a];
		ehead[b] = (i<<1);
		ehead[a] = (i<<1)-1;
	}

	memset(fa, -1, sizeof(fa));
	DFS(1, -1, 0);
	EXP(1);
	rmqst_init();

	scanf("%d",&q);
	for(i=1;i<=q;i++)
	{
		scanf("%d%d",&a,&b);
		printf("%d\n",calc(a, b));
	}

	return 0;
}
