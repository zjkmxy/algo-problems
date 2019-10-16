#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<ctime>
#include<algorithm>
using namespace std;

#define MAXN 500005
typedef long long int INT;  //�ͻᳬint
INT arr[MAXN], sum[MAXN], dp[MAXN];
int n, k, queue[MAXN], front, back;

/*
dp[i]=min{dp[j]+sum[i]-sum[j]-a[j+1]*(i-j)}
�������j1<j2����j2���ڣ����߲����ڣ�j1����ô
f[j1]+sum[i]-sum[j1]-a[j1+1]*(i-j1) >= f[j2]+sum[i]-sum[j2]-a[j2+1]*(i-j2)
����ã�[(f[j1]-sum[j1]+a[j1+1]*j1) - (f[j2]-sum[j2]+a[j2+1])] >= i*(a[j1+1]-a[j2+1])��
��Ϊ���������a[j2+1]>=a[j1+1]�� ����a[j1+1]-a[j2+1] <= 0��
����Ҳ����д�ɣ�[(f[j1]-sum[j1]+a[j1+1]*j1) - (f[j2]-sum[j2]+a[j2+1])] / (a[j1+1]-a[j2+1]) <= i��
���ԣ�������ھ���j1,j2�����������ʽ����j2����j1������֮j2��һ������j1����Ϊi�ǵ����ġ�

���ȸտ�ʼ����Ԫ��Ϊ0����������~~
Ȼ����������βָ��head < tail ����dy(queue[head],queue[head+1]) >= i*dx(queue[head],queue[head+1])��
��ô����Ԫ��ֱ�Ӷ����Ϳ����ˡ���Ϊi�ǵ����ģ������ǰqueue[head]û�� queue[head+1]�ã���ô���Ҳ���ᡣ

��β�Ĳ���Ҫ��΢�����һ�㣬������ôֱ�ۣ�
��Ϊ���ڶ�β��2��ԭ��x, y��˵��������ڵ�ǰi��y��xҪ�ã���ô��ǰ���֤����
���ڱȽϴ��i��y��һ���ͱ�x�ã��п��ܱ�x���ء���ô����������������ô���
������������ ����β3��Ԫ�ص������x,y,z�����dy(x,y)/dx(x,y)>=dy(y,z)/dx(y,z)��
��ô����ֱ�Ӱ�y��ɾ�ˡ���Ϊ dy(x,y)/dx(x,y)��dy(y,z)/dx(y,z)�Ǹ�����������ĳ��i��
���dy(x,y)/dx(x,y)<=i�Ļ�����ô dy(y,z)/dx(y,z)һ��ҲС�ڵ���i��Ҳ����˵��
���y����x����ôzһ������y�����ʱ������y��û���ˡ�������ֱ��ɾ�ˡ�����
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
		//ǰ���޳�
		while(front < back && (gety(queue[front]) - gety(queue[front+1])) >= i * (getx(queue[front]) - getx(queue[front+1])))
		{
			front++;
		}
		//����ѡ��
		j = queue[front];
		dp[i] = dp[j] + sum[i] - sum[j] - arr[j+1] * (i - j);
		//�������Ԫ�غ�͹�Ա���
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
