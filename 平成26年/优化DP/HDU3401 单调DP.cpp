#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<ctime>
#include<algorithm>
using namespace std;

#define MAXN 2005
#define MAXI 0x0fffffff

int dp[MAXN][MAXN];  //第i天期货数为j时的结果
int arr[MAXN], queue[MAXN], back, front;  //期货数目的单调减队列
int T, MP, W;
int AP[MAXN], BP[MAXN], AS[MAXN], BS[MAXN];

/*
转移方程
dp[i][j] = max(持平，买入，卖出)
持平：dp[i-1][j]
买入：dp[i-w-1][k]-AP[i]*(j-k)
卖出：dp[i-w-1][k]+BP[i]*(k-j)

以买入为例，取单调减队列维护：
max{dp[i-w-1][k]+AP[i]*k}
对每个j，退AS[i]后取队头值减去AP[i]*j
*/

void clear()
{
	front = 0;
	back = -1;
}

void push(int i)
{
	int val = arr[i];
	while(back >= front && val >= arr[queue[back]])
		back--;
	back++;
	queue[back] = i;
}

int pop1(int i, int k)
{
	while(back >= front && queue[front] < (i - k))  //可以改成if
		front++;
	return arr[queue[front]];
}

int pop2(int i, int k)
{
	while(back >= front && queue[front] > (i + k))  //可以改成if
		front++;
	return arr[queue[front]];
}

int dp_calc()
{
	int i, j, pre;

	for(i=0;i<=T;i++)
		for(j=0;j<=MP;j++)
			dp[i][j] = -MAXI;
	dp[0][0] = 0;
	for(i=1;i<=W+1;i++)
		for(j=0;j<=AS[i];j++)
			dp[i][j] = -AP[i] * j;  //这里必须加，不然WA，目测是数据问题……

	for(i=1;i<=T;i++)
	{
		//持平，初始化
		for(j=0;j<=MP;j++)
			dp[i][j] = max(dp[i][j], dp[i-1][j]);

		pre = i - W - 1;
		if(pre < 0)
			continue ;
		//买入
		for(j=0;j<=MP;j++)
			arr[j] = dp[pre][j] + AP[i] * j;
		clear();
		for(j=0;j<=MP;j++)
		{
			push(j);
			dp[i][j] = max(dp[i][j], pop1(j, AS[i]) - AP[i] * j);
		}
		//卖出
		for(j=0;j<=MP;j++)
			arr[j] = dp[pre][j] + BP[i] * j;
		clear();
		for(j=MP;j>=0;j--)
		{
			push(j);
			dp[i][j] = max(dp[i][j], pop2(j, BS[i]) - BP[i] * j);
		}
	}

	pre = 0;
	for(j=0;j<=MP;j++)
		pre = max(pre, dp[T][j]);

	return pre;
}

void domain()
{
	int i;

	scanf("%d%d%d",&T,&MP,&W);
	for(i=1;i<=T;i++)
	{
		scanf("%d%d%d%d",&AP[i],&BP[i],&AS[i],&BS[i]);
	}
	printf("%d\n",dp_calc());
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		domain();
	}
	return 0;
}
