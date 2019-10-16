#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 200003
#define LOGMN 30
#define lowbit(x) (x&(-x))

int n, q, s;
//��ʾĳ������ŷ�������г��κ������ֵ�λ��
int pfirst[MAXN], plast[MAXN], dis[MAXN];
//�߼���1��ʼ
int ehead[MAXN], edge[MAXN<<1], enext[MAXN<<1], wedge[MAXN<<1];
//��Ӧ���ǲ��ǳ���
bool eout[MAXN<<1];
//ŷ�����У�α������У���С�������ӣ�����ȶ�Ӧ�ڵ㣬1��ʼ
int euler[MAXN<<1], depth[MAXN<<1], anc[MAXN<<1], cnt, sig;
//��״���飬RMQ���飬1��ʼ
int trearr[MAXN<<1], rmqarr[MAXN][LOGMN];
double lg2;

//�õ�ŷ������
void DFS(int st, int dist, int par)
{
	int i, now;

	cnt++;
	euler[cnt] = st;
	pfirst[st] = cnt;
	dis[st] = dist;
	sig++;
	now = sig;
	anc[now] = st;
	depth[cnt] = now;
	for(i=ehead[st];i>0;i=enext[i])
	{
		if(edge[i] == par)
			continue;
		eout[i] = true;
		DFS(edge[i], dist + wedge[i], st);
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

//��״�������
int getsum(int i)
{
	int sum = 0;
	for(;i>0;i-=lowbit(i))
		sum += trearr[i];
	return sum;
}

//��״�������
void tarr_upd(int i, int val)
{
	for(;i<=cnt;i+=lowbit(i))
		trearr[i] += val;
}

//������
int calc(int a, int b)
{
	int lca, da, db, dc;
	int aa = pfirst[a], bb = pfirst[b];

	if(a == b)
		return 0;
	if(aa > bb)
	{
		swap(a, b);
		swap(aa, bb);
	}
	lca = anc[rmq_get(aa, bb)];
	da = dis[a] + getsum(aa);
	db = dis[b] + getsum(bb);
	dc = dis[lca] + getsum(pfirst[lca]);
	return (da + db - (dc << 1));
}

void update(int a, int w)
{
	a <<= 1;
	w = w - wedge[a];
	wedge[a] += w;
	wedge[a-1] += w;
	if(!eout[a])
		a--;
	a = edge[a];
	tarr_upd(pfirst[a], w);
	tarr_upd(plast[a]+1, -w);
}

int main()
{
	int i, a, b, w;

	scanf("%d%d%d",&n,&q,&s);
	for(i=1;i<n;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		edge[i<<1] = a;
		edge[(i<<1)-1] = b;
		enext[i<<1] = ehead[b];
		enext[(i<<1)-1] = ehead[a];
		ehead[b] = (i<<1);
		ehead[a] = (i<<1)-1;
		wedge[i<<1] = w;
		wedge[(i<<1)-1] = w;
	}

	DFS(1, 0, -1);
	rmqst_init();

	for(i=1;i<=q;i++)
	{
		scanf("%d",&a);
		if(a == 0)
		{
			scanf("%d",&b);
			printf("%d\n",calc(s, b));
			s = b;
		}else{
			scanf("%d%d",&b,&w);
			update(b, w);
		}
	}

	return 0;
}
