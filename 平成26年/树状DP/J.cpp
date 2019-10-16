/*
HDU-2167 Pebbles
给定一个N*N个矩阵（3<=N<=15)，要你选择若干个数，使得最后所选的数总和最大。选数的规则是如果选了某个数，那么它的八个相邻方向的数都不能选。
状态？转移？
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_NUM 32768
typedef unsigned short int WORD;

int dp[16][MAX_NUM]; //最大能拿到的[行号][压缩状态号]
int parr[16][16];    //数值组
int n;               //行列数
int max_num;         //状态个数
int cst[1600];       //自身无矛盾的状态（重要加速，实际上可以以此构造相互无矛盾的状态表进行进一步加速）
int cst_num;         //自身无矛盾的状态个数

bool check_st(WORD a) //检查单行的状态(a)是否矛盾
{
	return !((a << 1) & a);
}

bool check_st(WORD a, WORD b) //检查下行的状态(b)是否矛盾
{
	if((a & b) > 0)
		return false;
	if(((a << 1) & b) > 0)
		return false;
	if(((a >> 1) & b) > 0)
		return false;

	return true;
}

int getnum(int ind, WORD st) //得到ind行st状态下的数值
{
	int i, ret;

	ret = 0;
	for(i=0;i<n;i++)
	{
		if((st & (1 << i)) > 0)
			ret += parr[ind][i];
	}

	return ret;
}

void make_cst()
{
	int i;

	cst_num = 0;
	for(i=0;i<max_num;i++)
	{
		if(check_st(i))
		{
			cst[cst_num] = i;
			cst_num++;
		}
	}
}

int calc_dp()
{
	int i,sti,stbi; //当前行，当前行状态，前一行状态
	int max;      //前一行的最大值

	for(i=0;i<n;i++)
	{
		for(sti=0;sti<cst_num;sti++)
		{
			dp[i][cst[sti]] = getnum(i, cst[sti]);
			if(i > 0)  //不是第一行
			{
				max = 0;
				for(stbi=0;stbi<cst_num;stbi++)
				{
					if(check_st(cst[stbi], cst[sti]) && (dp[i-1][cst[stbi]] > max))
					{
						max = dp[i-1][cst[stbi]];
					}
				}
				dp[i][cst[sti]] += max;
			}
		}
	}

	//最后的最大
	max = 0;
	for(sti=0;sti<cst_num;sti++)
	{
		if(dp[n-1][cst[sti]] > max)
		{
			max = dp[n-1][cst[sti]];
		}
	}
	return max;
}

int innum(int ind, char *str)
{
	int cur,i;
	int pos,len;

	len = strlen(str);
	i = 0;
	cur = 0;
	for(pos=0;pos<=len;pos++)
	{
		if(isdigit(str[pos]))
		{
			cur = cur * 10 + str[pos] - '0';
		}else{
			parr[ind][i] = cur;
			i++;
			cur = 0;
		}
	}

	return i;
}

bool input()
{
	char str[500];
	int n2;

	if(!gets(str))
		return false;
	n = 0;

	while(str[0] != 0)
	{
		n++;

		n2 = innum(n - 1, str);

		if(!gets(str))
			break;
	}

	if(n != n2) //读取错误
		return *((int*)NULL);
	max_num = 1 << n;

	return true;
}

int main(int argc, char ** argv)
{
	while(input())
	{
		make_cst();
		printf("%d\n",calc_dp());
	}

	return 0;
}
