//ZOJ2112  1K 1660ms
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>

#define MAXN   50002

struct DATA{
	int pos;
	int num;
};
typedef DATA *PDATA;
DATA data[MAXN];
int n, m;

int comp(const void* A, const void *B)
{
	PDATA a,b;
	a = (PDATA)A;
	b = (PDATA)B;
	return (a->num - b->num);
}

int search(int st, int ed, int k)
{
	int i;

	for(i=0;i<n;i++)
	{
		if((data[i].pos >= st)&&(data[i].pos <= ed))
		{
			k--;
			if(k == 0)
				return data[i].num;
		}
	}

	return 0;
}

bool update(int pos, int val)
{
	DATA tmp;
	int i, j;

	for(i=0;i<n;i++)
	{
		if(data[i].pos == pos)
		{
			break;
		}
	}
	if(i == n)
		return false;
	if(data[i].num == val)
		return true;
	if(data[i].num < val)
	{
		data[i].num = val;
		tmp = data[i];
		for(j=i+1;j<n;j++)
		{	
			if(data[j].num >= val)
				break;
			else
				data[j-1] = data[j];
		}
		j--;
		data[j] = tmp;
	}else{
		data[i].num = val;
		tmp = data[i];
		for(j=i-1;j>=0;j--)
		{
			if(data[j].num <= val)
				break;
			else
				data[j+1] = data[j];
		}
		j++;
		data[j] = tmp;
	}
	return true;
}

void calc()
{
	int i, a, b, c;
	char op[3];

	scanf("%d%d",&n,&m);
	for(i=0;i<n;i++)
	{
		scanf("%d",&data[i].num);
		data[i].pos = i + 1;
	}
	qsort(data, n, sizeof(DATA), comp);
	for(i=0;i<m;i++)
	{
		scanf("%s",op);
		if(op[0] == 'Q')
		{
			scanf("%d%d%d",&a,&b,&c);
			printf("%d\n",search(a, b, c));
		}else{
			scanf("%d%d",&a,&b);
			update(a, b);
		}
	}
}

int main()
{
	int t, i;

	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		calc();
	}
	return 0;
}
