/*
HDU1812 Count the Tetris

0°  n^2
90°（考虑原图四分，偶数就是n^2/4，奇数是(n-1)/2*(n+1)/2+1=(n^2+3)/4）
270°（和90的一样）
180°（偶数n^2/2，奇数(n^2+1)/2）
最后除以总变换（4种）

棋盘格染色，教程给的就是这个例子
TLE了，大数计算可以优化吧。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;

typedef signed long long int SINT;

#define POW 100000000
#define PTSTR "%08d"
#define HPOW 10000
#define NUM 167
//167 * 8 = 1336 位整数

typedef unsigned int lint;  //作为数组使用：lint a[NUM];

lint temp[NUM], temp2[NUM];

void clearlint(lint *x);
void addlint(lint *a, lint *b, lint *x);
void simplemullint(lint *a, lint *b, lint *x, lint idx);
void mullint(lint *a, lint *b, lint *x);
void mullints(lint *a, lint b, lint *x);
void assignlint(lint *a, lint *x);
int printlint(lint *x);
void atolint(lint *x, char str[]);
int complint(lint *a, lint *b);
void divlints(lint *x, lint d);

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

void calcsub(lint b, lint *x)
{
	lint r1[NUM],r2[NUM];
	lint i;

	clearlint(r1);
	clearlint(r2);
	clearlint(x);

	r1[0] = 1;
	r2[0] = 1;
	x[0] = 1;

	if(b <= 2)
	{
		x[0] = 1;
		return;
	}
	if(b == 3)
	{
		x[0] = 2;
		return;
	}

	for(i=2;i<b;i++)   
	{
		addlint(r1, r2, x);
		assignlint(r2, r1);
		assignlint(x, r2);
	} 
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

//大数快速模取幂
void ApB(SINT a, SINT b, lint x[])
{
	lint tmp[NUM], tmp2[NUM];

	clearlint(tmp);
	clearlint(x);
	x[0] = 1;
	tmp[0] = a;
	while(b >= 1)
	{
		if((b & 1) == 1)
		{
			clearlint(tmp2);
			mullint(x, tmp, tmp2);
			assignlint(tmp2, x);
		}

		b >>= 1; 
		clearlint(tmp2);
		mullint(tmp, tmp, tmp2);
		assignlint(tmp2, tmp);
	}
}

void divlints(lint *x, lint d)
{
	SINT tmp = 0;
	int i;

	for(i=NUM-1;i>=0;i--)
	{
		tmp += x[i];
		x[i] = tmp / d;
		tmp %= d;
		tmp *= POW;
	}
}

void calc(SINT c, SINT n, lint ret[])
{
	n = n * n;
	//0°
	ApB(c, n, ret);
	//90°（考虑原图四分，偶数就是n^2/4，奇数是(n-1)/2*(n+1)/2+1=(n^2+3)/4）
	//270°（和90的一样）
	ApB(c, (n + 3) >> 2, temp);
	clearlint(temp2);
	mullints(temp, 2, temp2);
	addlint(ret, temp2, temp);
	assignlint(temp, ret);
	//180°（偶数n^2/2，奇数(n^2+1)/2）
	ApB(c, (n + 1) >> 1, temp);
	addlint(ret, temp, temp2);
	assignlint(temp2, ret);
	//除以总变换（4种）
	divlints(ret, 4);
}

int main()
{
	int a, b;
	lint ret[NUM];

	while(scanf("%d%d",&a,&b)!=EOF)
	{
		calc(b, a, ret);
		printlint(ret);
	}
	return 0;
}