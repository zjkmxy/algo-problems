#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define POW 100000000
#define PTSTR "%08d"
#define HPOW 10000
#define NUM 5000
//5000 * 8 = 40000 位整数

typedef unsigned int lint;  //作为数组使用：lint a[NUM];

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

void calcsub(lint b, lint *x);

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

void atolint(lint *x, char str[])
{
	int cnt,i,base,j;
	char tmp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	clearlint(x);
	cnt = strlen(str);
	base = cnt - 8;
	for(i=0;base>0;i++,base-=8)
	{
		for(j=0;j<8;j++)
			tmp[j] = str[base+j];
		x[i] = atoi(tmp);
	}
	cnt = 8 + base;
	for(j=0;j<8;j++)
	{
		if(j < cnt)
			tmp[j] = str[j];
		else
			tmp[j] = 0;
	}
	x[i] = atoi(tmp);
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

int main(int argc, char ** argv)
{
	lint n,pos,i;
	lint x[2][NUM];

	while(scanf("%d",&n) != EOF)
	{
		clearlint(x[0]);
		clearlint(x[1]);
		x[0][0] = 1;
		pos = 0;
		for(i=2;i<=n;i++)
		{
			mullints(x[pos], i, x[1-pos]);
			pos = 1 - pos;
		}
		printlint(x[pos]);
	}

	return 0;
}
