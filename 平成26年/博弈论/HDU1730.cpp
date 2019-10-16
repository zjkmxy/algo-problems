/*
HDU1730  Northcott Game

每一行棋子之间的距离是SG
所以这是一个组合Nim博弈
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

int main()
{
	int m, n, i, sum, a, b;

	while(scanf("%d%d",&n,&m)!=EOF)
	{
		sum = 0;
		for(i=0;i<n;i++)
		{
			scanf("%d%d",&a,&b);
			sum ^= labs(b - a) - 1;
		}
		if(sum == 0)
		{
			printf("BAD LUCK!\n");
		}else{
			printf("I WIN!\n");
		}
	}
	return 0;
}
