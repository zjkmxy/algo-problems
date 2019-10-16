#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
#include<stack>
using namespace std;

#define MAXL 105
#define TRUE 1
#define FALSE 0

int LSS[MAXL][MAXL], CNT[MAXL][MAXL];
int len;
char str[MAXL];

int init(int p, int q)
{
	int s;

	if(LSS[p][q] >= 0)
		return LSS[p][q];

	if(p > q)
	{
		LSS[p][q] = TRUE;
		return LSS[p][q];
	}
	if(p == q)
	{
		LSS[p][q] = FALSE;
		return LSS[p][q];
	}
	if(str[p] == '(' && str[q] == ')' && init(p+1, q-1))
	{
		LSS[p][q] = TRUE;
		return LSS[p][q];
	}
	if(str[p] == '[' && str[q] == ']' && init(p+1, q-1))
	{
		LSS[p][q] = TRUE;
		return LSS[p][q];
	}
	for(s=p+1;s<q-1;s++)
	{
		if(init(p, s) && init(s+1, q))
		{
			LSS[p][q] = TRUE;
			return LSS[p][q];
		}
	}

	LSS[p][q] = FALSE;
	return LSS[p][q];
}

int calc(int p, int q)
{
	int s;

	if(CNT[p][q] >= 0)
		return CNT[p][q];

	CNT[p][q] = 0;
	if(p >= q)
	{
		return CNT[p][q];
	}
	if(str[p] == '(' && str[q] == ')')
	{
		CNT[p][q] = max(CNT[p][q], calc(p+1, q-1) + 2);
	}
	if(str[p] == '[' && str[q] == ']')
	{
		CNT[p][q] = max(CNT[p][q], calc(p+1, q-1) + 2);
	}
	for(s=p;s<q;s++)
	{
		CNT[p][q] = max(CNT[p][q], calc(p, s) + calc(s+1, q));
	}
	return CNT[p][q];
}

int main()
{
	scanf("%s",&str);
	while(str[0] != 'e')
	{
		len = strlen(str);
		memset(LSS, -1, sizeof(LSS));
		memset(CNT, -1, sizeof(CNT));
		printf("%d\n",calc(0, len-1));

		scanf("%s",&str);
	}
	return 0;
}
