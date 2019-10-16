/*
问题：A和B轮流操作，A每次可以选至多一个单位造成1点伤害，B每次对所有单位造成1点伤害，问A最多有多少个人头
解法：
因为对相同HP的敌人，A在最后至多只能拿一个人头，所以A需要让敌人的HP能级分散开来。每次使一个单位降低一个能级就需要多耗费1点能量。
显然，低HP的敌人不打而将高HP的敌人降低至这个能级再收是没有意义的，这是第一个贪心策略。
于是我们可以维护每个HP值是否利用到了作为依据，计算得每个敌人实际攻击所需要到达的能级，也就算出了收这个人头需要的能量。
然后，相当于给定一个最高能量，和每个人头需要的能量，简单贪心就能求出最多的人头个数了。
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

#define MAXN 1005

int arr[MAXN];
bool used[MAXN];
int fee[MAXN];
int n;
void domain()
{
	int i, j, ans = 0, ene, mfee = 0;
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		scanf("%d",&arr[i]);
	}
	sort(arr+1, arr+n+1);
	memset(used, 0, sizeof(used));
	memset(fee, 0, sizeof(fee));
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=arr[i]&&used[arr[i]-j+1];j++);
		used[arr[i]-j+1] = true;
		if(j > arr[i])
			continue;
		fee[j]++;
		mfee = max(mfee, j);
	}
	ene = arr[n];
	for(i=1;i<=mfee;i++)
	{
		if(ene > i * fee[i])
		{
			ene -= i * fee[i];
			ans += fee[i];
		}else{
			ans += ene / i;
			break;
		};
	}
	printf("%d\n",ans);
}

int main()
{
	int t, cas = 0;

	scanf("%d",&t);
	for(cas = 1;cas <= t; cas++)
	{
		printf("Case #%d: ",cas);
		domain();
	}
	return 0;
}
