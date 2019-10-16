/*
HDU2516 ȡʯ����Ϸ

����쳲��������ġ�ʣ����Ϊ쳲�������ʱΪP��������N��
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
using namespace std;

int fib[45];

void calc_fib()
{
	int i;

	fib[0] = 1;
	fib[1] = 2;
	for(i=2;i<45;i++)
	{
		fib[i] = fib[i-1] + fib[i-2];
	}
}

int main()
{
	int n, i;
	bool flag;
	calc_fib();
	while(scanf("%d",&n)!=EOF)
	{
		if(n==0)
			break;
		flag = false;
		for(i=0;i<45;i++)
		{
			if(n == fib[i])
			{
				printf("Second win\n");
				flag = true;
				break;
			}
		}
		if(!flag)
			printf("First win\n");
	}
	return 0;
}