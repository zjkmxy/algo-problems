/*
HDU1846 Brave Game

Bash���ģ�
ֻ��һ��n����Ʒ�������������������Ʒ��ȡ��涨ÿ������ȡһ�������ȡm����
���ȡ���ߵ�ʤ��

��Ȼ(m+1)��ȫ��������P�㣬��������N�㡣
SGֵ��0,1,2,...,m,0,1,.....
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int main()
{
	int t, n, m;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d",&n,&m);
		if(n % (m + 1) == 0)
			printf("second\n");
		else
			printf("first\n");
	}
	return 0;
}