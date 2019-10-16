/*
HDU1847 Good Luck in CET-4 Everybody!

Bash博弈：总共N张牌，每人可以抓2的幂次（含1），抓到最后一张的胜。
3的倍数是P点，其他都是N点。
SG值是0,1,2,0,1,2,0,1,2......
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int main()
{
	int n;
	while(scanf("%d",&n)!=EOF)
	{
		if(n % 3 == 0)
			printf("Cici\n");
		else
			printf("Kiki\n");
	}
	return 0;
}
