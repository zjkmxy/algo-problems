/*
HDU1517

ͨ��nֵ����ʤ��̬��
n     ->   INF     P
n/9   ->   n-1     N
n/18  ->   n/9     P
......
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

int main()
{
	unsigned long int n;
	int x;
	while(scanf("%d",&n)!=EOF)
	{
		//n�������ϴ�n-1�����ʤ��̬Ϊ(x&1)?P:N
		for(x=0;n>1;x++)
		{
			if(x & 1)
			{
				n = ceil(n / 2.0);
			}else{
				n = ceil(n / 9.0);
			}
		}
		printf((x&1)?"Stan wins.\n":"Ollie wins.\n");
	}
	return 0;
}
