/*
hdu 4539
一个n*m的图(n<=10, m <=100)，一些格子有障碍不可选。需要选择一些点，使得对于每一个选择的点没有其他选择的点与其海明距离等于2或0。询问最多可选的点数。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_M     10
#define MAX_N     100
#define MAX_STATE 1024
typedef unsigned short int WORD;

int n,m,max_num;
bool map[MAX_N][MAX_M];
int cst[180];
int cst_num;
int dp[2][180][180];  //[有用否][这一行][前一行]
int cur;

bool chk_st1(WORD st) //无地图检查
{
	return !((st << 2) & st);
}

bool chk_st2(WORD st1, WORD st2, WORD st3) //三联无图检查（下、中、上三状态）
{
	if((st1 & st3) > 0)
		return false;
	if(((st1 << 1) & st2) > 0)
		return false;
	if(((st1 >> 1) & st2) > 0)
		return false;

	return true;
}

int get_num(int i, WORD st)  //得到某状态下的兵数
{
	int j,k,ret;

	ret = 0;
	for(j=0,k=1;j<m;j++,k<<=1)
	{
		if((st & k) > 0)
		{
			if(map[i][j])
			{
				ret++;
			}else{
				return 0;  //没必要改成-1
			}
		}
	}

	return ret;
}

void make_cst()
{
	int i;

	cst_num = 0;
	for(i=0;i<max_num;i++)
	{
		if(chk_st1(i))
		{
			cst[cst_num] = i;
			cst_num++;
		}
	}
}

void calc_dp(int i)
{
	int sti1,sti2,stif;
	WORD st1, st2, stf;
	int max,now;

	for(sti1=0;sti1<cst_num;sti1++)  //这一行
	{
		st1 = cst[sti1];
		now = get_num(i, st1);
		for(sti2=0;sti2<cst_num;sti2++)  //前一行
		{
			st2 = cst[sti2];
			if(!chk_st2(st1, st2, 0))
			{
				dp[cur^1][sti1][sti2] = 0;
				continue;
			}

			dp[cur^1][sti1][sti2] = now;
			max = 0;
			for(stif=0;stif<cst_num;stif++)  //再前一行
			{
				stf = cst[stif];
				if(!chk_st2(st1, st2, stf))
				{
					continue;
				}

				if(dp[cur][sti2][stif] > max)
				{
					max = dp[cur][sti2][stif];
				}
			}
			dp[cur^1][sti1][sti2] += max;
		}
	}
}

int calc()
{
	int i,sti,sti2,ret;
	WORD st;

	cur = 0;
	for(sti=0;sti<cst_num;sti++)
	{
		st = cst[sti];
		dp[cur][sti][0] = get_num(0, st);
	}

	for(i=1;i<n;i++)
	{
		calc_dp(i);
		cur ^= 1;
	}

	ret = 0;
	for(sti=0;sti<cst_num;sti++)
	{
		for(sti2=0;sti2<cst_num;sti2++)
		{
			if(dp[cur][sti][sti2] > ret)
			{
				ret = dp[cur][sti][sti2];
			}
		}
	}
	return ret;
}

bool input()
{
	int i,j,curr;

	if(scanf("%d %d", &n, &m) == EOF)
		return false;

	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			scanf("%d", &curr);
			map[i][j] = (curr > 0);
		}
	}
	max_num = (1 << m);

	return true;
}

int main()
{
	while(input())
	{
		make_cst();
		memset(dp, 0, sizeof(dp));
		printf("%d\n",calc());
	}
	return 0;
}
