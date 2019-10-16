//URAL1661 Dodecahedron ��G++��AC
/*
������Ŀ��˼�ܼ򵥡� ����ʮ��ľ����ÿ��ľ���и���ɫ�� ����Щľ���ܹ�������ٲ�ͬ����ʮ�����塣 
������ʮ�������ǵȼ۵ĵ��ҽ��������ܹ�ͨ����ת�������ȫ�غϣ�����ɫ��ͬ��

���������Ƚ������һ�������Σ�������K����ɫȾɫ�������������޵����⡣ 
�������Ľⷨ�Ƚϴ�ͳ����polya��burnside��ʽ������ܹ����㡣�ؼ����ڼ�����ʮ��������תȺ��60���� Ԫ�أ�ÿ��Ԫ�������µĲ���������� 
��ص�֪ʶ�Ͳ�׸���ˡ������ʵĿ�����������صı���֪ʶ��

��ʮ���������תȺԪ�أ�60�����ɷ�Ϊ4�ࣺ
1�������任1���� ��ͬ�ֻ�����30
2����Ӧ��������������ת6*4�����ֻ��ֽ�Ϊ��5)(5)(5)(5)(5)(5)
3: ��Ӧ��������ת10*2�����ֻ��ֽ�Ϊ(3)(3)(3)(3)(3)(3)(3)(3)(3)(3)
4: ��Ӧ��������ת15�����ֻ��ֽ�Ϊ(1)(1)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)

�����ֻ��ֽ���εõ�����ҿ�����ϸ���������ſӵ���ͼ����Ȼע��������ʮ���壬��ʵ��������ʮ�����壩
�ڼ����ֻ��ֽ�ʱ�����������ɣ�1�ǿռ����룬ȥ��ת���۲�ȼ۱ߣ���������Ⱥ�����ʣ�ȺԪ�صĽױض���Ⱥ������Լ���������ڲ¡�

�����Ļ���������Ϳ��Խ��ˡ���������⣬ÿ��ľ������ɫ���ȸ�������Ҫ������ɫ���䷽����������֤ÿ���ֻ��ڲ�����ɫ��ͬ�� 
���嵽������⣬��Ϊ�ֻ��ֽ���й��ɣ����Կ����������ѧֱ�Ӽ��㷽���� 
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define POW 100000000
#define PTSTR "%08d"
#define HPOW 10000
#define NUM 10
//5 * 8 = 40 λ����

typedef unsigned long int lint;  //��Ϊ����ʹ�ã�lint a[NUM];

lint zero[NUM];

void clearlint(lint *x);
void addlint(lint *a, lint *b, lint *x);
void simplemullint(lint *a, lint *b, lint *x, lint idx);
void mullint(lint *a, lint *b, lint *x);
void mullints(lint *a, lint b, lint *x);
void assignlint(lint *a, lint *x);
int printlint(lint *x);
void atolint(lint *x, char str[]);
int complint(lint *a, lint *b);

void assignlint(lint *a, lint *x)
{
	lint i;
	for(i=0;i<NUM;i++)
		x[i] = a[i];
}

void clearlint(lint *x)
{
	lint i;
	for(i=0;i<NUM;i++)
		x[i] = 0;
}

void addlint(lint *a, lint *b, lint *x)
{
	lint i;

	clearlint(x);

	for(i=0;i<NUM;i++)
	{
		x[i] += a[i] + b[i];
		if(i < NUM-1)
			x[i+1] += x[i]/POW;
		x[i] %= POW;
	}
}

void simplemullint(lint *a, lint *b, lint *x, lint idx)
{
	lint i;
	lint m,n,o,p,tmp;

	for(i=0;i<=idx;i++)
	{
		m = a[i] / HPOW;
		n = a[i] % HPOW;
		o = b[idx-i] / HPOW;
		p = b[idx-i] % HPOW;
		tmp = m * p + n * o;

		x[idx] += n * p;
		x[idx] += tmp % HPOW * HPOW;
		if(idx < NUM-1)
			x[idx+1] += x[idx]/POW + m * o + tmp / HPOW;
		x[idx] %= POW;
	}
}

void mullint(lint *a, lint *b, lint *x)
{
	lint i;

	clearlint(x);
	for(i=0;i<NUM;i++)
		simplemullint(a, b, x, i);
}

void mullints(lint *a, lint b, lint *x)
{
	lint i,m,n,o,p,tmp;

	clearlint(x);

	o = b / HPOW;
	p = b % HPOW; 
	for(i=0;i<=NUM;i++)
	{
		m = a[i] / HPOW;
		n = a[i] % HPOW;
		tmp = n * o + m * p;

		x[i] += n * p;
		x[i] += tmp % HPOW * HPOW;
		if(i < NUM-1)
			x[i+1] += x[i]/POW + m * o + tmp / HPOW;
		x[i] %= POW;
	}
}

int printlint(lint *x)
{
	int st = 0;
	int i;

	for(i=NUM-1;i>=0;i--)
	{
		if((x[i] == 0)&&(st == 0))
			continue;
		if(st == 0)
		{
			st = printf("%d",x[i]);
			continue;
		}
		st += printf(PTSTR,x[i]);
	}
	if(st == 0)
		st = printf("0");
	st += printf("\n");

	return st;
}

int complint(lint *a, lint *b)
{
	int i;

	for(i=NUM-1;i>=0;i--)
	{
		if(a[i]>b[i])
			return 1;
		if(a[i]<b[i])
			return -1;
	}
	return 0;
}

lint divlint32(lint *a, lint b)
{
	int i;
	long long tmp = 0;

	for(i=NUM-1;i>=0;i--)
	{
		tmp = tmp * POW + a[i];
		a[i] = tmp / b;
		tmp %= b;
	}

	return (lint)tmp;
}

//=====================================

int col[31];
lint cop_tmp[NUM];

void cop(int n, int m, lint x[])  //nCm
{
	int i;

	clearlint(x);
	x[0] = 1;
	if(m > (n >> 1))
	{
		m = n - m;
	}
	for(i=n;i>n-m;i--)
	{
		mullints(x, i, cop_tmp);
		assignlint(cop_tmp, x);
	}
	for(i=m;i>1;i--)
	{
		divlint32(x, i);
	}
}

void calc(int n, int k, lint x[])  //k��ɫn���ж�����
{
	int i;
	int col2[31];
	lint tmp[NUM], tmp2[NUM];

	clearlint(x);
	for(i=1;i<=30;i++)
	{
		if(col[i] % k > 0)
			return;
		col2[i] = col[i] / k;
	}
	x[0] = 1;
	for(i=1;i<=30;i++)
	{
		if(col2[i] <= 0)
			continue;
		cop(n, col2[i], tmp);
		mullint(tmp, x, tmp2);
		assignlint(tmp2, x);
		n -= col2[i];
	}
}

int main()
{
	int i, j, c;
	lint ret[NUM], tmp[NUM], tmp2[NUM];

	for(i=1;i<=30;i++)
	{
		col[i] = 0;
	}
	for(i=1;i<=30;i++)
	{
		scanf("%d",&c);
		col[c]++;
	}
	clearlint(ret);

	calc(30, 1, tmp);
	addlint(ret, tmp, tmp2);
	assignlint(tmp2, ret);

	calc(6, 5, tmp);
	mullints(tmp, 24, tmp2);
	addlint(ret, tmp2, tmp);
	assignlint(tmp, ret);

	calc(10, 3, tmp);
	mullints(tmp, 20, tmp2);
	addlint(ret, tmp2, tmp);
	assignlint(tmp, ret);

	for(i=1;i<=30;i++)
	{
		if(col[i] <= 0)
			continue;
		col[i]--;
		for(j=1;j<=30;j++)
		{
			if(col[j] <= 0)
				continue;
			col[j]--;

			calc(14, 2, tmp);
			mullints(tmp, 15, tmp2);
			addlint(ret, tmp2, tmp);
			assignlint(tmp, ret);

			col[j]++;
		}
		col[i]++;
	}

	divlint32(ret, 60);
	printlint(ret);

	return 0;
}
