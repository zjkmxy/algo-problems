#include<stdio.h>
#include<stdlib.h>

#define MAXN 10000
#define MAXK 60
#define MAXI 0x3fffffffffffffff

#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)>(y)?(x):(y))
typedef long long int LLI;
typedef struct _DATA
{
	LLI ori;
	LLI rev;
}DATA, *PDATA;
DATA arr[MAXN];
LLI c[MAXN], d[MAXN];
int N, K;

void input_one(int cnt)
{
	char str[MAXK+1];
	int i;
	LLI tmp, base;

	arr[cnt].ori = 0;
	arr[cnt].rev = 0;
	scanf("%s",&str);
	for(i=K-1,base=1;i>=0;i--,base<<=1)
	{
		if(str[i] == '0')
		{
			arr[cnt].ori += base;
		}
	}
	scanf("%s",&str);
	for(i=K-1,base=1;i>=0;i--,base<<=1)
	{
		if(str[i] == '0')
		{
			arr[cnt].rev += base;
		}
	}
	if(arr[cnt].rev < arr[cnt].ori)
	{
		tmp = arr[cnt].ori;
		arr[cnt].ori = arr[cnt].rev;
		arr[cnt].rev = tmp;
	}
}

int comp(const void* A, const void* B)
{
	if(((PDATA)A)->ori > ((PDATA)B)->ori)
		return 1;
	else if(((PDATA)A)->ori < ((PDATA)B)->ori)
		return -1;
	else return 0;
}

void input()
{
	int i;
	scanf("%d%d",&N,&K);
	for(i=0;i<N;i++)
		input_one(i);
}

//这种算法并不是唯一的
void domain()
{
	int i;
	LLI mint, maxt, cmin;
	LLI ret1, ret2;
	LLI min1, min2, max1, max2, minp, maxp;

	if(N == 1)
	{
		printf("0 0\n");
		return ;
	}
	qsort(arr, N, sizeof(DATA), comp);
	//全部最小
	mint = arr[0].ori;
	maxt = arr[N-1].ori;
	ret1 = maxt - mint;
	cmin = MAXI;
	ret2 = ret1;
	//中间取法
	for(i=0;i<N-1;i++)
	{
		cmin = MIN(cmin, arr[i].rev);
		maxt = MAX(maxt, arr[i].rev);
		mint = arr[i+1].ori;
		if(mint > cmin)
		{
			ret1 = MAX(ret1, maxt - cmin);
			ret2 = MIN(ret2, maxt - cmin);
			break;
		}else{
			ret1 = MAX(ret1, maxt - mint);
			ret2 = MIN(ret2, maxt - mint);
		}
	}
	//全部最大
	mint = arr[0].rev;
	maxt = arr[0].rev;
	for(i=0;i<N;i++)
	{
		mint = MIN(mint, arr[i].rev);
		maxt = MAX(maxt, arr[i].rev);
	}
	ret1 = MAX(ret1, maxt - mint);
	ret2 = MIN(ret2, maxt - mint);

	//大数的取法
	min1 = arr[0].ori;
	min2 = 0x3fffffff;
	max1 = arr[0].rev;
	max2 = 0;
	minp = maxp = 0;
	for(i=1;i<N;i++)
	{
		if(arr[i].ori <= min1)
		{
			min2 = min1;
			min1 = arr[i].ori;
			minp = i;
		}else if(arr[i].ori <= min2)
		{
			min2 = arr[i].ori;
		}
		if(arr[i].rev >= max1)
		{
			max2 = max1;
			max1 = arr[i].rev;
			maxp = i;
		}else if(arr[i].rev >= max2)
		{
			max2 = arr[i].rev;
		}
	}
	if(minp != maxp)
	{
		ret1 = max1 - min1;
	}else{
		ret1 = MAX(max2 - min1, max1 - min2);
	}

	printf("%lld %lld\n",ret1,ret2);
}

int main()
{
	int T, i;
	scanf("%d",&T);
	for(i=1;i<=T;i++)
	{
		input();
		printf("Case #%d: ",i);
		domain();
	}
	return 0;
}