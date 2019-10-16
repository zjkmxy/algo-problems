//FZU2107
/*
��Ŀ����������4*N�ľ��θ��ӡ�����������1*2��2*1��1*1��2*2�Ŀ飬����2*2��������Ҳ�ֻ��ʹ��һ�Ρ����������⡣

��ʹ��4*N�ĸ���ȫ���������ܷ���������֮�䲻���ص���

�ⷨ��״̬ѹ��DP��dp[i][j][k]��ʾǰ��i-1�ж��Ѿ��ڷ���ϣ���i�еİڷŶ�����״̬Ϊj��k��ʾ2*2�ķ����Ƿ��Ѿ��ڷţ��Ź��˾���1��û�Ź�����0.

��ֵdp[0][(1<<4)-1][0]=1

���Ƶ�ʱ��ֱ��ö��i-1�е�״̬��Ȼ�����ѣ�ʹ��i-1�а�������i�е�״̬������Ӧ�ķ�������

�ܸ��Ӷ�n*(1<<4)*(1<<4)


������dp[n+1][0][1]

����n+1��δ�ڷţ�2*2�Ѿ��ڷŵ�״̬��Ŀ��

�ð����������Լ��Ľⷨ������Ĳ�һ����ʱ�临�ӶȻ���̸��Ӷȡ�
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

int map[8][8]; //0Ϊ��λ��1Ϊ���òܲ٣��д�1�ƣ��д�0��
int state[30];
int stn;
int dp[6][30]; //������״̬��0С������һ�е�ס��1�ᣬ2����3�ϣ�
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
		//10��ʾһ�ᣬ3��Ч
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

//����״̬��Ч
bool check_m(int i, int st)
{
	int sa[4], j;

	decode(state[st], sa);
	for(j=0;j<4;j++)
	{
		if((map[i][j] == 1)&&(sa[j] > 0))
			return false;
		if((sa[j] == 1)&&(map[i][j+1] == 1)) //�Ὣ��·����
			return false;
		if((sa[j] == 2)&&(map[i+1][j] == 1)) //�ݽ���·����
			return false;
	}
	return true;
}

//����״̬��Ч
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

//���״̬��Ч
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
		for(sti=0;sti<stn;sti++)  //��һ��
		{
			if(!check_m(i, sti))
				continue;
			if((i==n)&&(!check_c(sti)))
				continue;
			for(stj=0;stj<stn;stj++)  //ǰһ��
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