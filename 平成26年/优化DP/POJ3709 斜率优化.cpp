#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<ctime>
#include<algorithm>
using namespace std;

#define MAXN 500005
typedef long long int INT;  //和会超int
INT arr[MAXN], sum[MAXN], dp[MAXN];
int n, k, queue[MAXN], front, back;

/*
dp[i]=min{dp[j]+sum[i]-sum[j]-a[j+1]*(i-j)}
假设决策j1<j2并且j2优于（或者不差于）j1，那么
f[j1]+sum[i]-sum[j1]-a[j1+1]*(i-j1) >= f[j2]+sum[i]-sum[j2]-a[j2+1]*(i-j2)
化简得：[(f[j1]-sum[j1]+a[j1+1]*j1) - (f[j2]-sum[j2]+a[j2+1])] >= i*(a[j1+1]-a[j2+1])。
因为有序序列嘛，a[j2+1]>=a[j1+1]， 所以a[j1+1]-a[j2+1] <= 0。
所以也可以写成：[(f[j1]-sum[j1]+a[j1+1]*j1) - (f[j2]-sum[j2]+a[j2+1])] / (a[j1+1]-a[j2+1]) <= i。
所以，如果对于决策j1,j2满足上述表达式，则j2优于j1。但反之j2不一定劣于j1，因为i是递增的。

首先刚开始队首元素为0——很明显~~
然后假设队列首尾指针head < tail 并且dy(queue[head],queue[head+1]) >= i*dx(queue[head],queue[head+1])，
那么队首元素直接丢掉就可以了。因为i是递增的，如果当前queue[head]没有 queue[head+1]好，那么今后也不会。

队尾的操作要稍微难理解一点，不是那么直观：
因为对于队尾的2个原素x, y来说，如果对于当前i，y比x要烂，那么由前面的证明：
对于比较大的i，y不一定就比x烂，有可能比x好呢。那么对这种情况看来不好处理，
但是我们来看 看队尾3个元素的情况：x,y,z，如果dy(x,y)/dx(x,y)>=dy(y,z)/dx(y,z)，
那么可以直接把y给删了。因为 dy(x,y)/dx(x,y)和dy(y,z)/dx(y,z)是个常数，对于某个i，
如果dy(x,y)/dx(x,y)<=i的话，那么 dy(y,z)/dx(y,z)一定也小于等于i，也就是说：
如果y优于x，那么z一定优于y，这个时候留着y就没用了。。。。直接删了。。。
*/

INT gety(int i)
{
	return dp[i] - sum[i] + arr[i+1]*i;
}

INT getx(int i)
{
	return arr[i+1];
}

void input()
{
	int i;

	scanf("%d%d",&n,&k);
	sum[0] = 0;
	for(i=1;i<=n;i++)
	{
		scanf("%lld",&arr[i]);
		sum[i] = sum[i-1] + arr[i];
	}
}

void domain()
{
	int i, j, x, y, z;

	front = back = 0;
	queue[0] = 0;
	dp[0] = 0;
	for(i=1;i<=n;i++)
	{
		//前置剔除
		while(front < back && (gety(queue[front]) - gety(queue[front+1])) >= i * (getx(queue[front]) - getx(queue[front+1])))
		{
			front++;
		}
		//最优选择
		j = queue[front];
		dp[i] = dp[j] + sum[i] - sum[j] - arr[j+1] * (i - j);
		//后面加入元素和凸性保持
		z = i - k + 1;
		if(z >= k)
		{
			while(front < back)
			{
				x = queue[back-1];
				y = queue[back];
				if((gety(x) - gety(y)) * (getx(y) - getx(z)) >= (getx(x) - getx(y)) * (gety(y) - gety(z)))
					back--;
				else
					break;
			}
			back++;
			queue[back] = z;
		}
	}
	printf("%lld\n",dp[n]);
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		input();
		domain();
	}
	return 0;
}
