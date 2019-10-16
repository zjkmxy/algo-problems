#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 20002
#define RANGE  1000002
typedef unsigned long CHAR; //注意getlcp中默认以0收尾

CHAR gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //公用
int N,K;

void make_sa1(CHAR *str, int sa[], int rank[], int len)
{
	int i;
	int *cnt;

	cnt = new int[RANGE+1];
	//计数排序
	memset(cnt, 0, sizeof(int) * (RANGE + 1));
	for(i=0;i<len;i++)
	{
		cnt[str[i]]++;
	}
	for(i=1;i<RANGE;i++)
	{
		cnt[i]+=cnt[i-1];
	}
	for(i=len-1;i>=0;i--)
	{
		cnt[str[i]]--;
		sa[cnt[str[i]]] = i;
	}

	//取得排位
	rank[sa[0]] = 1;
	for(i=1;i<len;i++)
	{
		if(str[sa[i]] == str[sa[i-1]])
			rank[sa[i]] = rank[sa[i-1]];
		else
			rank[sa[i]] = rank[sa[i-1]] + 1;
	}

	delete cnt;
}

int getrank(int len, int rank[], int i)
{
	if(i < len)
		return rank[i];
	else
		return 0;
}

int make_sam(CHAR *str, int sa[], int rank[], int k, int len) //基数排序i+k,i
{
	int i,*cnt,*temp;

	cnt = new int[len+1];
	temp = new int[len];
	//排序尾位
	memset(cnt, 0, sizeof(int) * (len + 1));
	for(i=0;i<len;i++)
	{
		cnt[getrank(len, rank, i+k)]++;
	}
	for(i=1;i<=len;i++)
	{
		cnt[i]+=cnt[i-1];
	}
	for(i=len-1;i>=0;i--)
	{
		cnt[getrank(len, rank, i+k)]--;
		temp[cnt[getrank(len, rank, i+k)]] = i;
	}

	//排序前位
	memset(cnt, 0, sizeof(int) * (len + 1));
	for(i=0;i<len;i++)
	{
		cnt[getrank(len, rank, temp[i])]++;
	}
	for(i=1;i<=len;i++)
	{
		cnt[i]+=cnt[i-1];
	}
	for(i=len-1;i>=0;i--)
	{
		cnt[getrank(len, rank, temp[i])]--;
		sa[cnt[getrank(len, rank, temp[i])]] = temp[i];
	}

	//取得排位
	temp[sa[0]] = 1;
	for(i=1;i<len;i++)
	{
		if((getrank(len, rank, sa[i])==getrank(len, rank, sa[i-1]))&&(getrank(len, rank, sa[i]+k)==getrank(len, rank, sa[i-1]+k)))
			temp[sa[i]] = temp[sa[i-1]];
		else
			temp[sa[i]] = temp[sa[i-1]] + 1;
	}
	memcpy(rank, temp, sizeof(int) * len);

	if(temp[sa[len-1]]==len)
		k = 0; //可以停止
	else
		k = k * 2;

	delete [] temp;
	delete [] cnt;
	return k;
}

void make_sa(CHAR *str, int sa[], int rank[], int len)
{
	int m;

	make_sa1(str, sa, rank, len);
	for(m=1;m<len;m<<=1)
	{
		if(make_sam(str, sa, rank, m, len) == 0)
			break;
	}
	for(m=0;m<len;m++)
	{
		rank[m]--;  //原来是从1数的
	}
}

int getlcp(CHAR *a, CHAR *b)
{
	int i = 0;
	while(a[i] && b[i] && (a[i] == b[i]))
		i++;
	return i;
}

void calc_ht(CHAR *str, int sa[], int rank[], int height[], int len)
{
	int i,l;

	height[0] = 0;
	if(rank[0])
	{
		height[rank[0]] = getlcp(str, str + sa[rank[0]-1]);
	}
	for(i=1;i<len;i++)
	{
		if(!rank[i])
			continue;
		if(height[rank[i-1]] <= 1)
		{
			height[rank[i]] = getlcp(str + i, str + sa[rank[i]-1]);
		}else{
			l = height[rank[i-1]] - 1;
			height[rank[i]] = l + getlcp(str + i + l, str + sa[rank[i]-1] + l);
		}
	}
}

bool exist(CHAR *str, int sa[], int height[], int len, int sublen)
{
	int i,min_sa,max_sa;

	for(i=1;i<len;i++)
	{
		if(height[i] < sublen)
			continue;

		min_sa = sa[i-1];
		max_sa = sa[i-1];
		while((height[i] >= sublen)&&(i < len))
		{
			if(sa[i] > max_sa)
				max_sa = sa[i];
			if(sa[i] < min_sa)
				min_sa = sa[i];
			if(max_sa - min_sa >= sublen)
				return true;
			i++;
		}
	}
	return false;
}

int bisearch(CHAR *str, int sa[], int height[], int len)
{
	int start,end,mid;

	start = 1;
	end = len / 2;

	while(end > start + 1)
	{
		mid = (start + end) / 2;
		if(exist(str, sa, height, len, mid))
		{
			start = mid;
		}else{
			end = mid;
		}
	}

	if(exist(str, sa, height, len, end))
		return end;
	if(exist(str, sa, height, len, start))
		return start;
	return 0;
}

bool input()
{
	int i;

	if(scanf("%d %d",&N,&K)==EOF)
	{
		return false;
	}
	K--;
	for(i=0;i<N;i++)
	{
		scanf("%d",gstr+i);
		gstr[i]++;  //防0
	}
	gstr[N] = 0;

	return true;
}

int mlk(int height[], int len, int x)  //连续大于等于x的数字最长有几个
{
	int i,ret,max;

	ret = 0;
	max = 0;
	for(i=1;i<len;i++)
	{
		if(height[i] >= x)
		{
			ret++;
		}else
		{
			if(ret > max)
				max = ret;
			ret = 0;
		}
	}
	if(ret > max)
		max = ret;
	return max;
}

int calc_minmax()  //二分计算height中连续K(已减)个数字的最小值的最大值
{
	int start,end,mid;

	start = 0;
	end = N - 1;
	while(end > start + 1)
	{
		mid = (start + end) / 2;
		if(mlk(gheight, N, mid) >= K)
		{
			start = mid;
		}else{
			end = mid;
		}
	}

	if(mlk(gheight, N, end) >= K)
	{
		return end;
	}else{
		return start;
	}
}

int main()
{
	while(input())
	{
		make_sa(gstr, gsa, grank, N);
		calc_ht(gstr, gsa, grank, gheight, N);
		printf("%d\n",calc_minmax());
	}

	return 0;
}
