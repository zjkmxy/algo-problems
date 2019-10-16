/*
HDU1536 S-Nim

最基本的SG值组合计算
这个因为求出了所有sg值所以跑得比较慢，递归求有效sg的标程就要快不少了。
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

int set[105];
int k;
int arr[105];
int m, l;

int sg[10005];
int maxval;

int mex(int pos)
{
	int i, j;
	bool flag;

	for(i=0;i<pos;i++)
	{
		flag = false;
		for(j=0;(j<k)&&(pos >= set[j]);j++)
		{
			if(sg[pos - set[j]] == i)
			{
				flag = true;
				break;
			}
		}
		if(!flag)
			break;
	}
	return i;
}

void calc(int mv)
{
	int i;

	for(i=maxval;i<=mv;i++)
		sg[i] = mex(i);
	maxval = mv;
}

void domain()
{
	int i, j, mv, sum;

	maxval = 0;
	for(i=0;i<k;i++)
	{
		scanf("%d",&set[i]);
	}
	sort(set,set+i);
	scanf("%d",&m);
	for(i=0;i<m;i++)
	{
		scanf("%d",&l);
		mv = 0;
		for(j=0;j<l;j++)
		{
			scanf("%d",&arr[j]);
			mv = max(mv, arr[j]);
		}
		calc(mv);
		sum = 0;
		for(j=0;j<l;j++)
		{
			sum ^= sg[arr[j]];
		}
		if(sum==0)
			printf("L");
		else
			printf("W");
	}
	printf("\n");
}

int main()
{
	while((scanf("%d",&k)!=EOF)&&(k!=0))
	{
		domain();
	}
	return 0;
}
