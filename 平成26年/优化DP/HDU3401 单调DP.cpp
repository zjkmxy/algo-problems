#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<ctime>
#include<algorithm>
using namespace std;

#define MAXN 2005
#define MAXI 0x0fffffff

int dp[MAXN][MAXN];  //��i���ڻ���Ϊjʱ�Ľ��
int arr[MAXN], queue[MAXN], back, front;  //�ڻ���Ŀ�ĵ���������
int T, MP, W;
int AP[MAXN], BP[MAXN], AS[MAXN], BS[MAXN];

/*
ת�Ʒ���
dp[i][j] = max(��ƽ�����룬����)
��ƽ��dp[i-1][j]
���룺dp[i-w-1][k]-AP[i]*(j-k)
������dp[i-w-1][k]+BP[i]*(k-j)

������Ϊ����ȡ����������ά����
max{dp[i-w-1][k]+AP[i]*k}
��ÿ��j����AS[i]��ȡ��ͷֵ��ȥAP[i]*j
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
	while(back >= front && queue[front] < (i - k))  //���Ըĳ�if
		front++;
	return arr[queue[front]];
}

int pop2(int i, int k)
{
	while(back >= front && queue[front] > (i + k))  //���Ըĳ�if
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
			dp[i][j] = -AP[i] * j;  //�������ӣ���ȻWA��Ŀ�����������⡭��

	for(i=1;i<=T;i++)
	{
		//��ƽ����ʼ��
		for(j=0;j<=MP;j++)
			dp[i][j] = max(dp[i][j], dp[i-1][j]);

		pre = i - W - 1;
		if(pre < 0)
			continue ;
		//����
		for(j=0;j<=MP;j++)
			arr[j] = dp[pre][j] + AP[i] * j;
		clear();
		for(j=0;j<=MP;j++)
		{
			push(j);
			dp[i][j] = max(dp[i][j], pop1(j, AS[i]) - AP[i] * j);
		}
		//����
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
