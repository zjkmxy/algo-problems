/*
HDU2147 kiki's game

��ʲ���ģ�������
�����в����͡�
SGֵ��ò��ֻ��0,1�ɡ�
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int main()
{
	int m,n;
	while(scanf("%d%d",&m,&n)!=EOF)
	{
		if((m==0)&&(n==0))
			return 0;
		if(((m&1)==0)||((n&1)==0))
			printf("Wonderful!\n");
		else
			printf("What a pity!\n");
	}
	return 0;
}
