/*
Ural 1519 Formula 1
�����Ʋ�ͷDP��Ѱ�ҹ��ܶٻ�·
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

struct MAPDP{  //���Ŷ�ַhash
	int head[HASH],next[STATE],size;
	QWORD state[STATE];
	QWORD f[STATE];
};

bool block[14][14];      //ͼ���Ƿ����[��][��]
MAPDP dp[2];             //������[��ǰ֮ǰ]
int n,m;                 //����������
int st_max;              //���״̬��+1(1 << m)
int cur;                 //��ǰȡ���ݵ�dp(next = cur ^ 1)
int ex,ey;               //�յ�λ��

QWORD encode(BYTE code[]) //����״̬(0=#,1=(,2=)����������������)
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

void decode(QWORD st, BYTE code[]) //����״̬
{
	int i;

	for(i=m;i>=0;i--)
	{
		code[i] = (st & 3);
		st >>= 2;
	}
}

void shrcode(BYTE code[])    //��������λ ()##()# => #()##()
{
	int i;

	for(i=m;i>=1;i--)
	{
		code[i] = code[i-1];
	}
	code[0] = 0; //����߲��ᱻ��
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

void calc_true(int i, int j) //������Ч��DP;i=0..n;j=1..m;������cur;�Ի�����ǰԤ��
{
	int sti;
	BYTE code[13],left,up,t;
	QWORD now;
	int fp,bk; //������ר��

	for(sti=0;sti<dp[cur].size;sti++)
	{
		decode(dp[cur].state[sti], code);
		now = dp[cur].f[sti];
		up = code[j];
		left = code[j-1];
		if((up == 0)&&(left == 0))  //Case1:��λ
		{
			if(block[i][j+1]&&block[i+1][j]) //���Կ��¿�
			{
				code[j-1] = 1;
				code[j]   = 2;
				hash_in(encode(code), now, cur^1);
			}
		}else if((up > 0)&&(left > 0)) //Case2:˫λ
		{
			if(up == left)  //2.1��ͬ
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
					shrcode(code); //������λ
				hash_in(encode(code), now, cur^1);
			}
			if((up == 1)&&(left == 2))  //2.2��·
			{
				code[j-1] = 0;
				code[j]   = 0;
				if(j == m)
					shrcode(code); //������λ
				hash_in(encode(code), now, cur^1);
			}
			if((up == 2)&&(left == 1))  //2.3�վ�
			{
				if((i == ex)&&(j == ey))
				{
					code[j-1] = 0;
					code[j]   = 0;
					if(j == m)
						shrcode(code); //������λ��code=0��
					hash_in(encode(code), now, cur^1);
				}
			}
		}else{                      //Case3:��λ
			if(up > 0)   //����������ۣ������д
				t = up;
			else
				t = left;
			if(block[i+1][j])
			{
				code[j-1] = t;
				code[j]   = 0;
				if(j == m)
					shrcode(code); //������λ
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

void calc_false(int i, int j) //������Ч��DP;i=0..n;j=1..m;
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
		if((up == 0)&&(left == 0))  //���ƿ�
		{
			if(j == m)
				shrcode(code); //������λ
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