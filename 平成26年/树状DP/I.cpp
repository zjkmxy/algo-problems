/*
HDU4778 Gems Fight!
��B�������������и�����ɫ��GEM������G����ɫ��Alice��Bob������ѡ�����ŵ�һ���ط��������ѡ������ͬ����ɫ��GEM����S������ǰ�غ��߿��Ե÷֣�ÿ����S����һ�֡�����ڻغ��ڵ÷��ˣ����Զ������һ���غϡ���Alice�ĵ÷ּ�ȥBob�ĵ÷�����Ƕ��١�
״̬��dp[i] ��ʾ ��ʼ��ȡ��״̬Ϊ i ��ʱ�� ��������ʣ�µİ�����ȡ�������ħ��ʯ������
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<list>
using namespace std;

#define NUM_ST 2097192

struct BAG
{
	int color[8];
};

int G,B,S;      //��ɫ������������������
BAG bags[21];   
int dp[NUM_ST]; //�������ܵõ������ħ��ʯ��
int cmax,sum;   //����״̬����һ����ʯͷ��

int calc_ms(int state, int *color) //����ĳһ״̬���ѵõ���ħ��ʯ����
{
	int i,cur,j;
	int ret;

	for(j=0;j<G;j++)
	{
		color[j] = 0;
	}
	for(i=0,cur=1;i<B;i++,cur=cur<<1)
	{
		if((state & cur) == 0)
		{
			for(j=0;j<G;j++)
			{
				color[j] += bags[i].color[j];
			}
		}
	}

	ret = 0;
	for(j=0;j<G;j++)
	{
		ret += color[j] / S;
		color[j] = color[j] % S;
	}

	return ret;
}

void calc_dp()
{
	int i,j,k;
	int cms, color[8]; //��ǰʣ�µĵ�ħ��ʯ��������ʯ��
	int cget;          //��һ�ֲ������õ��ı�ʯ��

	memset(dp, 0, cmax * sizeof(int));
	sum = calc_ms(0, color);
	for(i=1;i<cmax;i++)
	{
		cms = sum - calc_ms(i, color);
		for(j=0;j<B;j++)
		{
			if((i & (1<<j)) == 0) //������ȡ
				continue;

			cget = 0;
			for(k=0;k<G;k++)
			{
				cget += (color[k] + bags[j].color[k]) / S;
			}

			if(cget > 0) //��ʯͷ�����ã�
			{
				dp[i] = max(dp[i], cget + dp[i ^ (1<<j)]);
			}else{
				dp[i] = max(dp[i], cms - dp[i ^ (1<<j)]);
			}
		}
	}
}

bool input()
{
	int i,j,n,c;

	scanf("%d %d %d",&G, &B, &S);
	if(G == 0)
		return false;

	for(i=0;i<B;i++)
	{
		scanf("%d",&n);
		for(j=0;j<G;j++)
		{
			bags[i].color[j] = 0;
		}
		for(j=0;j<n;j++)
		{
			scanf("%d",&c);
			bags[i].color[c-1]++;
		}
	}
	cmax = 1 << B;

	return true;
}

int main()
{
	while(input())
	{
		calc_dp();
		printf("%d\n",dp[cmax-1] - (sum - dp[cmax-1]));
	}
	return 0;
}

