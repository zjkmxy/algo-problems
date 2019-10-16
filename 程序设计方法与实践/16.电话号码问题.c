#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
DWORD map[2][31251];	//3,6

const BYTE tab[] = 
{
	2, 2, 2,
	3, 3, 3,
	4, 4, 4,
	5, 5, 5,
	6, 6, 6,
	7, 0, 7, 7,
	8, 8, 8,
	9, 9, 9, 0,
};

typedef struct _DUP
{
	DWORD number;
	WORD cnt;
}DUP, *PDUP;
DUP dup[1502];
WORD nd;
BYTE flgerr, flgdup;
char str[22];

BYTE mapnum(DWORD num)
{
	int pos, p, q;
	pos = ((num / 1000000) == 6);
	num %= 1000000;
	p = (num >> 5);
	q = (1 << (num & 0x1F));
	if(map[pos][p] & q)
	{
		return 1;
	}else{
		map[pos][p] = (map[pos][p] | q);
		return 0;
	}
}

DWORD decode()
{
	WORD i, p, cnt = 0;
	DWORD ret = 0;

	for(i=0;str[i]!=0;i++)
	{
		if(str[i] == '-')
			continue;
		cnt++;
		if(isdigit(str[i]))
		{
			ret = ret * 10 + (str[i] - '0');
		}else{
			p = tab[str[i]-'A'];
			if(p == 0)
				return 0;
			else
				ret = ret * 10 + p;
		}
	}
	if(cnt != 7)
		return 0;
	p = (WORD)(ret / 1000000);
	if(p != 3 && p != 6)
		return 0;
	return ret;
}

void addone(DWORD num)
{
	WORD p;

	for(p=0;p<nd;p++)
	{
		if(dup[p].number == num)
			break;
	}
	if(p == nd)
	{
		nd++;
	}
	dup[p].number = num;
	dup[p].cnt++;
}

int comp(const void*A, const void*B)
{
	PDUP a = (PDUP)A, b = (PDUP)B;
	return a->number - b->number;
}

int main()
{
	DWORD num, i;
	flgerr = flgdup = 0;
	printf("Error:\n");
	while(scanf("%s",str)!=EOF)
	{
		num = decode();
		if(num == 0)
		{
			flgerr = 1;
			printf("%s\n",str);
			continue;
		}
		if(mapnum(num))
		{
			flgdup = 1;
			addone(num);
		}
	}
	if(!flgerr)
	{
		printf("Not found.\n");
	}
	printf("\nDuplication:\n");
	if(flgdup)
	{
		qsort(dup, nd, sizeof(DUP), comp);
		for(i=0;i<nd;i++)
		{
			printf("%03d-%04d %d\n",dup[i].number/10000,dup[i].number%10000,dup[i].cnt+1);
		}
	}else{
		printf("Not found.\n");
	}
	return 0;
}