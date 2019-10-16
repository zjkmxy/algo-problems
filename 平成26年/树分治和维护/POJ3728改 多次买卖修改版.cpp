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
//��ʾĳ������ŷ�������г��κ������ֵ�λ�ã������о���
int pfirst[MAXN], plast[MAXN], dis1[MAXN], dis2[MAXN];
//����Ȩֵ���߼���1��ʼ��
int weight[MAXN], ehead[MAXN], edge[MAXN<<1], enext[MAXN<<1];
//��Ӧ���ǲ��ǳ���
bool eout[MAXN<<1];
//ŷ�����У�α������У���С�������ӣ�����ȶ�Ӧ�ڵ㣬1��ʼ
int euler[MAXN<<1], depth[MAXN<<1], anc[MAXN<<1], cnt, sig;
//RMQ���飬1��ʼ
int rmqarr[MAXN][LOGMN];
double lg2;

//�õ�ŷ������
void DFS(int st, int dist1, int dist2, int par)
{
	int i, now, w;

	cnt++;
	euler[cnt] = st;
	pfirst[st] = cnt;
	dis1[st] = dist1;
	dis2[st] = dist2;
	sig++;
	now = sig;
	anc[now] = st;
	depth[cnt] = now;
	for(i=ehead[st];i>0;i=enext[i])
	{
		if(edge[i] == par)
			continue;
		eout[i] = true;
		w = weight[edge[i]] - weight[st];
		if(w > 0)
			DFS(edge[i], dist1, dist2 + w, st);
		else
			DFS(edge[i], dist1 - w, dist2, st);
		cnt++;
		euler[cnt] = st;
		depth[cnt] = now;
	}
	plast[st] = cnt;
}

//RMQ��ʼ��
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

//RMQ����
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

//������
int calc(int a, int b)
{
	int lca, da, db;
	int aa = pfirst[a], bb = pfirst[b];

	if(a == b)
		return 0;
	if(aa > bb)
	{
		//swap(a, b);
		swap(aa, bb);
	}
	lca = anc[rmq_get(aa, bb)];
	da = dis1[a] - dis1[lca];
	db = dis2[b] - dis2[lca];
	return (da + db);
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

	DFS(1, 0, 0, -1);
	rmqst_init();

	scanf("%d",&q);
	for(i=1;i<=q;i++)
	{
		scanf("%d%d",&a,&b);
		printf("%d\n",calc(a, b));
	}

	return 0;
}
