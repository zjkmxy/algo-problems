#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

#define MAXN 100005

typedef long long int LLI;
const LLI MAXI = 0x3fffffffffffffff;

int N, arr[MAXN];
LLI M, sum[MAXN], dp[MAXN];
int queue[MAXN], front, back;

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

void pop(int i)
{
	while(back >= front && sum[i] - sum[queue[front]-1] > M)
		front++;
	return ;
}

int main()
{
	int i, j, k;

	scanf("%d %lld", &N, &M);
	for(i=1;i<=N;i++)
	{
		scanf("%d",&arr[i]);
		sum[i] = sum[i-1] + arr[i];
	}
	j = 1;
	clear();
	for(i=1;i<=N;i++)
	{
		if(arr[i] > M)
		{
			printf("-1\n");
			return 0;
		}
		push(i);
		while(sum[i] - sum[j-1] > M)
		{
			j++;
		}
		pop(i);
		dp[i] = dp[j-1] + arr[queue[front]];
		for(k=front;k<back;k++)
		{
			dp[i] = min(dp[i], dp[queue[k]] + arr[queue[k+1]]);
		}
	}
	printf("%lld\n", dp[N]);

	return 0;
}
