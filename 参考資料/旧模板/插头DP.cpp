/*
Ural 1519 Formula 1
三进制插头DP，寻找哈密顿回路
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

typedef unsigned char          BYTE;
typedef unsigned short int     WORD;
typedef unsigned long int      DWORD;
typedef unsigned long long int QWORD;

#define HASH  30007
#define STATE 1000010

struct MAPDP{  //开放定址hash
	int head[HASH],next[STATE],size;
	QWORD state[STATE];
	QWORD f[STATE];
};

bool block[14][14];      //图块是否可行[行][列]
MAPDP dp[2];             //方案数[当前之前]
int n,m;                 //行数，列数
int st_max;              //最大状态号+1(1 << m)
int cur;                 //当前取数据的dp(next = cur ^ 1)
int ex,ey;               //终点位置

QWORD encode(BYTE code[]) //编码状态(0=#,1=(,2=)，从左往右依次算)
{
	QWORD ret = 0;
	int i;

	for(i=0;i<m+1;i++)
	{
		ret <<= 2;
		ret += code[i];
	}

	return ret;
}

void decode(QWORD st, BYTE code[]) //解码状态
{
	int i;

	for(i=m;i>=0;i--)
	{
		code[i] = (st & 3);
		st >>= 2;
	}
}

void shrcode(BYTE code[])    //换行右移位 ()##()# => #()##()
{
	int i;

	for(i=m;i>=1;i--)
	{
		code[i] = code[i-1];
	}
	code[0] = 0; //最左边不会被插
}

void hash_init(int obj)
{
	dp[obj].size = 0;
	memset(dp[obj].head , -1, sizeof(dp[obj].head));
}

int hash_find(QWORD state, int obj)
{
	int i;

	for(i=dp[obj].head[state % HASH];i!=-1;i=dp[obj].next[i])
	{
		if(dp[obj].state[i]==state)
		{
			return i;
		}
	}

	return -1;
}

void hash_in(QWORD state, QWORD count, int obj)
{
	int i;
	
	i = hash_find(state, obj);
	if(i != -1)
	{
		dp[obj].f[i]+=count;
	}else{
		dp[obj].state[dp[obj].size]=state;
		dp[obj].f[dp[obj].size]=count;
		dp[obj].next[dp[obj].size]=dp[obj].head[state % HASH];
		dp[obj].head[state % HASH]=dp[obj].size++;
	}
}

void calc_true(int i, int j) //先手有效块DP;i=0..n;j=1..m;不处理cur;对换行提前预判
{
	int sti;
	BYTE code[13],left,up,t;
	QWORD now;
	int fp,bk; //找括号专用

	for(sti=0;sti<dp[cur].size;sti++)
	{
		decode(dp[cur].state[sti], code);
		now = dp[cur].f[sti];
		up = code[j];
		left = code[j-1];
		if((up == 0)&&(left == 0))  //Case1:空位
		{
			if(block[i][j+1]&&block[i+1][j]) //可以开新块
			{
				code[j-1] = 1;
				code[j]   = 2;
				hash_in(encode(code), now, cur^1);
			}
		}else if((up > 0)&&(left > 0)) //Case2:双位
		{
			if(up == left)  //2.1两同
			{
				bk = 1;
				for(fp=((up==1)?j+1:j-2);(fp>0)&&(fp<=m);((up==1)?fp++:fp--))
				{
					if(code[fp] == up)
					{
						bk++;
					}
					if((code[fp] != up)&&(code[fp] > 0))
					{
						bk--;
					}
					if(bk == 0)
					{
						code[fp] = up;
						break;
					}
				}
				code[j-1] = 0;
				code[j]   = 0;
				if(j == m)
					shrcode(code); //换行移位
				hash_in(encode(code), now, cur^1);
			}
			if((up == 1)&&(left == 2))  //2.2闭路
			{
				code[j-1] = 0;
				code[j]   = 0;
				if(j == m)
					shrcode(code); //换行移位
				hash_in(encode(code), now, cur^1);
			}
			if((up == 2)&&(left == 1))  //2.3终局
			{
				if((i == ex)&&(j == ey))
				{
					code[j-1] = 0;
					code[j]   = 0;
					if(j == m)
						shrcode(code); //换行移位（code=0）
					hash_in(encode(code), now, cur^1);
				}
			}
		}else{                      //Case3:单位
			if(up > 0)   //避免分类讨论，简便书写
				t = up;
			else
				t = left;
			if(block[i+1][j])
			{
				code[j-1] = t;
				code[j]   = 0;
				if(j == m)
					shrcode(code); //换行移位
				hash_in(encode(code), now, cur^1);
			}
			if(block[i][j+1])
			{
				code[j-1] = 0;
				code[j]   = t;
				hash_in(encode(code), now, cur^1);
			}
		}
	}
}

void calc_false(int i, int j) //先手无效块DP;i=0..n;j=1..m;
{
	int sti;
	BYTE code[13],left,up;
	QWORD now;

	for(sti=0;sti<dp[cur].size;sti++)
	{
		decode(dp[cur].state[sti], code);
		now = dp[cur].f[sti];
		up = code[j];
		left = code[j-1];
		if((up == 0)&&(left == 0))  //能绕开
		{
			if(j == m)
				shrcode(code); //换行移位
			hash_in(encode(code), now, cur^1);
		}
	}
}

QWORD calc_dp()
{
	int i,j;
	int sti;

	if(ex == -1)
		return 0;
	hash_init(0);
	hash_init(1);
	cur = 0;
	hash_in(0, 1, 0);
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			hash_init(cur^1);
			if(block[i][j])
				calc_true(i, j);
			else
				calc_false(i, j);
			cur ^= 1;
		}
	}

	sti = hash_find(0, cur);
	if(sti == -1)
		return 0;
	else
		return dp[cur].f[sti];
}

bool input()
{
	int i,j;
	char cur;

	if(!(cin >> n))
		return false;
	cin >> m;

	ex = -1;
	ey = -1;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			cin >> cur;
			if(cur == '.')
			{
				block[i][j] = true;
				ex = i;
				ey = j;
			}
			else
				block[i][j] = false;
		}
	}

	return true;
}

int main()
{
	while(input())
	{
		cout << calc_dp() << endl;
	}
	return 0;
}