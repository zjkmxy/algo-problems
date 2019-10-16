/*
描述：
算法实验室k人，每个人有ai句语录，并且形成了m条固定的互黑关系。
现在要用n句语录编成一个n元环，问有几种方案，要求：
1.在顺时针方向上，每个人的话的下一个一定是被他黑过的人的语录，前一个一定是黑过他的人的语录。
2.旋转之后相等的，只能算一种。
3.每个人都不会黑自己，所以相邻的语录一定不是同一个人的。

输入：
多组测试用例，每组第一行为n,m,k
接下来一行k个数，分别表示每个人（1-k号）的语录个数
接下来m行，每行两个数a,b，表示a曾经黑过b
所有数据范围为(0~10^4)

输出：
满足条件的方案数模9973的值

示例输入：
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

示例输出：
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

long nmod[32];        //n快速取幂用
long fcnt[100];       //n各个素因子幂次
long faci[100];       //n素因子
long fval[100];       //n素因子积
long nfac;            //n素因子个数
SINT ans;             //最终答案

#define MODN 9973
//#define MODN 1000000007
#define MAXN 10005
int n, m, k;             //k个人，n元环,m条边
int arr[MAXN];
int e_head[MAXN];
int e_next[MAXN];
int edge[MAXN];

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
		BFS();
		DFS(0, 1, 1);
		ans *= Divisor(n, MODN);
		ans %= MODN;
		printf("%ld\n",ans);
	}
	return 0;
}






/*
加强版

1.在顺时针或者逆时针方向上，每个人的话的下一个一定是被他黑过的人的语录，前一个一定是黑过他的人的语录。
2.旋转或者翻转之后相等的，只能算一种。
3.每个人都不会黑自己，所以相邻的语录一定不是同一个人的。
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

long nmod[32];        //n快速取幂用
long fcnt[100];       //n各个素因子幂次
long faci[100];       //n素因子
long fval[100];       //n素因子积
long nfac;            //n素因子个数
SINT ans;             //最终答案

#define MODN 9973
//#define MODN 1000000007
#define MAXN 10005
int n, m, k;             //k个人，n元环,m条边
int arr[MAXN];
int graph[MAXN][MAXN];

typedef struct _DATA
{
	int start;  //起点
	int end;    //终点
	SINT cnt;    //种类数
	int step;   //步数
	bool dbl;   //双向
}DATA, *PDATA;
SINT bfs_sum[MAXN];    //记录各个和的数组
SINT bfs_link[MAXN];   //记录双项链和

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
		cur.cnt = 1;     //最后会再次乘一遍
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
