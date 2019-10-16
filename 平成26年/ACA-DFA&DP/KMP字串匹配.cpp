#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<iostream>
using namespace std;

#define MAXLEN 1000
char stra[MAXLEN],strb[MAXLEN];
int  strbn[MAXLEN];

// µº ∆•≈‰
int KMPMatch(char *s, char *p, int slen, int plen, int *next)
{
	int i, j;

	i = 0;
	j = 0;
	while(i < slen)
	{
		while((s[i] == p[j])&&(j < plen))
		{
			i++;
			j++;
		}
		if(j == plen)
		{
			return (i - plen); //∆•≈‰≥…π¶
		}
		j = next[j];
		if(j == -1)
		{
			i++;
			j++;
		}
	}

	return -1; //∆•≈‰ ß∞‹
}

//◊‘Œ“∆•≈‰
void KMPMakenext(char *p, int plen, int *next)
{
	int i,k;

	i = 0;
	k = -1;
	next[0] = -1;
	while(i < plen)
	{
		if(k == -1||p[i] == p[k])
		{
			i++;
			k++;
			next[i] = k;
		}else{
			k = next[k];
		}
	}
}

int main()
{
	int la,lb;

	while(scanf("%s%s",stra,strb)!=EOF)
	{
		la = strlen(stra);
		lb = strlen(strb);
		KMPMakenext(strb, lb, strbn);
		printf("  %d\n", KMPMatch(stra, strb, la, lb, strbn));
	}
	return 0;
}
