/*
HDU1846 Brave Game

Bash博弈：
只有一堆n个物品，两个人轮流从这堆物品中取物，规定每次至少取一个，最多取m个。
最后取光者得胜。

显然(m+1)的全部倍数是P点，其他都是N点。
SG值是0,1,2,...,m,0,1,.....
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int main()
{
	int t, n, m;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d",&n,&m);
		if(n % (m + 1) == 0)
			printf("second\n");
		else
			printf("first\n");
	}
	return 0;
}