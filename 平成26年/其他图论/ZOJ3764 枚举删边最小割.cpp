#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
#include<set>
using namespace std;

#define MAXM 1000
typedef long long int LLI;
const LLI INF = 0x3fffffffffffffff;
struct EDGE
{
	int a, b, f;
	bool operator<(const EDGE &other)const
	{
		return f > other.f;
	}
}edge[MAXM];
int N, M, K;

LLI solve(int st)
{
	int i, k = K;
	LLI ret = 0;
	for(i=0;i<M;i++)
	{
		if(((st>>edge[i].a) & 1)==((st>>edge[i].b) & 1))
			continue;
		if(k)
			--k;
		else
			ret += edge[i].f;
	}
	return ret;
}

bool domain()
{
	int i, tmp;
	LLI ans = INF;
	if(scanf("%d%d%d",&N,&M,&K)==EOF)
		return false;
	for(i=0;i<M;i++)
	{
		scanf("%d%d%d",&edge[i].a,&edge[i].b,&edge[i].f);
		edge[i].a--;
		edge[i].b--;
	}
	sort(edge, edge + M);
	tmp = 1 << (N - 1);
	for(i=1;i<tmp;i+=2)
		ans = min(ans, solve(i));
	printf("%lld\n",ans);
	return true;
}

int main()
{
	while(domain());
	return 0;
}
