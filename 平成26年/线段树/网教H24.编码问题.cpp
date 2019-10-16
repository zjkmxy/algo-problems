//��a[i]֮ǰ��a[i]С��Ԫ�ظ�����Ϊ���룬����״������Եó��ܺõĽ⡣
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define lowbit(x) (x&(-x))

int arr1[10000],arr2[10000],scan[10000];
int n;

int getsum(int i)
{
	int sum = 0;
	for(;i>0;i-=lowbit(i))
		sum += scan[i];
	return sum;
}

void update(int i, int val)
{
	for(;i<=n;i+=lowbit(i))
		scan[i] += val;
}

int bisearch(int f)  //�����Ѿ���õı�aС�ĸ���������ǰ���aС�ĸ���������a
	                 //���Դ���ǰ���aС�����ĸ��������Զ��ֵó�a
{
	int begin,end,mid;

	begin = 0;
	end = n-1;

	while(end > begin + 1)
	{
		mid = (begin + end) / 2;
		if(getsum(mid+1)+f<=mid)
		{
			end = mid;
		}else{
			begin = mid;
		}
	}

	if(getsum(begin+1)+f==begin)
	{
		return begin;
	}else{
		return end;
	}
}

void code()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		arr2[i] = getsum(arr1[i]+1);
		update(arr1[i]+1, 1);
	}
}

void decode()
{
	int i;
	for(i=n-1;i>=0;i--)
	{
		arr2[i] = bisearch(arr1[i]);
		update(arr2[i]+1, 1);
	}
}

int main()
{
	int i,d;

	scanf("%d%d",&d,&n);
	for(i=0;i<n;i++)
		scanf("%d",arr1 + i);
	if(d == 1)
	{
		code();
	}else{
		decode();
	}
	for(i=0;i<n;i++)
	{
		printf(i == n-1?"%d\n":"%d ",arr2[i]);
	}

	return 0;
}