/*
HDU1525 Euclid's Game

典型的欧几里得游戏：
给出两个数，a和b，将大的数中，减去若干b的倍数，最终有一个数为0的话就胜了。
网上的迭代解法不是最优解。

考虑：（以下a>b）
a/b==1时，先手只能有一种选择(b,a%b)，因此a/b>=2时先手可以选择有谁面对(a,a%b)所以必胜。
令a/b=r，那么r==1或r>=2时，先手胜。
当1<r<2时先手被迫到达(b,a-b)，于是再次判定，如果b>=2(a-b)即r<3/2时。后手必胜。
当3/2<r<2时后手又到达了(a-b,2b-a)，于是当r>=5/3时，先手必胜。
当3/2<r<5/3时，……
容易看出这个序列中a,b的系数都是Fibonacci数列中的数，于是最终的比率是：
rlim==lim{n->inf}Fn+1/Fn=1/g
其中g是黄金分割比。
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

