#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
using namespace std;

#define MAXN 505
#define MAXI 0x3fffffff

int mat[MAXN][MAXN];
int wage[MAXN];
bool vis[MAXN];
bool used[MAXN];
int n, m;
int s, t, mcut;

//p - 次数
int Prim(int p)
{
	int i,j,m,v;
	int max;

	t = 1;
	for(i=1;i<=n;i++)          //构建临时树
	{
		wage[i] = mat[1][i];
		vis[i] = false;
	}
	wage[1] = 0;
	vis[1] = true;
	for(i=1;i<=n-p;i++)
	{
		max = 0;
		for(j=1;j<=n;j++)		//寻找最大联通数
		{
			if(wage[j] >= max && !vis[j] && !used[j])
			{
				m = j;
				max = wage[j];
			}
		}
		if(t==m)
			return wage[t];
		s = t;
		t = m;
		vis[m] = true;
		for(j=1;j<=n;j++)		//更新wage
		{
			if(!vis[j] && !used[j] && j!=m)
			{
				wage[j] += mat[m][j];
			}
		}
	}

	return wage[t];
}

int Stoer_Wagner()
{
	int i, j;

	mcut = MAXI;
	memset(used, 0, sizeof(used));
	for(i=1;i<=n-1;i++)
	{
		mcut = min(mcut, Prim(i));
		if(mcut == 0)
			return 0;
		used[t] = true;
		for(j=1;j<=n;j++)
		{
			if(!used[j] && j!=s)
			{
				mat[j][s] += mat[j][t];
				mat[s][j] += mat[t][j];
			}
		}
	}
	return mcut;
}

int main()
{
	int a, b, c, i;

	while(scanf("%d%d",&n,&m)!=EOF)
	{
		memset(mat, 0, sizeof(mat));
		for(i=0;i<m;i++)
		{
			scanf("%d%d%d",&a,&b,&c);
			a++;
			b++;
			mat[a][b] += c;
			mat[b][a] += c;
		}
		printf("%d\n",Stoer_Wagner());
	}
	return 0;
}
