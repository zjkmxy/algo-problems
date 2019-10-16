/*
������
�㷨ʵ����k�ˣ�ÿ������ai����¼�������γ���m���̶��Ļ��ڹ�ϵ��
����Ҫ��n����¼���һ��nԪ�������м��ַ�����Ҫ��
1.��˳ʱ�뷽���ϣ�ÿ���˵Ļ�����һ��һ���Ǳ����ڹ����˵���¼��ǰһ��һ���Ǻڹ������˵���¼��
2.��ת֮����ȵģ�ֻ����һ�֡�
3.ÿ���˶�������Լ����������ڵ���¼һ������ͬһ���˵ġ�

���룺
�������������ÿ���һ��Ϊn,m,k
������һ��k�������ֱ��ʾÿ���ˣ�1-k�ţ�����¼����
������m�У�ÿ��������a,b����ʾa�����ڹ�b
�������ݷ�ΧΪ(0~10^4)

�����
���������ķ�����ģ9973��ֵ

ʾ�����룺
3 3 3
6 5 9
1 2
2 3
3 1
4 8 4
2 1 3 4
1 2
1 3
2 3
2 4
3 4
3 1
4 1
4 2

ʾ�������
270
55

*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
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
//#define MODN 1000000007
#define MAXN 10005
int n, m, k;             //k���ˣ�nԪ��,m����
int arr[MAXN];
int e_head[MAXN];
int e_next[MAXN];
int edge[MAXN];

typedef struct _DATA
{
	int start;  //���
	int end;    //�յ�
	SINT cnt;    //������
	int step;   //����
}DATA, *PDATA;
SINT bfs_sum[MAXN];    //��¼�����͵�����

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
	int i;
	SINT cnt;
	DATA cur;

	memset(bfs_sum, 0, sizeof(int) * (n + 1));
	cur.step = 0;
	for(i=0;i<k;i++)
	{
		cur.start = cur.end = i;
		cur.cnt = 1;     //�����ٴγ�һ��
		oplst.push(cur);
	}
	while(!oplst.empty())
	{
		cur = oplst.front();
		oplst.pop();
		if(cur.start == cur.end)
		{
			bfs_sum[cur.step] += cur.cnt;
			bfs_sum[cur.step] %= MODN;
		}
		cur.step++;
		if(cur.step > n)
			continue;

		cnt = cur.cnt;
		for(i=e_head[cur.end];i!=-1;i=e_next[i])
		{
			cur.end = edge[i];
			cur.cnt = (cnt * arr[cur.end]) % MODN;
			oplst.push(cur);
		}
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
		memset(e_head, -1, sizeof(int) * k);
		for(i=0;i<m;i++)
		{
			scanf("%d%d",&a,&b);
			a--;
			b--;
			edge[i] = b;
			e_next[i] = e_head[a];
			e_head[a] = i;
		}
		BFS();
		DFS(0, 1, 1);
		ans *= Divisor(n, MODN);
		ans %= MODN;
		printf("%ld\n",ans);
	}
	return 0;
}






/*
��ǿ��

1.��˳ʱ�������ʱ�뷽���ϣ�ÿ���˵Ļ�����һ��һ���Ǳ����ڹ����˵���¼��ǰһ��һ���Ǻڹ������˵���¼��
2.��ת���߷�ת֮����ȵģ�ֻ����һ�֡�
3.ÿ���˶�������Լ����������ڵ���¼һ������ͬһ���˵ġ�
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
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
//#define MODN 1000000007
#define MAXN 10005
int n, m, k;             //k���ˣ�nԪ��,m����
int arr[MAXN];
int graph[MAXN][MAXN];

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
	int i, st;
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
		for(i=0;i<n;i++)
		{
			if(!graph[st][i])
				continue;
			cur.end = i;
			cur.cnt = (cnt * arr[st]) % MODN;
			cur.dbl = (dbl && graph[i][st]);
			oplst.push(cur);
		}
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
		for(i=0;i<m;i++)
		{
			scanf("%d%d",&a,&b);
			a--;
			b--;
			graph[a][b] = true;
		}
		BFS();
		DFS(0, 1, 1);
		if(n % 2 == 0)
		{
			ans += bfs_link[(n >> 1) + 1] * (n >> 1);
			ans %= MODN;
		}
		ans *= Divisor(2 * n, MODN);
		ans %= MODN;
		printf("%ld\n",ans);
	}
	return 0;
}
