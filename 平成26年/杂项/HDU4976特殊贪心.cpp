/*
���⣺A��B����������Aÿ�ο���ѡ����һ����λ���1���˺���Bÿ�ζ����е�λ���1���˺�����A����ж��ٸ���ͷ
�ⷨ��
��Ϊ����ͬHP�ĵ��ˣ�A���������ֻ����һ����ͷ������A��Ҫ�õ��˵�HP�ܼ���ɢ������ÿ��ʹһ����λ����һ���ܼ�����Ҫ��ķ�1��������
��Ȼ����HP�ĵ��˲��������HP�ĵ��˽���������ܼ�������û������ģ����ǵ�һ��̰�Ĳ��ԡ�
�������ǿ���ά��ÿ��HPֵ�Ƿ����õ�����Ϊ���ݣ������ÿ������ʵ�ʹ�������Ҫ������ܼ���Ҳ��������������ͷ��Ҫ��������
Ȼ���൱�ڸ���һ�������������ÿ����ͷ��Ҫ����������̰�ľ������������ͷ�����ˡ�
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
