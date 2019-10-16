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

long nmod[32];        //n快速取幂用
long fcnt[100];       //n各个素因子幂次
long faci[100];       //n素因子
long fval[100];       //n素因子积
long nfac;            //n素因子个数
SINT ans;             //最终答案

#define MODN 9973
#define MAXN 65
#define MAXM 805
int n, m, k;             //k个人，n元环,m条边
int arr[MAXN];
int e_head[MAXN];
int e_next[MAXM];
int edge[MAXM];

int augm[2][MAXN][MAXN]; //a->b的路有多少种

typedef struct _DATA
{
	int start;  //起点
	int end;    //终点
	SINT cnt;    //种类数
	int step;   //步数
}DATA, *PDATA;
SINT bfs_sum[MAXN];    //记录各个和的数组

//筛法预处理素数
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

//因子分解
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
	if(x > 1)  //x剩余自身为素数（这个很容易忘）
	{
		faci[nfac] = x;
		fcnt[nfac] = 1;
		fval[nfac] = x;
		nfac++;
	}
}

//扩展欧几里得算法
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

//x对y乘法逆元
SINT Divisor(SINT x, SINT y)
{
	SINT d, ret, tmp;

	d = ExtendGcd(x,y,ret,tmp);
	//不互质则无解
	if(d != 1)  
		return -1;
	//变成最小正
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
		cur.cnt = 1;     //最后会再次乘一遍
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

//起点终点增广法
void BFS2()
{
	int cur;
	int i, j, e, s, ed;

	cur = 1;
	memset(bfs_sum, 0, sizeof(bfs_sum));
	memset(augm[0], 0, sizeof(augm[0]));
	memset(augm[1], 0, sizeof(augm[1]));
	for(i=0;i<k;i++)
	{
		augm[1][i][i] = 1;
	}
	for(s=0;s<=n;s++)
	{
		memset(augm[cur^1], 0, sizeof(augm[0]));
		for(i=0;i<k;i++)
		{
			for(j=0;j<k;j++)
			{
				if(augm[cur][i][j] == 0)
					continue;
				if(i == j)
				{
					bfs_sum[s] += augm[cur][i][j];
					bfs_sum[s] %= MODN;
				}
				for(e=e_head[j];e!=-1;e=e_next[e])
				{
					ed = edge[e];
					augm[cur^1][i][ed] += (augm[cur][i][j] * arr[ed]) % MODN;
				}
			}
		}
		cur ^= 1;
	}
}

void DFS(int pfi, int fc, int phi)  //素因子序号，当前因子，n/i的欧拉函数值
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
		BFS2();
		DFS(0, 1, 1);
		ans *= Divisor(n, MODN);
		ans %= MODN;
		if(n > 1)
		{
			printf("%ld\n",ans);
		}else{
			ans = 0;
			for(i=0;i<k;i++)
			{
				ans += arr[i];
				ans %= MODN;
			}
			printf("%ld\n",ans);
		}
	}
	return 0;
}