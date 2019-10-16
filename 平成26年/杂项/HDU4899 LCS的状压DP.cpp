/*
�������Ȳ�����15��DNA���У���DNA��ΪM������LCSΪk(0<=k<=len1)��֮����ȡģ
795ms ��ǰ��һ
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

//Ԥ����λͼ
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

//λ������
int bitcnt(int x)
{
    int res = 0, t, j;
    int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};

	for(t=1,j=0;j<5;j++,t<<=1)
		x = (x & b[j]) + ((x >> t) & b[j]);
    return x;
}

//̰�ļ��㣬y����ƥ���λ�㣬x����ƥ���λ�㣬�����µ�ƥ��λ��
inline int domatch(int y, int x)
{
	int tx = y | x;
	int ty = (y << 1) | 1;
	return (tx & ((tx - ty) ^ tx));
	//����Ѿ�ƥ���λΪ0����һ���ƥ��x�����λ
	//����Ѿ�ƥ���λ��Ϊ0����һ�����ÿһ����͵�1����ͬ��ĩ��1
	//��AXBCDA,BDA,��ʱ�����001010=>100011
	//���Ǽ���Ľ�������ڱ�֤����ȵ�����¿����е����λ���ֲ�
	//ע�⣺���Ҫ���LCS��ֵ���ǳ��ȣ���ʱ����ִ���
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

			//ö���ַ�
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
