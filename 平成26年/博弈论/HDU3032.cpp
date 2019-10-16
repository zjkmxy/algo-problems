/*
HDU3032 Nim or not Nim?

逆推SG
0   ->   0
1   ->   1
2   ->   2
3   ->   4(1+2=3)
4   ->   3(之前的3变成了4)
5   ->   5
6   ->   6
7   ->   8(2+5=7)
8   ->   7(同理)
...
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
using namespace std;

int calc_sg(int x)
{
	if((x & 3) == 3)
		return (x + 1);
	if((x & 3) == 0)
		return (x - 1);
	return x;
}

int main()
{
	int t, n, a, sum;

	scanf("%d",&t);
	while(t--)
	{
		scanf("%d",&n);
		sum = 0;
		while(n--)
		{
			scanf("%d",&a);
			sum ^= calc_sg(a);
		}
		if(sum)
			printf("Alice\n");
		else
			printf("Bob\n");
	}
	return 0;
}
