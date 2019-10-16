/*
�кܶ�Ӵ��̶�ȡ���ݵ����󣬰���˳���ȡ�������ȡ��Ϊ�����Ч�ʣ���Ҫ��Ϊ���Ŵ��̶�ȡ��Ȼ��������ʵ�У����������ܸ��ӡ����ǿ���һ����Լ򵥵ĳ�����
�������������ÿ�������������������ڴ洢���ݡ������������ض���������ȡ����ʱ����ͷ��Ҫ��ת���ض��Ĺ���������������ж�ȡ������Ϊ�˼򵥣����Ǽ����ͷ������ĳ�����˳ʱ�����ʱ��������ת����תһ�ܵ�ʱ����360����λʱ�䡣��ͷҲ���������ƶ���ĳ��������ж�ȡ��ÿ��ת��һ�����ڹ����ʱ��Ϊ400����λʱ�䣬��תǰ���ͷ��������λ�ò��䡣һ�ζ�ȡ���ݵ�ʱ��Ϊ10����λʱ�䣬��ȡǰ���ͷ���ڵ�����λ�ò��䡣��ͷͬʱֻ����һ���£���ת�������ת���ȡ��
���ڣ���Ҫ�ڴ��̶�ȡһ�����ݣ�����ÿ�����������һ����ȡ���������ȡ�������ǹ���Ϸֲ��� 0��359�ڵ�һ���������������������ĳ��360�ȷֵ㡣��ͷ����ʼ����0���0��������ʱû�����ݶ�ȡ����������ж�ȡ�󣬴�ͷ��Ҫ�ص�0���0������ʼ��λ�á�������ɸ����Ķ�ȡ�������Сʱ�䡣
����ĵ�һ�а���һ������M��0<M<=10������ʾ�������ݵ�������
����ÿ��������ݣ���һ�а���һ������N��0<N<=50000������ʾҪ��ȡ�����ݵ�������֮��ÿ�а�����������T��S��0<T<=1000000��0<= S<360������ʾÿ�����ݵĴŵ����������ŵ��ǰ��������У�����û���ظ���
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
