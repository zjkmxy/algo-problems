/*
HDU-2167 Pebbles
����һ��N*N������3<=N<=15)��Ҫ��ѡ�����ɸ�����ʹ�������ѡ�����ܺ����ѡ���Ĺ��������ѡ��ĳ��������ô���İ˸����ڷ������������ѡ��
״̬��ת�ƣ�
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_NUM 32768
typedef unsigned short int WORD;

int dp[16][MAX_NUM]; //������õ���[�к�][ѹ��״̬��]
int parr[16][16];    //��ֵ��
int n;               //������
int max_num;         //״̬����
int cst[1600];       //������ì�ܵ�״̬����Ҫ���٣�ʵ���Ͽ����Դ˹����໥��ì�ܵ�״̬����н�һ�����٣�
int cst_num;         //������ì�ܵ�״̬����

bool check_st(WORD a) //��鵥�е�״̬(a)�Ƿ�ì��
{
	return !((a << 1) & a);
}

bool check_st(WORD a, WORD b) //������е�״̬(b)�Ƿ�ì��
{
	if((a & b) > 0)
		return false;
	if(((a << 1) & b) > 0)
		return false;
	if(((a >> 1) & b) > 0)
		return false;

	return true;
}

int getnum(int ind, WORD st) //�õ�ind��st״̬�µ���ֵ
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
	int i,sti,stbi; //��ǰ�У���ǰ��״̬��ǰһ��״̬
	int max;      //ǰһ�е����ֵ

	for(i=0;i<n;i++)
	{
		for(sti=0;sti<cst_num;sti++)
		{
			dp[i][cst[sti]] = getnum(i, cst[sti]);
			if(i > 0)  //���ǵ�һ��
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

	//�������
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

	if(n != n2) //��ȡ����
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
