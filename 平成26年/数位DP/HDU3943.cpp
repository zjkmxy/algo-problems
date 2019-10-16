//区间(P,Q]之间第K_i大的Nya数（含X个4和Y个7的数）。
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

typedef signed long long LL;
LL dp[30][10][21][21];  //位数，元素，4,7

void dp_calc()
{
	int i, j, k, l, x;

	for(i=0;i<=9;i++)
	{
		dp[0][i][0][0] = 1;
	}
	dp[0][4][0][0] = 0;
	dp[0][4][1][0] = 1;
	dp[0][7][0][0] = 0;
	dp[0][7][0][1] = 1;

	for(i=1;i<30;i++)
	{
		for(j=0;j<10;j++)
		{
			for(k=0;k<=20;k++)
			{
				for(l=0;l<=20;l++)
				{
					for(x=0;x<10;x++)
					{
						switch(j)
						{
						case 4:
							if(k > 0)
								dp[i][j][k][l] += dp[i-1][x][k-1][l];//这里坑了我很久，因为k-1是负数的时候会得到错误的结果
							break;
						case 7:
							if(l > 0)
								dp[i][j][k][l] += dp[i-1][x][k][l-1];
							break;
						default:
							dp[i][j][k][l] += dp[i-1][x][k][l];
							break;
						}
					}
				}
			}
		}
	}
}

LL calc(LL ans, int x, int y)
{
	int i = 0, j, k;
	int dig[30];
	LL ret = 0;

	while(ans > 0)
	{
		dig[i] = ans % 10;
		ans /= 10;
		i++;
	}
	dig[i] = 0;
	for(j=i-1;j>=0;j--)
	{
		for(k=0;k<dig[j];k++)
		{
			ret += dp[j][k][x][y];
		}
		if(dig[j] == 4)
		{
			x--;
		}
		if(dig[j] == 7)
		{
			y--;
		}
		if(x < 0 || y < 0)
			break;
	}
	if(x == 0 && y == 0)
		ret++;

	return ret;
}

LL bisearch(LL p, LL q, int x, int y, LL k)
{
	LL equ, mid, cur;

	equ = calc(p, x, y) + k;
	if(equ > calc(q, x, y))
		return -1;

	//答案在(p,q]区间内
	while(q > p + 1)
	{
		mid = (p + q) >> 1;
		cur = calc(mid, x, y);
		if(cur >= equ)
		{
			q = mid;
		}else{
			p = mid;
		}
	}

	return q;
}

int main()
{
	int t, x, y, n, i;
	LL ans, p, k, q;

	dp_calc();
	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		printf("Case #%d:\n",i);
		scanf("%I64d%I64d%d%d%d",&p,&q,&x,&y,&n);
		while(n--)
		{
			scanf("%I64d",&k);
			ans = bisearch(p, q, x, y, k);
			if(ans >= 0)
				printf("%I64d\n",ans);
			else
				printf("Nya!\n");
		}
	}

	return 0;
}

