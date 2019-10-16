#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef long long LL;
#define MAXN	10
#define MAXS	1024
#define MAVS	252
int N, M, P;
int c_map[MAXN];	//人配给猫的违例
int d_map[MAXN];	//猫配给狗的违例
int state[MAVS];	//猫配给状态
int pcnt[MAVS];		//猫配给个数
int nst;
LL dp[MAXN+1][MAXS];

int bitcnt(unsigned int x)
{
	int j, t;
	unsigned int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};
	for(j = 0, t = 1; j < 5; j ++, t <<= 1)
		x = (x & b[j]) + ((x >> t) & b[j]);
	return x;
}

void enume()
{
	int st;	//猫的状态

	nst = 0;
	for(st=0;st<(1<<M);st++)
	{
		if(bitcnt(st) == N)
		{
			state[nst++] = st;
		}
	}
}

//计数每种猫的状态下有多少种方法
void dp_dog(int pos)
{
	int cst, dst;	//猫狗的状态
	int i, j;
	cst = state[pos];
	memset(dp, 0, sizeof(dp));
	dp[0][0] = 1;
	for(i=1;i<=M;i++)
	{
		//这只猫不要
		if((cst & (1<<(i-1))) == 0)
		{
			for(dst=0;dst<(1<<P);dst++)
				dp[i][dst] = dp[i-1][dst];
			continue;
		}
		for(dst=0;dst<(1<<P);dst++)
		{
			if(dp[i-1][dst] == 0)
				continue;
			//枚举狗
			for(j=0;j<P;j++)
			{
				//用了吗？能选吗？
				if((dst & (1<<j)) || (d_map[i-1] & (1<<j)))
					continue;
				dp[i][dst | (1<<j)] += dp[i-1][dst];
			}
		}
	}
	pcnt[pos] = 0;
	for(dst=0;dst<(1<<P);dst++)
	{
		pcnt[pos] += dp[M][dst];
	}
}

//计数有多少种方法
LL dp_cat()
{
	int i, cst, j;
	LL ans;
	memset(dp, 0, sizeof(dp));
	dp[0][0] = 1;
	for(i=1;i<=N;i++)
	{
		for(cst=0;cst<(1<<M);cst++)
		{
			if(dp[i-1][cst] == 0)
				continue;
			//枚举猫
			for(j=0;j<M;j++)
			{
				//用了吗？能选吗？
				if((cst & (1<<j)) || (c_map[i-1] & (1<<j)))
					continue;
				dp[i][cst | (1<<j)] += dp[i-1][cst];
			}
		}
	}
	ans = 0;
	for(j=0;j<nst;j++)
	{
		ans += LL(dp[N][state[j]]) * LL(pcnt[j]);
	}
	return ans;
}

bool domain()
{
	int i, Q, a, b;
	if(scanf("%d%d%d%d",&N,&M,&P,&Q)==EOF)
		return false;
	memset(c_map, 0, sizeof(c_map));
	memset(d_map, 0, sizeof(d_map));
	for(i=0;i<Q;i++)
	{
		scanf("%d%d",&a,&b);
		if(a <= N)
		{
			//人猫关系
			a--;
			b -= N + 1;
			c_map[a] |= (1 << b);
		}else{
			//猫狗关系
			a -= N + 1;
			b -= N + M + 1;
			d_map[a] |= (1 << b);
		}
	}
	if(M < N || P < N)
	{
		printf("0\n");
		return true;
	}
	enume();
	for(i=0;i<nst;i++)
		dp_dog(i);
	printf("%lld\n",dp_cat());

	return true;
}

int main()
{
	while(domain());
	return 0;
}
