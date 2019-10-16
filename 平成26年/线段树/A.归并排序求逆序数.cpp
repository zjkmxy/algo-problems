//HDU1394 Minimum Inversion Number 
#include<cstdio>
#include<cstdlib>
#include<cstring>

int n;
int nums[5000];
int sort[5000];
int tmp1[5000],tmp2[5000];

int merge(int l, int r, int mid)
{
	int i, j, k, ret;

	for(i=l;i<=mid;i++)
		tmp1[i] = sort[i];
	for(i=mid+1;i<=r;i++)
		tmp2[i] = sort[i];
	i = l;
	j = mid + 1;
	ret = 0;
	for(k=l;k<=r;k++)
	{
		if(tmp1[i] > tmp2[j])
		{
			sort[k] = tmp2[j];
			j++;
			ret += mid + 1 - i;
		}else{
			sort[k] = tmp1[i];
			i++;
		}
		if((j > r)||(i > mid))
		{
			k++;
			break;
		}
	}
	while(i <= mid)
	{
		sort[k] = tmp1[i];
		k++;
		i++;
	}
	while(j <= r)
	{
		sort[k] = tmp2[j];
		k++;
		j++;
	}

	return ret;
}

int msort(int l, int r)
{
	int ret, mid;

	if(l >= r)
		return 0;

	mid = (l + r) >> 1;
	ret = msort(l, mid);
	ret += msort(mid + 1, r);
	ret += merge(l, r, mid);
	return ret;
}

bool input()
{
	int i;

	if(scanf("%d",&n)==EOF)
		return false;
	for(i=0;i<n;i++)
	{
		scanf("%d",&nums[i]);
		nums[i]++;
		sort[i] = nums[i];
	}

	return true;
}

int calc()
{
	int i, min, ret;

	ret = msort(0, n-1);
	min = ret;
	for(i=0;i<n;i++)
	{
		ret = ret + n + 1 - 2 * nums[i];
		if(ret < min)
			min = ret;
	}

	return min;
}

int main()
{
	while(input())
	{
		printf("%d\n",calc());
	}
	return 0;
}
