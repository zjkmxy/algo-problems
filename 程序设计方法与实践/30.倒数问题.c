#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define BOOL	int
#define TRUE	1
#define FALSE	0
#define MAXL	180
#define MAXN	250

char arr[2][MAXN][MAXL];	//2,5;ÃÝ´Î

void init()
{
	char str[MAXL];
	int i, j, plu;

	memset(str, 0, sizeof(str));
	str[0] = '1';
	strcpy(arr[0][0], str);
	strcpy(arr[1][0], str);
	for(i = 1; i<MAXN; i++)
	{
		plu = 0;
		for(j = 0; str[j] != 0; j++)
		{
			plu = plu + (str[j] - '0') * 5;
			str[j] = plu % 10 + '0';
			plu /= 10;
		}
		if(plu > 0)
			str[j] = plu + '0';
		strcpy(arr[1][i], str);
	}
	memset(str, 0, sizeof(str));
	str[0] = '1';
	for(i = 1; i<MAXN; i++)
	{
		plu = 0;
		for(j = 0; str[j] != 0; j++)
		{
			plu = plu + (str[j] - '0') * 2;
			str[j] = plu % 10 + '0';
			plu /= 10;
		}
		if(plu > 0)
			str[j] = plu + '0';
		strcpy(arr[0][i], str);
	}
}

BOOL scomp(char *stra, char *strb)
{
	int i = 0, j = 0;
	while(strb[j]>0)
		++j;
	for(--j; j >= 0; ++i, --j)
	if(stra[i] != strb[j])
		return FALSE;
	return stra[i] == 0;
}

BOOL deal()
{
	char str[MAXL];
	int zero = 0, len, i, p, mat = -1;
	if(scanf("%s", str) == EOF)
		return FALSE;
	len = strlen(str);
	while(str[len - 1] == '0')
	{
		++zero;
		str[--len] = 0;
	}
	for(i = 0; i<MAXN; i++)
	{
		if(scomp(str, arr[0][i]))
		{
			p = 1;
			mat = i;
			break;
		}
	}
	if(mat < 0)
	{
		for(i = 0; i<MAXN; i++)
		{
			if(scomp(str, arr[1][i]))
			{
				p = 0;
				mat = i;
				break;
			}
		}
	}
	if(mat < 0)
	{
		mat = *(int*)NULL;
	}
	zero += i;
	len = strlen(arr[p][mat]);
	zero -= len;
	printf("0.");
	while(zero--)
		printf("0");
	for(i = len - 1; i >= 0; i--)
		printf("%c", arr[p][mat][i]);
	printf("\n");
	return TRUE;
}

int main()
{
	int Q;
	init();
	scanf("%d", &Q);
	while(Q--)
		deal();
	return 0;
}