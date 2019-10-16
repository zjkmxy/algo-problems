#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<ctime>
#include<queue>
using namespace std;

typedef signed long int SINT;

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

long nmod[32];        //n����ȡ����
long fcnt[100];       //n�����������ݴ�
long faci[100];       //n������
long fval[100];       //n�����ӻ�
long nfac;            //n�����Ӹ���
SINT ans;             //���մ�

#define MODN 9973
#define MAXN 105
#define MAXM 805
int n, m, k;             //k���ˣ�nԪ��,m����
int arr[MAXN];
int graph[MAXN][MAXN];
int e_head[MAXN], edge[MAXM], e_next[MAXM];

typedef struct _DATA
{
	int start;  //���
	int end;    //�յ�
	SINT cnt;    //������
	int step;   //����
	bool dbl;   //˫��
}DATA, *PDATA;
SINT bfs_sum[MAXN];    //��¼�����͵�����
SINT bfs_link[MAXN];   //��¼˫������

int augm[2][MAXN][MAXN][2]; //a->b��·�ж����֣�0��1˫��

//ɸ��Ԥ��������
void init_prime()
{
	int i, j = 0, k = 0;
	for(i=2;k<PRIMENUM;i++)
	{
		if(notprime[i])
			continue;

		for(j=i+i; j < 32000; j+=i)
		{
			notprime[j] = true;
		}
		primes[k] = i;
		k++;
	}
}

//���ӷֽ�
void fact(int x)
{
	int i;

	nfac = 0;
	for(i=0;(i<PRIMENUM)&&(x>1);i++)
	{
		if(x % primes[i] == 0)
		{
			faci[nfac] = primes[i];
			fcnt[nfac] = 0;
			fval[nfac] = 1;
			do{
				x /= primes[i];
				fval[nfac] *= primes[i];
				fcnt[nfac]++;
			}while(x % primes[i] == 0);
			nfac++;
		}
	}
	if(x > 1)  //xʣ������Ϊ�������������������
	{
		faci[nfac] = x;
		fcnt[nfac] = 1;
		fval[nfac] = x;
		nfac++;
	}
}

//��չŷ������㷨
SINT ExtendGcd(SINT a,SINT b,SINT &x,SINT &y)
{
	SINT d,tmp;

	if(b==0)
	{
		x = 1;
		y = 0;
		return a;
	}
	d = ExtendGcd(b,a%b,x,y);
	tmp = x;
	x = y;
	y = tmp - (a/b)*y;

	return d;
}

//x��y�˷���Ԫ
SINT Divisor(SINT x, SINT y)
{
	SINT d, ret, tmp;

	d = ExtendGcd(x,y,ret,tmp);
	//���������޽�
	if(d != 1)  
		return -1;
	//�����С��
	ret = (ret % y + y) % y;
	return ret;
}

void BFS()
{
	queue<DATA> oplst;
	int i, st, ed;
	bool dbl;
	SINT cnt;
	DATA cur;

	memset(bfs_sum, 0, sizeof(int) * (n + 1));
	memset(bfs_link, 0, sizeof(int) * (n + 1));
	cur.step = 0;
	for(i=0;i<k;i++)
	{
		cur.start = cur.end = i;
		cur.cnt = 1;     //�����ٴγ�һ��
		cur.dbl = true;
		oplst.push(cur);
	}
	while(!oplst.empty())
	{
		cur = oplst.front();
		oplst.pop();
		if(cur.start == cur.end)
		{
			if(cur.dbl)
				bfs_sum[cur.step] += cur.cnt;
			else
				bfs_sum[cur.step] += cur.cnt * 2;
			bfs_sum[cur.step] %= MODN;
		}
		if(cur.dbl)
		{
			bfs_link[cur.step+1] += cur.cnt * arr[cur.start];
			bfs_link[cur.step+1] %= MODN;
		}
		cur.step++;
		if(cur.step > n)
			continue;

		cnt = cur.cnt;
		st = cur.end;
		dbl = cur.dbl;
		for(i=e_head[st];i>=0;i=e_next[i])
		{
			ed = edge[i];
			cur.end = ed;
			cur.cnt = (cnt * arr[st]) % MODN;
			cur.dbl = (dbl && graph[ed][st]);
			oplst.push(cur);
		}
	}
}

//����յ����㷨
void BFS2()
{
	int cur;
	int i, j, e, s, ed, pa, pa2;

	cur = 1;
	memset(bfs_sum, 0, sizeof(int) * (n + 1));
	memset(bfs_link, 0, sizeof(int) * (n + 1));
	memset(augm[0], 0, sizeof(augm[0]));
	memset(augm[1], 0, sizeof(augm[1]));
	for(i=0;i<k;i++)
	{
		augm[1][i][i][0] = 0;
		augm[1][i][i][1] = 1;
	}
	for(s=0;s<=n;s++)
	{
		memset(augm[cur^1], 0, sizeof(augm[0]));
		for(i=0;i<k;i++)
		{
			for(j=0;j<k;j++)
			{
				for(pa=0;pa<=1;pa++)
				{
					if(augm[cur][i][j][pa] == 0)
						continue;

					if(i == j)
					{
						bfs_sum[s] += (pa == 0) ? (augm[cur][i][j][pa] * 2) : augm[cur][i][j][pa];
						bfs_sum[s] %= MODN;
					}
					if(pa == 1)
					{
						bfs_link[s+1] += augm[cur][i][j][1] * arr[i];
						bfs_link[s+1] %= MODN;
					}
					for(e=e_head[j];e!=-1;e=e_next[e])
					{
						ed = edge[e];
						if(pa == 0)
						{
							augm[cur^1][i][ed][0] += augm[cur][i][j][0] * arr[ed];
							augm[cur^1][i][ed][0] %= MODN;
						}else{
							pa2 = graph[ed][j] ? 1 : 0;
							augm[cur^1][i][ed][pa2] += augm[cur][i][j][1] * arr[ed];
							augm[cur^1][i][ed][pa2] %= MODN;
						}
					}
				}
			}
		}
		cur ^= 1;
	}
}

void DFS(int pfi, int fc, int phi)  //��������ţ���ǰ���ӣ�n/i��ŷ������ֵ
{
	int i, p;

	if(pfi < nfac)
	{
		for(i=0,p=fval[pfi];i<=fcnt[pfi];i++,p/=faci[pfi])
		{
			DFS(pfi + 1, fc * (fval[pfi] / p), phi * (p - p / faci[pfi]));
		}
	}else{
		ans += (bfs_sum[fc] * (SINT)phi) % MODN;
		ans %= MODN;
	}
}


int main()
{
	int i, a, b;

	init_prime();
	while(scanf("%d%d%d",&n, &m, &k)!=EOF)
	{
		ans = 0;
		fact(n);
		for(i=0;i<k;i++)
		{
			scanf("%d",&arr[i]);
		}
		memset(graph, 0, sizeof(graph));
		memset(e_head, -1, sizeof(int) * k);
		for(i=0;i<m;i++)
		{
			scanf("%d%d",&a,&b);
			a--;
			b--;
			graph[a][b] = true;
			edge[i] = b;
			e_next[i] = e_head[a];
			e_head[a] = i;
		}
		BFS2();
		DFS(0, 1, 1);
		if(n % 2 == 0)
		{
			ans += bfs_link[(n >> 1) + 1] * (n >> 1);
			ans %= MODN;
		}
		ans *= Divisor(2 * n, MODN);
		ans %= MODN;
		if(n == 1)
		{
			ans = 0;
			for(i=0;i<k;i++)
			{
				ans += arr[i];
			}
		}
		printf("%ld\n",ans);
	}
	return 0;
}