/*
给定L,R,N,求[1,N]内书写不包括[L,R]内任何质数的数字有多少个
思路：
1.一个质数是有价值的，仅当它不能被之前所有的质数表示。总数减去有价值的质数即可。
2.维护有效的前缀个数。
*/
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<string>
#include<cstdlib>
#include<queue>
#include<stack>
#include<utility>
#include<map>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;

const int MAXN = 1005;
const int MODN = 1000000007;
typedef long long LL;

char str[MAXN];
int arr[MAXN];
int len, l, r;
LL dp2[2][2][10001];  //位数，是否卡上界，任何前缀（0表示所有可能）
bool vis[10001], valprime[10001];
int pms[10001], pmc;

int dp_calc()
{
	int i, j, k;
	int fr, fw, tmp;

	memset(dp2[0], 0, sizeof(dp2[0]));
	dp2[0][1][0] = 1;//只有一个0
	for(fr=0,fw=1,i=1;i<=len;i++,fr^=1,fw^=1)
	{
		memset(dp2[fw], 0, sizeof(dp2[0]));
		for(k=1;k<=r;k++)
		{
			j = k % 10;
			dp2[fw][0][k] += dp2[fr][0][k / 10];
			if(j < arr[i])
			{	
				dp2[fw][0][k] += dp2[fr][1][k / 10];
			}else if(j == arr[i])
			{
				dp2[fw][1][k] += dp2[fr][1][k / 10];
			}
			dp2[fw][0][k] %= MODN;
			dp2[fw][1][k] %= MODN;
		}
		dp2[fw][0][0] = (dp2[fr][0][0] * 10 + dp2[fr][1][0] * arr[i]) % MODN;
		dp2[fw][1][0] = dp2[fr][1][0];
		for(k=l;k<=r;k++)
		{
			if(!valprime[k])
				continue;
			dp2[fw][0][0] -= dp2[fw][0][k];
			dp2[fw][1][0] -= dp2[fw][1][k];
			//维护合理的前缀
			for(tmp=10;tmp<k;tmp*=10)
			{
				if(k % tmp == k % (tmp/10))
					continue;
				dp2[fw][0][k%tmp] -= dp2[fw][0][k];
				dp2[fw][1][k%tmp] -= dp2[fw][1][k];
			}
			dp2[fw][0][k] = dp2[fw][1][k] = 0;
			if(dp2[fw][0][0] < 0)
				dp2[fw][0][0] += MODN;
			if(dp2[fw][1][0] < 0)
				dp2[fw][1][0] += MODN;
		}
	}
	return (dp2[fr][0][0] + dp2[fr][1][0] - 1 + MODN) % MODN;  //减去'0'
}

void pred()
{
	int i, j;
	vis[0] = vis[1] = true;
	for(i=2;i<=10000;i++)
	{
		if(vis[i])
			continue;
		pms[pmc++] = i;
		for(j=i+i;j<=10000;j+=i)
			vis[j] = true;
	}
}

bool incl(int a, int b)
{
	int p = 10;
	while(p <= b)
		p *= 10;
	while(a >= b)
	{
		if(a % p == b)
			return true;
		a /= 10;
	}
	return false;
}

void pre2()
{
	int i, j;
	memset(valprime, 0, sizeof(valprime));
	for(i=l;i<=r;i++)
	{
		if(vis[i])
			continue;
		valprime[i] = true;
		for(j=0;pms[j]<l;j++);
		while(pms[j] < i)
		{
			if(incl(i, pms[j]))
			{
				valprime[i] = false;
				break;
			}
			j++;
		}
	}
}

int brute()
{
	int x = atoi(str);
	int i, j, ret = 0;
	bool flag;

	for(i=1;i<=x;i++)
	{
		flag = false;
		for(j=l;j<=r&&j<=i;j++)
		{
			if(valprime[j] && incl(i, j))
			{
				flag = true;
				break;
			}
		}
		if(!flag)
			ret++;
	}
	return ret;
}

int main()
{
	int i, ret, cas = 0;
	pred();
	while(scanf("%d%d%s",&l,&r,str)!=EOF)
	{
		len = strlen(str);
		for(i=0;i<len;i++)
		{
			arr[i+1] = str[i] - '0';
		}
		pre2();
		ret = dp_calc();
		printf("Case #%d: %d\n",++cas,ret);
	}
	return 0;
}
