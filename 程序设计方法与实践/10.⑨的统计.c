#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAXN 1260
#define POW 100000000
typedef struct _NUMBER
{
	int x[MAXN];
	int n;
}NUMBER, *PNUMBER;
char str[2][10005];
NUMBER pow9[10005];
NUMBER ret[2];
NUMBER tmp[3];

//x*mt+at;mt<=10,at<10
void muladd(PNUMBER x, PNUMBER y, int mt, int at)
{
	int tmp = 0, i;

	if(x->n == 0)
	{
		y->n = 1;
		y->x[0] = at;
		return ;
	}
	tmp = at;
	for(i=0;i<x->n;i++)
	{
		tmp = x->x[i] * mt + tmp;
		y->x[i] = tmp % POW;
		tmp /= POW;
	}
	if(tmp > 0)
	{
		y->x[x->n] = tmp;
		y->n = x->n + 1;
	}else{
		y->n = x->n;
	}
}

void sipmul(PNUMBER a, PNUMBER b, PNUMBER x, int idx)
{
	int i;
	long long tmp;

	for(i=0;i<=idx;i++)
	{
		if(i >= a->n || idx-i >= b->n)
			continue;
		tmp = (long long)a->x[i] * (long long)b->x[idx-i];
		x->x[idx] += tmp % POW;
		x->x[idx+1] += tmp / POW;
	}
}

void mul(PNUMBER a, PNUMBER b, PNUMBER x)
{
	int i;

	for(i=0;i<a->n+b->n;i++)
		x->x[i] = 0;
	x->n = a->n + b->n;
	for(i=0;i<a->n+b->n;i++)
	{
		sipmul(a, b, x, i);
	}
	while(x->x[x->n-1] == 0 && x->n > 1)
	{
		--x->n;
	}
}

void atolint(PNUMBER x, char *s, int len)
{
	int pos = 0, i, pw = 1;

	x->x[0] = 0;
	for(i=len-1;i>=0;i--)
	{
		x->x[pos] += pw * s[i];
		pw *= 10;
		if(pw >= POW)
		{
			pw = 1;
			++pos;
			x->x[pos] = 0;
		}
	}
	x->n = pos + 1;
}

void sublint(PNUMBER a, PNUMBER b)
{
	int p;

	if(b->n > a->n)
	{
		a->n = *(int*)NULL;
	}
	if(b->n == a->n)
	{
		for(p=a->n-1;p>=0;p--)
		{
			if(a->x[p] < b->x[p])
				a->n = *(int*)NULL;
			else if(a->x[p] > b->x[p])
				break;
		}
	}

	for(p=b->n;p<a->n;p++)
		b->x[p] = 0;
	for(p=0;p<a->n;p++)
	{
		while(a->x[p] < b->x[p])
		{
			a->x[p+1]--;
			a->x[p] += POW;
		}
		a->x[p] -= b->x[p];
	}
	while(a->x[a->n-1] == 0 && a->n > 1)
	{
		--a->n;
	}
}

void printlint(PNUMBER x)
{
	int i;

	i = x->n - 1;
	printf("%d",x->x[i]);
	for(i--;i>=0;i--)
	{
		if(x->x[i] < 0)
			x->n = *(int*)NULL;
		printf("%08d",x->x[i]);
	}
	printf("\n");
}

void deal(int p)
{
	int i, len;
	int flag = 0;

	len = strlen(str[p]);
	for(i=0;i<len;i++)
		str[p][i] -= '0';

	tmp[0].n = 0;
	for(i=0;i<len;++i)
	{
		muladd(&tmp[0], &tmp[0], 9, str[p][i]);
		if(str[p][i] == 9)
			break;
	}
	atolint(&tmp[2], str[p], len);
	if(str[p][i] == 9)
	{
		mul(&tmp[0], &pow9[len-i-1], &tmp[1]);
		memcpy(&tmp[0], &tmp[1], sizeof(NUMBER));
		muladd(&tmp[2], &tmp[2], 1, 1);
	}
	sublint(&tmp[2], &tmp[0]);
	memcpy(&ret[p], &tmp[2], sizeof(NUMBER));
}

int main()
{
	int t;
	int flag;

	pow9[0].n = 1;
	pow9[0].x[0] = 1;
	for(t=1;t<=10000;t++)
	{
		muladd(&pow9[t-1], &pow9[t], 9, 0);
	}
	scanf("%d",&t);
	while(t--)
	{
		if(scanf("%s%s",str[0],str[1])==EOF)
			str[0][0] = *(int*)NULL;
		if(strstr(str[0], "9")!=NULL)
			flag = 1;
		else
			flag = 0;
		deal(0);
		deal(1);
		sublint(&ret[1], &ret[0]);
		if(flag)
			muladd(&ret[1], &ret[1], 1, 1);
		printlint(&ret[1]);
	}
	return 0;
}