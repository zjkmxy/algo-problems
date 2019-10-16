/*
HDU1404 Digital Deletions
6�����֣����ֲ�����ĳλ��С�������ǰѵ���0��ĳλ�Լ��������ɾ��������˭���ɾ�����֡�

�մ���P�㣬����������0��ͷ�Ķ���N�㡣��1��P�㣬���Ǵ�1��ʼö�١�
ע�����Ų����в������˽����λ����0����������ö��ʱҲ�������������
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

#define MAXN 1000000
bool N[MAXN];  //��ǰ���Ƿ���N��
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

void enum_n(int x) //��Px����ٶ�Ӧ��N
{
	int len, i, j;
	int arr[6];

	len = numlen(x);
	decode(x, arr);
	//��λ
	for(i=0;i<len;i++)
	{
		for(j=9-arr[i];j>0;j--)
		{
			N[x + j * npow[i]] = true;
		}
	}
	//��0����0��ͷ����
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
