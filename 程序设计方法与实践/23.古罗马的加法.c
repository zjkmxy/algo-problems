#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char number[2001][20];
char stra[20], strb[20];
int a, b;

void convert(int num, char* str)
{
	int pos = 0;

	while(num >= 1000)
	{
		pos += sprintf(str + pos, "M");
		num -= 1000;
	}

	if(num >= 900)
	{
		pos += sprintf(str + pos, "CM");
		num -= 900;
	}
	if(num >= 500)
	{
		pos += sprintf(str + pos, "D");
		num -= 500;
	}
	if(num >= 400)
	{
		pos += sprintf(str + pos, "CD");
		num -= 400;
	}
	while(num >= 100)
	{
		pos += sprintf(str + pos, "C");
		num -= 100;
	}

	if(num >= 90)
	{
		pos += sprintf(str + pos, "XC");
		num -= 90;
	}
	if(num >= 50)
	{
		pos += sprintf(str + pos, "L");
		num -= 50;
	}
	if(num >= 40)
	{
		pos += sprintf(str + pos, "XL");
		num -= 40;
	}
	while(num >= 10)
	{
		pos += sprintf(str + pos, "X");
		num -= 10;
	}

	if(num >= 9)
	{
		pos += sprintf(str + pos, "IX");
		num -= 9;
	}
	if(num >= 5)
	{
		pos += sprintf(str + pos, "V");
		num -= 5;
	}
	if(num >= 4)
	{
		pos += sprintf(str + pos, "IV");
		num -= 4;
	}
	while(num >= 1)
	{
		pos += sprintf(str + pos, "I");
		num -= 1;
	}
}

void init()
{
	int i;

	for(i = 1; i <= 2000; i++)
	{
		convert(i, number[i]);
	}
}

int match(char *str)
{
	int i;
	for(i = 1; i <= 1000; i++)
	if(strcmp(str, number[i]) == 0)
		return i;
	return -1;
}

int main()
{
	init();
	scanf("%s%s", stra, strb);
	a = match(stra);
	b = match(strb);
	if(a == -1 || b == -1)
	{
		printf("Aha! I don't need to calculate the sum\n");
	} else{
		printf("%s\n", number[a + b]);
	}
	return 0;
}