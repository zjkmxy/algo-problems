/*
HDU2147 kiki's game

巴什博弈：走棋盘
九字切不解释。
SG值？貌似只有0,1吧。
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int main()
{
	int m,n;
	while(scanf("%d%d",&m,&n)!=EOF)
	{
		if((m==0)&&(n==0))
			return 0;
		if(((m&1)==0)||((n&1)==0))
			printf("Wonderful!\n");
		else
			printf("What a pity!\n");
	}
	return 0;
}
