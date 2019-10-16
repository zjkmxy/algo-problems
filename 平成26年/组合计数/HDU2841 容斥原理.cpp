#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;

typedef signed long long int SINT;
#define MAXN 100005
#define MAXK 10

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
			ret -= l / lcm;
		else
			ret += l / lcm;
	}
	return ret;
}

SINT calc(int m, int n)
{
	int i;
	SINT ret = 0;
	if(n < m)
		swap(m, n);
	for(i=1;i<=m;i++)
	{
		ret += (n / i) * euler[i] + get_pair(i, n % i);
		//ret += get_pair(i, n);
	}
	return ret;
}

int main()
{
	int t, m, n;

	init();
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d",&m,&n);
		printf("%I64d\n",calc(m, n));
	}
	return 0;
}
