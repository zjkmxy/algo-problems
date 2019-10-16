//FZU2107
/*
题目描述：给定4*N的矩形格子。在里面填上1*2，2*1，1*1，2*2的块，其中2*2的真必需且并只能使用一次。其他的随意。

求使得4*N的格子全部填满的总方法数，块之间不能重叠。

解法：状态压缩DP，dp[i][j][k]表示前面i-1行都已经摆放完毕，第i行的摆放二进制状态为j，k表示2*2的方块是否已经摆放，放过了就是1，没放过就是0.

初值dp[0][(1<<4)-1][0]=1

递推的时候直接枚举i-1行的状态，然后深搜，使得i-1行摆满，第i行的状态加上相应的方法数。

总复杂度n*(1<<4)*(1<<4)


最后输出dp[n+1][0][1]

即第n+1行未摆放，2*2已经摆放的状态数目。

好吧下面是我自己的解法和上面的不一样。时间复杂度换编程复杂度。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

int map[8][8]; //0为空位，1为安置曹操，行从1计，列从0计
int state[30];
int stn;
int dp[6][30]; //行数，状态（0小兵或被上一行挡住，1横，2竖，3废）
int n;

void decode(int st,int sa[])
{
	int i;
	for(i=0;i<4;i++)
	{
		sa[i] = (st&3);
		st >>= 2;
	}
}

int encode(int sa[])
{
	int i, ret;
	ret = 0;
	for(i=3;i>=0;i--)
	{
		ret += sa[i];
		ret <<= 2;
	}
	return ret;
}

void enumst()
{
	int st, sa[4], i, flag;
	for(st=0;st<256;st++)
	{
		decode(st, sa);
		flag = true;
		//10表示一横，3无效
		for(i=0;i<=3;i++)
		{
			if((sa[i]==1)&&(sa[i+1]!=0))
			{
				flag = false;
				break;
			}
			if(sa[i]==3)
			{
				flag = false;
				break;
			}
		}
		if((sa[3]==1)||(sa[3]==3))
			flag = false;
		if(!flag)
			continue;
		state[stn] = st;
		stn++;
	}
}

//单行状态有效
bool check_m(int i, int st)
{
	int sa[4], j;

	decode(state[st], sa);
	for(j=0;j<4;j++)
	{
		if((map[i][j] == 1)&&(sa[j] > 0))
			return false;
		if((sa[j] == 1)&&(map[i][j+1] == 1)) //横将右路挡曹
			return false;
		if((sa[j] == 2)&&(map[i+1][j] == 1)) //纵将下路挡曹
			return false;
	}
	return true;
}

//集体状态有效
bool check_b(int now, int bak)
{
	int san[4], sab[5], j;

	decode(state[now], san);
	decode(state[bak], sab);
	sab[4] = 0;
	for(j=0;j<4;j++)
	{
		if((sab[j] == 2)&&(san[j] > 0))
			return false;
		if((san[j] == 1)&&(sab[j+1] == 2))
			return false;
	}
	return true;
}

//最后状态有效
bool check_c(int st)
{
	int sa[4], j;

	decode(state[st], sa);
	for(j=0;j<4;j++)
	{
		if((map[n][j] == 1)&&(sa[j] > 0))
			return false;
		if(sa[j] == 2)
			return false;
	}
	return true;
}

int dp_calc()
{
	int i, sti, stj, ret;
	memset(dp, 0, sizeof(dp));
	dp[0][0] = 1;
	for(i=1;i<=n;i++)
	{
		for(sti=0;sti<stn;sti++)  //这一行
		{
			if(!check_m(i, sti))
				continue;
			if((i==n)&&(!check_c(sti)))
				continue;
			for(stj=0;stj<stn;stj++)  //前一行
			{
				if(!check_b(sti, stj))
					continue;

				dp[i][sti] += dp[i-1][stj];
			}
		}
	}

	ret = 0;
	for(sti=0;sti<stn;sti++)
	{
		if(check_c(sti))
			ret += dp[n][sti];
	}

	return ret;
}

int makemap()
{
	int i, j, ret;

	ret = 0;
	for(i=1;i<=n-1;i++)
	{
		for(j=0;j<3;j++)
		{
			map[i][j] = 1;
			map[i][j+1] = 1;
			map[i+1][j] = 1;
			map[i+1][j+1] = 1;
			ret += dp_calc();
			map[i][j] = 0;
			map[i][j+1] = 0;
			map[i+1][j] = 0;
			map[i+1][j+1] = 0;
		}
	}

	return ret;
}

int main()
{
	int t;
	enumst();
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d",&n);
		if(n <= 1)
		{
			printf("0\n");
			continue;
		}
		printf("%d\n",makemap());
	}
}