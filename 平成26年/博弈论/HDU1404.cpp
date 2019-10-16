/*
HDU1404 Digital Deletions
6个数字，两种操作，某位变小，或者是把等于0的某位以及后面的数删掉，问你谁最后删完数字。

空串是P点，所以所有以0开头的都是N点。而1是P点，于是从1开始枚举。
注意最优策略中不会有人将最高位减到0，所以我们枚举时也忽略这种情况。
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

#define MAXN 1000000
bool N[MAXN];  //当前点是否是N点
int npow[6] = {1, 10, 100, 1000, 10000, 100000};

int numlen(int x)
{
	int ret = 0;
	while(x > 0)
	{
		x /= 10;
		ret++;
	}
	return ret;
}

int encode(int arr[6])
{
	int i, ret = 0;

	for(i=5;i>=0;i--)
	{
		ret *= 10;
		ret += arr[i];
	}
	return ret;
}

void decode(int x, int arr[6])
{
	int i;
	for(i=0;i<6;i++)
	{
		arr[i] = x % 10;
		x /= 10;
	}
}

void enum_n(int x) //对Px，穷举对应的N
{
	int len, i, j;
	int arr[6];

	len = numlen(x);
	decode(x, arr);
	//加位
	for(i=0;i<len;i++)
	{
		for(j=9-arr[i];j>0;j--)
		{
			N[x + j * npow[i]] = true;
		}
	}
	//补0，加0开头的数
	for(i=6-len;i>0;i--)
	{
		N[x * npow[i]] = true;
		for(j=1;j<npow[i-1];j++)
		{
			N[x * npow[i] + j] = true;
		}
	}
}

void make_p()
{
	int i;
	N[0] = true;
	for(i=1;i<MAXN;i++)
	{
		if(!N[i])
			enum_n(i);
	}
}

int main()
{
	char str[10];
	int num;

	make_p();
	while(scanf("%s",str)!=EOF)
	{
		if(str[0] == '0')
		{
			printf("Yes\n");
			continue;
		}
		num = atoi(str);
		if(N[num])
		{
			printf("Yes\n");
		}else{
			printf("No\n");
		}
	}
	return 0;
}
