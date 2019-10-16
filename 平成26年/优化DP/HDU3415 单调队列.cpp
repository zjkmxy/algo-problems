#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<ctime>
using namespace std;

#define MAXN 200008
#define MAXI 0x0fffffff

int queue[MAXN], back, front;
int sum[MAXN];
int ans, st, len;
int n, k;

void push(int i)
{
	while(sum[queue[back]] >= sum[i] && front <= back)
	{
		back--;
	}
	back++;
	queue[back] = i;
}

void pop(int i)
{
	int s, j;
	bool flag = false;

	if(queue[front] < i - k)
		front++;
	j = queue[front];
	s = sum[i] - sum[j];
	if(s > ans)
	{
		flag = true;
	}else if(s == ans && (j + 1) < st)
	{
		flag = true;
	}else if(s == ans && (j + 1) == st && (i - j) < len)
	{
		flag = true;
	}
	if(flag)
	{
		ans = s;
		st = j + 1;
		len = i - j;
	}
}

void domain()
{
	int i;

	ans = -MAXI;
	front = 0;
	back = -1;
	scanf("%d%d",&n,&k);
	for(i=1;i<=n;i++)
	{
		scanf("%d",&sum[i]);
		sum[i+n] = sum[i];
	}
	n <<= 1;
	for(i=2;i<=n;i++)
	{
		sum[i] += sum[i-1];
	}
	push(0);
	for(i=1;i<=n;i++)
	{
		pop(i);
		push(i);
	}
	n >>= 1;
	printf("%d %d %d\n",ans,st,(st+len-1 > n)?(st + len - n - 1):(st + len - 1));
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
