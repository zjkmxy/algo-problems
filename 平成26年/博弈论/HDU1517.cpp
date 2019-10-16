/*
HDU1517

通过n值反推胜负态：
n     ->   INF     P
n/9   ->   n-1     N
n/18  ->   n/9     P
......
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

int main()
{
	unsigned long int n;
	int x;
	while(scanf("%d",&n)!=EOF)
	{
		//n以上至上次n-1区间的胜负态为(x&1)?P:N
		for(x=0;n>1;x++)
		{
			if(x & 1)
			{
				n = ceil(n / 2.0);
			}else{
				n = ceil(n / 9.0);
			}
		}
		printf((x&1)?"Stan wins.\n":"Ollie wins.\n");
	}
	return 0;
}
