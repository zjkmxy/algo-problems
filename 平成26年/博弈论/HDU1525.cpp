/*
HDU1525 Euclid's Game

���͵�ŷ�������Ϸ��
������������a��b����������У���ȥ����b�ı�����������һ����Ϊ0�Ļ���ʤ�ˡ�
���ϵĵ����ⷨ�������Ž⡣

���ǣ�������a>b��
a/b==1ʱ������ֻ����һ��ѡ��(b,a%b)�����a/b>=2ʱ���ֿ���ѡ����˭���(a,a%b)���Ա�ʤ��
��a/b=r����ôr==1��r>=2ʱ������ʤ��
��1<r<2ʱ���ֱ��ȵ���(b,a-b)�������ٴ��ж������b>=2(a-b)��r<3/2ʱ�����ֱ�ʤ��
��3/2<r<2ʱ�����ֵ�����(a-b,2b-a)�����ǵ�r>=5/3ʱ�����ֱ�ʤ��
��3/2<r<5/3ʱ������
���׿������������a,b��ϵ������Fibonacci�����е������������յı����ǣ�
rlim==lim{n->inf}Fn+1/Fn=1/g
����g�ǻƽ�ָ�ȡ�
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double ratio;
double rate;

void init()
{
	ratio = (sqrt(5.0) + 1.0) / 2.0;
}

int main()
{
	int a, b, t;

	init();
	while(scanf("%d%d",&a,&b)!=EOF)
	{
		if((a==0)&&(b==0))
			break;
		if(a == b)
		{
			printf("Stan wins\n");
			continue;
		}
		if(a < b)
		{
			t = a;
			a = b;
			b = t;
		}
		rate = (double)a / (double)b;
		if(rate >= ratio)
			printf("Stan wins\n");
		else
			printf("Ollie wins\n");
	}
	return 0;
}

