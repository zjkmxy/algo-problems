#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;

typedef __int64 SINT;
#define MAXN 200005
#define MAXK 20

bool vis[MAXN];
int euler[MAXN];
int prime[MAXN][MAXK];
int pcnt[MAXN];
int k;

SINT gcd(SINT a,SINT b)
{
	if(a < b)
		swap(a, b);
    return b==0?a:gcd(b,a%b);
}

void init()
{
	int i, j = 0;
	for(i=1;i<MAXN;i++)
	{
		euler[i] = i;
	}
	for(i=2;i<MAXN;i++)
	{
		if(vis[i])
			continue;
		k++;
		for(j=i; j < MAXN; j+=i)
		{
			prime[j][pcnt[j]] = i;
			pcnt[j]++;
			euler[j] = euler[j] / i * (i-1);
			vis[j] = true;
		}
	}
}

int get_pair(int x, int l)
{
	int cn, i, j, k, lcm;
	int ret = l;

	for(i=0,cn=0;i<pcnt[x];i++,cn++)
	{
		if(prime[x][i] > l)
			break;
	}
	if(cn <= 0)
		return l;
	for(i=1;i<(1<<cn);i++)
	{
		k = 0;
		lcm = 1;
		for(j=0;j<cn;j++)
		{
			if((i & (1<<j)) == 0)
				continue;
			k++;
			lcm *= prime[x][j];
		}
		if(k & 1)
			ret -= (l / lcm);
		else
			ret += (l / lcm);
	}
	return ret;
}

SINT calc(int p, int q, int l, int r, int md)
{
	int k, l2, r2, k2;
	SINT ret = 0;

	for(k=0;k<=q;k+=md)
	{
		if(k < p)
			continue;
		k2 = k / md;
		l2 = max(l, k) / md - 1;
		r2 = r / md;
		ret += (SINT)(r2 / k2) * (SINT)euler[k2] + get_pair(k2, r2 % k2);
		ret -= (SINT)(l2 / k2) * (SINT)euler[k2] + get_pair(k2, l2 % k2);
	}
	return ret;
}

int main()
{
	int t, a, b, c, d, k, i;

	init();
	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		scanf("%d%d%d%d%d",&a,&b,&c,&d,&k);
		if(d < b)
			swap(b, d);
		if(k == 0)
		{
			printf("Case %d: 0\n", i);
			continue;
		}
		printf("Case %d: %I64d\n",i,calc(a, b, c, d, k));
	}
	
	return 0;
}
