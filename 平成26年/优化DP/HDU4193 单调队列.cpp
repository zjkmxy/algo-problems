#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 2000005
int sum[MAXN], n, queue[MAXN];
int front, back, ans;

void push(int i)
{
	while(front <= back && sum[queue[back]] >= sum[i])  //维护单调
		back--;
	back++;
	queue[back] = i;
}

void pop(int i)  //如果区间中最小的元素都大于被减和，那么全部和都大于被减和
{
	if(queue[front] <= i)
		front++;
	if(sum[queue[front]] >= sum[i])
		ans++;
}

bool input()
{
	int i;

	scanf("%d",&n);
	if(n == 0)
		return false;
	for(i=1;i<=n;i++)
	{
		scanf("%d",&sum[i]);
		sum[i+n] = sum[i];
	}
	for(i=2;i<=n*2;i++)
	{
		sum[i] += sum[i-1];
	}
	return true;
}

int main()
{
	int i;

	while(input())
	{
		ans = 0;
		front = 0;
		back = -1;
		for(i=1;i<n;i++)
		{
			push(i);
		}
		for(i=n;i<2*n;i++)
		{
			push(i);
			pop(i - n);
		}
		printf("%d\n",ans);
	}
	return 0;
}
