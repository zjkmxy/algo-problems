/*
给定长度不大于15的DNA序列，求DNA长为M且与其LCS为k(0<=k<=len1)者之个数取模
795ms 当前第一
*/
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

#define MAXS (1<<15)
#define MODN 1000000007

char source[16];
int pattern[4], len1, M;
int dp[2][MAXS];
int ans[16];
int got[MAXS][4];

//预处理位图
void set_pattern()
{
	int i, j, k;

	memset(pattern, 0, sizeof(pattern));
	for(i=0,j=1;i<len1;i++,j<<=1)
	{
		switch(source[i])
		{
		case 'A':
			k = 0;
			break;
		case 'G':
			k = 1;
			break;
		case 'C':
			k = 2;
			break;
		case 'T':
			k = 3;
			break;
		}
		pattern[k] = (pattern[k] | j);
	}
}

//位数计算
int bitcnt(int x)
{
    int res = 0, t, j;
    int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};

	for(t=1,j=0;j<5;j++,t<<=1)
		x = (x & b[j]) + ((x >> t) & b[j]);
    return x;
}

//贪心计算，y是已匹配的位点，x是能匹配的位点，返回新的匹配位点
inline int domatch(int y, int x)
{
	int tx = y | x;
	int ty = (y << 1) | 1;
	return (tx & ((tx - ty) ^ tx));
	//如果已经匹配的位为0，这一句会匹配x的最低位
	//如果已经匹配的位不为0，这一句会用每一段最低的1换下同段末的1
	//如AXBCDA,BDA,此时结果是001010=>100011
	//于是计算的结果就是在保证最长长度的情况下可能有的最低位数分布
	//注意：如果要求出LCS的值而非长度，此时会出现错误
}

void dp_calc()
{
	int i, st, ed, j;
	int rd, wt;

	for(st=0;st<(1<<len1);st++)
	{
		dp[0][st] = dp[1][st] = 0;
	}
	dp[0][0] = 1;

	int S = (1 << len1);
	for(st=0;st<S;st++){
		for(j=0;j<4;j++){
			got[st][j] = domatch(st, pattern[j]);
		}
	}

	for(i=1,rd=0,wt=1;i<=M;i++,wt^=1,rd^=1)
	{
		for(st=0;st<S;st++)
			dp[wt][st] = 0;
		for(st=0;st<(1<<len1);st++)
		{
			if(dp[rd][st] == 0)
				continue;

			//枚举字符
			for(j=0;j<4;j++)
			{
				ed = got[st][j];
				dp[wt][ed] += dp[rd][st];
				dp[wt][ed] %= MODN;
			}
		}
	}

	memset(ans, 0, sizeof(ans));
	for(st=0;st<(1<<len1);st++)
	{
		j = bitcnt(st);
		ans[j] += dp[rd][st];
		ans[j] %= MODN;
	}
	for(i=0;i<=len1;i++)
	{
		printf("%d\n",ans[i]);
	}
}

int main()
{
	int t;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%s%d",source,&M);
		len1 = strlen(source);
		set_pattern();
		dp_calc();
	}
	return 0;
}
