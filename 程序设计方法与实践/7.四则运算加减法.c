#include<stdlib.h>
void mstrrev(char *str)
{
	char *p = str, tmp;
	while(*p != 0)
		p++;
	p--;
	while(p > str)
	{
		tmp = *p;
		*p = *str;
		*str = tmp;
		str++;
		p--;
	}
}

int convert(char *str, int len, int flg)
{
	int i;

	if(flg)
	{
		mstrrev(str);
		for(i = len - 1; i>0; i--)
		if(str[i] != '0')
			break;
		len = i + 1;
		str[len] = 0;
	}
	for(i = 0; i<len; i++)
	{
		str[i] = (flg ? str[i] - '0' : str[i] + '0');
		if(!flg && (str[i] < '0' || str[i] > '9'))
			return 0;
	}
	if(!flg)
	{
		str[len] = 0;
		mstrrev(str);
	}
	return len;
}

void plus(char *a, char *b, char *c)
{
	int len1, len2, p, base;
	int tmp;
	memset(c, 0, 1000);
	len1 = strlen(a);
	len2 = strlen(b);
	len1 = convert(a, len1, 1);
	len2 = convert(b, len2, 1);
	if(len1 < len2)
	{
		tmp = (int)a;
		a = b;
		b = (char*)tmp;
		tmp = len1;
		len1 = len2;
		len2 = tmp;
	}
	for(p = len2; p<len1; p++)
	{
		b[p] = 0;
	}
	for(p = 0; p<len1; p++)
	{
		a[p] = a[p] + b[p];
		a[p + 1] = a[p + 1] + a[p] / 10;
		a[p] %= 10;
	}
	if(a[len1] > 0)
		len1++;
	convert(a, len1, 0);
	strcpy(c, a);
}

int intcmp(char *a, char *b, int len)
{
	int i;

	for(i = len - 1; i >= 0; i--)
	{
		if(a[i] > b[i])
			return 1;
		if(a[i] < b[i])
			return -1;
	}
	return 0;
}

void minus(char *a, char *b, char *c)
{
	int len1, len2, p, base;
	int tmp, flg = 0;
	len1 = strlen(a);
	len2 = strlen(b);
	len1 = convert(a, len1, 1);
	len2 = convert(b, len2, 1);
	if(len1 < len2 || (len1 == len2 && intcmp(a, b, len1) < 0))
	{
		tmp = (int)a;
		a = b;
		b = (char*)tmp;
		tmp = len1;
		len1 = len2;
		len2 = tmp;
		printf("-");
	}
	for(p = len2; p<len1; p++)
	{
		b[p] = 0;
	}
	for(p = 0; p<len1; p++)
	{
		if(a[p] < b[p])
		{
			a[p + 1]--;
			a[p] += 10;
		}
		a[p] -= b[p];
	}
	///////
	for(base = len1 - 1; base>0; base--)
	{
		if(a[base] > 0)
			break;
	}
	len1 = base + 1;
	convert(a, len1, 0);
	strcpy(c, a);
}