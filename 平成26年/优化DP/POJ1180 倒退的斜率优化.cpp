#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

typedef long long LL;
#define MAXN 10005

LL F[MAXN], T[MAXN], FS[MAXN], TS[MAXN];
LL dp[MAXN];
int que[MAXN], front, back;
int N, S;

bool input()
{
	int i;

	if(scanf("%d%d",&N,&S)==EOF)
		return false;

	for(i=1;i<=N;i++)
	{
		scanf("%lld%lld",&T[i],&F[i]);
	}
	FS[N+1] = TS[N+1] = 0;
	dp[N+1] = 0;
	for(i=N;i>=1;i--)
	{
		FS[i] = FS[i+1] + F[i];
		TS[i] = TS[i+1] + T[i];
	}

	return true;
}

inline LL gety(int k)
{
	return dp[k];
}

inline LL getx(int k)
{
	return TS[k] - S;
}

LL dpcalc()
{
	int i, x, y;

	que[0] = N + 1;
	back = front = 0;
	
	for(i=N;i>=1;i--)
	{
		while(front < back && (gety(que[front]) - gety(que[front+1])) >= FS[i] * (getx(que[front]) - getx(que[front+1])))
		{
			front++;
		}
		dp[i] = dp[que[front]] + (TS[i] - TS[que[front]] + S) * FS[i];
		while(front < back)
		{
			x = que[back-1];
			y = que[back];
			if((gety(x) - gety(y)) * (getx(y) - getx(i)) >= (gety(y) - gety(i)) * (getx(x) - getx(y)))
				back--;
			else
				break;
		}
		back++;
		que[back] = i;
	}

	return dp[1];
}

int main()
{
	while(input())
	{
		printf("%lld\n",dpcalc());
	}
	return 0;
}
