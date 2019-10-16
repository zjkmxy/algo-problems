#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char str[1000];
int dp1[1000][2], dp2[1000][2];  //Ç°ºó£»r,b
int N;

int max(int a, int b)
{
	return a > b ? a : b;
}

int main()
{
	int i;
	int maxo;

	scanf("%d%s",&N,str);
	for(i=N;i<N*2;i++)
	{
		str[i] = str[i-N];
	}
	for(i=N*2-1;i>=0;i--)
	{
		if(str[i] == 'r')
		{
			dp1[i][1] = 0;
		}else{
			dp1[i][1] = dp1[i+1][1] + 1;
		}
		if(str[i] == 'b')
		{
			dp1[i][0] = 0;
		}else{
			dp1[i][0] = dp1[i+1][0] + 1;
		}
	}
	for(i=1;i<N*2;i++)
	{
		if(str[i-1] == 'r')
		{
			dp2[i][1] = 0;
		}else{
			dp2[i][1] = dp2[i-1][1] + 1;
		}
		if(str[i-1] == 'b')
		{
			dp2[i][0] = 0;
		}else{
			dp2[i][0] = dp2[i-1][0] + 1;
		}
	}
	maxo = max(dp1[0][0], dp1[0][1]) + max(dp2[N][0], dp2[N][1]);
	for(i=1;i<N;i++)
	{
		maxo = max(maxo, max(dp1[i][0], dp1[i][1]) + max(dp2[i+N][0], dp2[i+N][1]));
	}
	printf("%d\n",maxo>N?N:maxo);
	
	return 0;
}