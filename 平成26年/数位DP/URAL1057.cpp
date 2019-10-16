//���������[X,Y]�������������������������������ǡ�õ���K ��������ȵ�B����������֮�͡�
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

typedef signed long LL;
LL dp[40][30];  //λ����1����

void dp_calc()
{
	int i, j;

	dp[0][0] = 1;
	for(i=1;i<40;i++)
	{
		dp[i][0] = 1;
		for(j=1;j<30;j++)
		{
			dp[i][j] = dp[i-1][j] + dp[i-1][j-1];
		}
	}
}

LL calc(LL ans, LL k, LL b)
{
	int i = 0, j;
	int dig[40];
	LL ret = 0;

	while(ans > 0)
	{
		dig[i] = ans % b;
		ans /= b;
		i++;
	}
	dig[i] = 0;
	
	for(j=i-1;j>=0;j--)
	{
		if(dig[j] > 1)
		{
			ret += dp[j+1][k];
			break;               //ͳ�������е�����ˣ�����Ҫ������
		}else if(dig[j] == 1)
		{
			ret += dp[j][k];
			k--;
		}
		if(k < 0)
			break;
	}
	if(k == 0)
		ret++;

	return ret;
}

int main()
{
	int x, y, k, b;

	dp_calc();
	scanf("%ld%ld%ld%ld",&x,&y,&k,&b);
	printf("%ld\n",calc(y, k, b) - calc(x-1, k, b));
	//while(scanf("%ld%ld%ld",&x,&k,&b)!=EOF)
	//	printf("%ld\n",calc(x, k, b));
	return 0;
}

