//URAL1661 Dodecahedron 限G++能AC
/*
首先题目意思很简单。 有三十根木棍，每根木棍有个颜色。 用这些木棍能够搭出多少不同的正十二面体。 
两个正十二面体是等价的当且仅当他们能够通过旋转神马的完全重合，且颜色相同。

首先我们先解决他的一个简单情形，就是用K种颜色染色，但是数量不限的问题。 
这个问题的解法比较传统。用polya，burnside公式神马的能够计算。关键在于计算正十二面体旋转群（60个） 元素，每个元素作用下的不动点个数。 
相关的知识就不赘述了。有疑问的可以先搜索相关的背景知识。

正十二面体的旋转群元素（60个）可分为4类：
1：不动变换1个， 不同轮换个数30
2：对应面中心连线轴旋转6*4个，轮换分解为（5)(5)(5)(5)(5)(5)
3: 对应点连线旋转10*2个，轮换分解为(3)(3)(3)(3)(3)(3)(3)(3)(3)(3)
4: 对应边连线旋转15个，轮换分解为(1)(1)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)(2)

具体轮换分解如何得到，大家可以仔细看上面那张坑爹的图（居然注释是正二十面体，其实明明是正十二面体）
在计算轮换分解时，有两个技巧，1是空间想想，去旋转，观察等价边，而是利用群的性质，群元素的阶必定是群阶数的约数。有利于猜。

这样的话，简单问题就可以解了。对于这道题，每个木棍的颜色事先给定，需要计算颜色分配方案个数，保证每个轮换内部的颜色相同。 
具体到这个问题，因为轮换分解很有规律，可以考虑用组合数学直接计算方案。 
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define POW 100000000
#define PTSTR "%08d"
#define HPOW 10000
#define NUM 10
//5 * 8 = 40 位整数

typedef unsigned long int lint;  //作为数组使用：lint a[NUM];

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

void calc(int n, int k, lint x[])  //k着色n个有多少种
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
