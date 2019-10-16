/*
POJ2888 Magic Bracelet

Polya定理不好使了，直接用Burnside引理解决
详细解说请参见getd的注释
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;

typedef signed long long int SINT;

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

long nmod[32];        //n快速取幂用
long fcnt[100];       //n各个素因子幂次
long faci[100];       //n素因子
long fval[100];       //n素因子积
long nfac;            //n素因子个数
SINT ans;             //最终答案

#define MAXM 11
#define MODN 9973
int n, m, k;
int mat[32][MAXM][MAXM];  //mat[i]=关系矩阵的2^i次幂

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

//快速矩阵幂预备
void mul(int mata[MAXM][MAXM], int matb[MAXM][MAXM], int prod[MAXM][MAXM])
{
	int i, j, k;
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
		{
			prod[i][j] = 0;
			for(k=0;k<m;k++)
			{
				prod[i][j] += mata[i][k] * matb[k][j];
				//prod[i][j] %= MODN;  这里不注释掉的话会跑得很慢……
			}
			prod[i][j] %= MODN;
		}
	}
}
void matpow_init()
{
	int i;
	for(i=1;i<32;i++)
	{
		mul(mat[i-1], mat[i-1], mat[i]);
	}
}
void mati(int ret[MAXM][MAXM])
{
	int i, j;
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
		{
			ret[i][j] = (i==j);
		}
	}
}

/*
核心函数getd：计算在变换f有cnt个循环节的时候，我们得到的不变着色方案数D(f)
也就是——有cnt个物品相邻组成链，不考虑任何变换，我们能得到的可行方案数
假设以ai开头的链环ai-b1-b2-...-bq-ai满足条件，那么必定有关系矩阵
g[ai,b1]*g[b1,b2]*...*g[bq,ai]=1
于是以ai开头的可行链环的个数就是关系矩阵g的k次幂中对角元素gk[ai,ai]的值
于是我们用矩阵快速幂计算
*/
int getd(int cnt)  
{
	int temp[2][MAXM][MAXM], i, cur = 0, ret = 0;

	mati(temp[cur]);
	for(i=0;cnt>0;i++,cnt>>=1)
	{
		if(cnt & 1)
		{
			mul(temp[cur], mat[i], temp[cur^1]);
			cur ^= 1;
		}
	}
	for(i=0;i<m;i++)
	{
		ret += temp[cur][i][i];
		//ret %= MODN;  //这里也是效率问题
	}
	return ret;
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
		ans += (getd(fc) * (SINT)phi) % MODN;
		ans %= MODN;
	}
}

int main()
{
	int t, i, j, a, b;

	scanf("%d",&t);
	init_prime();
	while(t--)
	{
		scanf("%d%d%d",&n, &m, &k);
		ans = 0;
		fact(n);
		for(i=0;i<m;i++)
		{
			for(j=0;j<m;j++)
			{
				mat[0][i][j] = 1;
			}
		}
		for(i=0;i<k;i++)
		{
			scanf("%d%d",&a,&b);
			a--;
			b--;
			mat[0][a][b] = mat[0][b][a] = 0;
		}
		matpow_init();
		DFS(0, 1, 1);
		ans *= Divisor(n, MODN);
		ans %= MODN;
		printf("%lld\n",ans);
	}
	return 0;
}
