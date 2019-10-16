#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 305
#define MAXP 35
#define MAXI 0x0fffffff

int vill[MAXN], presum[MAXN], postsum[MAXN];
int dp[MAXN][MAXP], v, p;

void input()
{
	int i;

	scanf("%d%d",&v,&p);
	for(i=1;i<=v;i++)
	{
		scanf("%d",&vill[i]);
		presum[i] = presum[i-1] + (vill[i] - vill[i-1]) * (i - 1);  //֮ǰ���е�i֮����
		postsum[i] = postsum[i-1] + vill[i] - vill[1];              //��iΪֹ���е�1�ľ���
	}
}

int getdist(int p, int q, int pos)  //p..q���е㵽pos�ľ����
{
	int ret = 0;

	ret += presum[pos] - presum[p] - (p - 1) * (vill[pos] - vill[p]);    //ǰ�ú�
	ret += postsum[q] - postsum[pos] - (q - pos) * (vill[pos] - vill[1]);//���ú�
	return ret;
}

int calc()
{
	int i, j, k;

	memset(dp, 0, sizeof(dp));
	for(i=1;i<=v;i++)
	{
		dp[i][1] = getdist(1, i, (i+1)>>1);
	}
	for(j=2;j<=p;j++)
	{
		for(i=1;i<=v;i++)
		{
			dp[i][j] = MAXI;
			for(k=1;k<i;k++)
			{
				dp[i][j] = min(dp[i][j], dp[k][j-1] + getdist(k+1, i, (k+1+i)>>1));//֮ǰ��j-1���ʾ֣�����֮��1���ʾ�
			}
		}
	}
	return dp[v][p];
}

int main()
{
	input();
	printf("%d\n",calc());
	return 0;
}
