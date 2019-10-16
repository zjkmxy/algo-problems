/*
HDU4778 Gems Fight!
有B个包裹，里面有各种颜色的GEM，共有G个颜色。Alice和Bob轮流挑选包裹放到一个地方，如果挑选出来的同种颜色的GEM超过S个，当前回合者可以得分，每超过S个得一分。如果在回合内得分了，可以额外进行一个回合。问Alice的得分减去Bob的得分最大是多少。
状态：dp[i] 表示 初始可取包状态为 i 的时候 先手利用剩下的包还能取到的最大魔法石数量。
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<list>
using namespace std;

#define NUM_ST 2097192

struct BAG
{
	int color[8];
};

int G,B,S;      //颜色数，包裹数，炼成数
BAG bags[21];   
int dp[NUM_ST]; //先手所能得到的最大魔法石数
int cmax,sum;   //最大的状态数，一共的石头数

int calc_ms(int state, int *color) //计算某一状态下已得到的魔法石个数
{
	int i,cur,j;
	int ret;

	for(j=0;j<G;j++)
	{
		color[j] = 0;
	}
	for(i=0,cur=1;i<B;i++,cur=cur<<1)
	{
		if((state & cur) == 0)
		{
			for(j=0;j<G;j++)
			{
				color[j] += bags[i].color[j];
			}
		}
	}

	ret = 0;
	for(j=0;j<G;j++)
	{
		ret += color[j] / S;
		color[j] = color[j] % S;
	}

	return ret;
}

void calc_dp()
{
	int i,j,k;
	int cms, color[8]; //当前剩下的的魔法石个数，宝石数
	int cget;          //下一手操作能拿到的宝石数

	memset(dp, 0, cmax * sizeof(int));
	sum = calc_ms(0, color);
	for(i=1;i<cmax;i++)
	{
		cms = sum - calc_ms(i, color);
		for(j=0;j<B;j++)
		{
			if((i & (1<<j)) == 0) //不可以取
				continue;

			cget = 0;
			for(k=0;k<G;k++)
			{
				cget += (color[k] + bags[j].color[k]) / S;
			}

			if(cget > 0) //有石头可以拿？
			{
				dp[i] = max(dp[i], cget + dp[i ^ (1<<j)]);
			}else{
				dp[i] = max(dp[i], cms - dp[i ^ (1<<j)]);
			}
		}
	}
}

bool input()
{
	int i,j,n,c;

	scanf("%d %d %d",&G, &B, &S);
	if(G == 0)
		return false;

	for(i=0;i<B;i++)
	{
		scanf("%d",&n);
		for(j=0;j<G;j++)
		{
			bags[i].color[j] = 0;
		}
		for(j=0;j<n;j++)
		{
			scanf("%d",&c);
			bags[i].color[c-1]++;
		}
	}
	cmax = 1 << B;

	return true;
}

int main()
{
	while(input())
	{
		calc_dp();
		printf("%d\n",dp[cmax-1] - (sum - dp[cmax-1]));
	}
	return 0;
}

