/*
HDU1850 Being a Good Boy in Spring Festival

Nim���ģ�
Nim�ͣ����Ϊ0�ĵ�ȫ��P�㣬���м��ַ����Ļ���ֻҪ�ܰѵ�ǰNim�������ͺá�
���Ծ���sum^arr[i]<=arr[i]����Ŀ��

SGֵ����sum�������͡�
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int arr[105];
int main()
{
	int n, i, sum, ret;
	while(scanf("%d",&n)!=EOF)
	{
		if(n==0)
			return 0;
		sum = 0;
		for(i=0;i<n;i++)
		{
			scanf("%d",&arr[i]);
			sum ^= arr[i];
		}
		ret = 0;
		if(sum == 0)
		{
			printf("0\n");
			continue;
		}
		for(i=0;i<n;i++)
		{
			if(arr[i] >= (sum ^ arr[i]))
				ret++;
		}
		printf("%d\n",ret);
	}
	return 0;
}
