/*
有很多从磁盘读取数据的需求，包括顺序读取、随机读取。为了提高效率，需要人为安排磁盘读取。然而，在现实中，这种做法很复杂。我们考虑一个相对简单的场景。
磁盘有许多轨道，每个轨道有许多扇区，用于存储数据。当我们想在特定扇区来读取数据时，磁头需要跳转到特定的轨道、具体扇区进行读取操作。为了简单，我们假设磁头可以在某个轨道顺时针或逆时针匀速旋转，旋转一周的时间是360个单位时间。磁头也可以随意移动到某个轨道进行读取，每跳转到一个相邻轨道的时间为400个单位时间，跳转前后磁头所在扇区位置不变。一次读取数据的时间为10个单位时间，读取前后磁头所在的扇区位置不变。磁头同时只能做一件事：跳转轨道，旋转或读取。
现在，需要在磁盘读取一组数据，假设每个轨道至多有一个读取请求，这个读取的扇区是轨道上分布在 0到359内的一个整数点扇区，即轨道的某个360等分点。磁头的起始点在0轨道0扇区，此时没有数据读取。在完成所有读取后，磁头需要回到0轨道0扇区的始点位置。请问完成给定的读取所需的最小时间。
输入的第一行包含一个整数M（0<M<=10），表示测试数据的组数。
对于每组测试数据，第一行包含一个整数N（0<N<=50000），表示要读取的数据的数量。之后每行包含两个整数T和S（0<T<=1000000，0<= S<360），表示每个数据的磁道和扇区，磁道是按升序排列，并且没有重复。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 50005
#define MAXI 0x3fffffff
int T[MAXN], S[MAXN], n;
int dp[MAXN][2];
int dS[MAXN];
int DPminVAL[360], dp2[MAXN];  //dp2[i] = dp(i, i+1)

inline int dist(int i, int j)
{
	int ret;

	ret = labs(S[i] - S[j]);
	if(ret > 180)
		ret = 360 - ret;
	return ret;
}

inline int dist2(int p, int q)
{
	int ret;

	ret = labs(p - q);
	if(ret > 180)
		ret = 360 - ret;
	return ret;
}

int dpcalc()
{
	int i, j, k;
	int cur = 0;

	dp[0][0] = dist(0, 1);
	for(j=2;j<=n;j++, cur^=1)
	{
		k = dist(j-1, j);
		for(i=0;i<=j-2;i++)
		{
			dp[i][cur^1] = dp[i][cur] + k;
		}
		dp[j-1][cur^1] = MAXI;
		for(k=0;k<=j-2;k++)
		{
			i = dp[k][cur] + dist(k, j);
			if(i < dp[j-1][cur^1])
				dp[j-1][cur^1] = i;
		}
	}

	return dp[n-1][cur] + dist(n-1, n);
}

int calc2()
{
	int i, p, ans;

	for(i=0;i<360;i++)
		DPminVAL[i] = MAXI;
	dp2[0] = dist(0, 1);
	DPminVAL[0] = 0;
	for(i=1;i<n;i++)
	{
		ans = MAXI;
		for(p=0;p<360;p++)
		{
			ans = min(ans, DPminVAL[p] + dist2(p, S[i+1]));
		}
		dp2[i] = ans + dS[i];
		DPminVAL[S[i]] = min(DPminVAL[S[i]], dp2[i] - dS[i+1]);
	}

	return dp2[n-1] + dist(n-1, n);
}

int main()
{
	int i, t, ans;

	scanf("%d",&t);
	while(t--)
	{
		scanf("%d",&n);
		for(i=1;i<=n;i++)
		{
			scanf("%d%d",&T[i], &S[i]);
			dS[i] = dS[i-1] + dist(i-1, i);
		}
		if(n==1)
		{
			ans = T[n] * 800 + S[n] * 2 + 10;
		}else{
			//ans = dpcalc() + T[n] * 800 + 10 * n;
			ans = calc2() + T[n] * 800 + 10 * n;
		}
		printf("%d\n",ans);
	}
	return 0;
}
