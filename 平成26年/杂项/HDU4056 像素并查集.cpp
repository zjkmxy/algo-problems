#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
#include<cctype>
using namespace std;

#define MAXN 200005
#define MID(l, r) (((l)+(r))>>1)
#define LCHD(id)  ((id)<<1)
#define RCHD(id)  (((id)<<1)+1)

void assert(bool exp)
{
	if(!exp)
	{
		int x = 0;
		exp /= x;
	}
}
int anscnt[11];

struct DATA
{
	char str[12];
	int a, b, c, d;
	int col;
}arr[MAXN];
int N, M, Q;

bool split(int pos, int &l, int &r, int x)
{
	int tmp;
	switch(arr[pos].str[0])
	{
	case 'C':
		tmp = labs(x - arr[pos].a);
		if(tmp > arr[pos].c)
			return false;
		tmp = floor(sqrt(double(arr[pos].c*arr[pos].c - tmp*tmp))+1e-8);
		l = arr[pos].b - tmp;
		r = arr[pos].b + tmp;
		break;
	case 'R':
		if(x < arr[pos].a || x >= arr[pos].a + arr[pos].c)
			return false;
		l = arr[pos].b;
		r = arr[pos].b + arr[pos].d - 1;
		break;
	case 'D':
		tmp = labs(x - arr[pos].a);
		if(tmp > arr[pos].c)
			return false;
		tmp = arr[pos].c - tmp;
		l = arr[pos].b - tmp;
		r = arr[pos].b + tmp;
		break;
	case 'T':
		if(x < arr[pos].a || x > arr[pos].a + (arr[pos].c-1) / 2)
			return false;
		tmp = arr[pos].c - 2 * (x - arr[pos].a);
		l = arr[pos].b - tmp / 2;
		r = arr[pos].b + tmp / 2;
		break;
	}
	return true;
}

void inputobj(int pos)
{
	scanf("%s%d%d%d",&arr[pos].str,&arr[pos].a,&arr[pos].b,&arr[pos].c);
	if(arr[pos].str[0] == 'R')
		scanf("%d",&arr[pos].d);
	scanf("%d",&arr[pos].col);
}

int parset[MAXN], vis[MAXN];
void init()
{
	int i;
	for(i=0;i<=M;i++)
	{
		parset[i] = i;
		vis[i] = false;
	}
}

int find(int x)
{
	if(parset[x] != x)
		parset[x] = find(parset[x]);
	return parset[x];
}
void merge(int a, int b)
{
	a = parset[a];
	b = parset[b];
	if(a == b)
		return ;
	if(a > b)
		std::swap(a, b);
	parset[a] = b;
}

bool domain()
{
	int i, j, k, l, r, p, q;
	if(scanf("%d%d%d",&N,&M,&Q)==EOF)
		return false;
	for(i=0;i<Q;i++)
		inputobj(i);
	for(i=0;i<=9;i++)
		anscnt[i] = 0;
	for(j=0;j<N;j++)
	{
		init();
		for(k=Q-1;k>=0;k--)
		{
			if(!split(k, l, r, j))
				continue;
			if(l < 0)
				l = 0;
			if(r >= M)
				r = M-1;
			p = find(r);
			for(i=l;i<=r;i=q+1)
			{
				q = find(i);
				if(!vis[q])
					anscnt[arr[k].col]++;
				vis[q] = true;
				parset[q] = p;
			}
		}
	}
	for(i=1;i<=9;i++)
	{
		printf(i==9?"%d\n":"%d ",anscnt[i]);
	}
	return true;
}

int main()
{
	while(domain());
	return 0;
}
