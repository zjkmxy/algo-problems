#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 100005
typedef long long int LL;
int arr[MAXN], n, k;
int que[MAXN], front, back;

int GetInt(){/////////////////这里是读入外挂（时间优化巨大）
    char ch=getchar();
    while(ch<'0'||ch>'9')ch=getchar();
    int num=0;
    while(ch>='0'&&ch<='9'){
        num=num*10+ch-'0';
        ch=getchar();
    }
    return num;
}

bool input()
{
	int i;

	if(scanf("%d%d",&n,&k)==EOF)
		return false;
	for(i=1;i<=n;i++)
	{
		//scanf("%d",&arr[i]);
		//arr[i] += arr[i-1];
		arr[i] = GetInt() + arr[i-1];
	}
	return true;
}

inline bool CompPoint(int a, int b, int p)
{
	LL c, d;
	c = (LL)(a) * (LL)(arr[p]) - (LL)(p - b) * (LL)(arr[a]);
	d = (LL)b * (LL)arr[p] - (LL)(p - a) * (LL)arr[b];
	return c <= d;
}

inline bool Comp2(int x, int y, int z)
{
	long long int c, d;
	c = (LL)(arr[y] - arr[x]) * (LL)(z - y);
	d = (LL)(arr[z] - arr[y]) * (LL)(y - x);
	return c >= d;
}

void solve()
{
	int i;
	double mv = 0.00;

	front = back = 0;
	que[0] = 0;
	for(i=k;i<=n;i++)
	{
		//选择最优
		while(front < back && CompPoint(que[front], que[front+1], i))
		{
			front++;
		}
		mv = max(mv, (double)(arr[i] - arr[que[front]]) / (double)(i - que[front]));
		//凸性维护
		while(front < back && Comp2(que[back-1], que[back], i - k + 1))
		{
			back--;
		}
		back++;
		que[back] = i - k + 1;
	}
	printf("%.2lf\n",mv);
}

int main()
{
	while(input())
	{
		solve();
	}
	return 0;
}
