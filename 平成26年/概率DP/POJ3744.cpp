/*
���⣺��һ���������׵�·�ϣ������ڵ������1������N���㴦���е��ף�1<=N<=10�����׵�����귶Χ��[1,100000000].
ÿ��ǰ��p�ĸ���ǰ��һ����1-p�ĸ���ǰ��1-p������˳��ͨ������·�ĸ��ʡ����ǲ�Ҫ�ߵ��е��׵ĵط���
 
��dp[i]��ʾ����i��ĸ��ʣ��� ��ʼֵ dp[1]=1.
�������뵽ת�Ʒ��̣� dp[i]=p*dp[i-1]+(1-p)*dp[i-2];
������������ķ�Χ�ܴ�ֱ���������ǲ��еģ����ҵ��е�ĳЩ�㻹���ڵ��ס�
 
N���е��׵ĵ������Ϊ x[1],x[2],x[3]```````x[N].
���ǰѵ�·�ֳ�N�Σ�
1~x[1];
x[1]+1~x[2];
x[2]+1~x[3];
`
`
`
x[N-1]+1~x[N].
 
����ÿһ��ֻ��һ�����ס�����ֻҪ���ͨ��ÿһ�εĸ��ʡ��˷�ԭ����˾��Ǵ𰸡�
����ÿһ�Σ�ͨ���öεĸ��ʵ���1-�ȵ��ö��յ�ĵ��׵ĸ��ʡ�
 
�ͱ����һ�� 1~x[1].  ͨ���ö���ʵ���൱���ǵ���x[1]+1�㡣��ôp[x[1]+1]=1-p[x[1]].
�������ǰ����p[1]=1,�����ĸ��ʵ���1.���ں���Ķ�����Ҳ��һ���ļ��裬�����ͳ��������Ǵ��ˡ�
 
����ÿһ�εĸ��ʵ��󷨿���ͨ������˷������������
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef double MATRIX[2][2];

void mul(MATRIX a, MATRIX b)
{
	int i, j, k;
	MATRIX x;

	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			x[i][j] = 0;
			for(k=0;k<2;k++)
			{
				x[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			a[i][j] = x[i][j];
		}
	}
}

void powm(MATRIX a, int n, MATRIX x)
{
	MATRIX tmp;

	x[0][0] = x[1][1] = 1.0;
	x[0][1] = x[1][0] = 0.0;
	tmp[0][0] = a[0][0];
	tmp[0][1] = a[0][1];
	tmp[1][0] = a[1][0];
	tmp[1][1] = a[1][1];
	while(n > 0)
	{
		if(n & 1)
			mul(x, tmp);
		mul(tmp, tmp);
		n >>= 1;
	}
}

int comp(const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}

double domain(int n, double p)
{
	MATRIX mp, mr;
	int x[20], i;
	double ret = 1.0;

	x[0] = 0;
	for(i=1;i<=n;i++)
	{
		scanf("%d",x+i);
	}
	qsort(x + 1, n, sizeof(int), comp);

	mp[0][0] = p;
	mp[0][1] = 1;
	mp[1][0] = 1 - p;
	mp[1][1] = 0;
	//(1,0)*mp^n = (mp[0][0], mp[0][1])
	for(i=1;i<=n;i++)
	{
		if(x[i] <= x[i-1])
			continue;
		powm(mp, x[i] - x[i-1] - 1, mr);
		ret *= (1.0 - mr[0][0]);  //1��ȥ���е��׵ĸ���
	}

	return ret;
}

int main()
{
	int n;
	double p;

	while(scanf("%ld%lf",&n,&p)!=EOF)
	{
		printf("%.7lf\n",domain(n, p));
	}

	return 0;
}
