/*
HDU1847 Good Luck in CET-4 Everybody!

Bash���ģ��ܹ�N���ƣ�ÿ�˿���ץ2���ݴΣ���1����ץ�����һ�ŵ�ʤ��
3�ı�����P�㣬��������N�㡣
SGֵ��0,1,2,0,1,2,0,1,2......
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int main()
{
	int n;
	while(scanf("%d",&n)!=EOF)
	{
		if(n % 3 == 0)
			printf("Cici\n");
		else
			printf("Kiki\n");
	}
	return 0;
}
