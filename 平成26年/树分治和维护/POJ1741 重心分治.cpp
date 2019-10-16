#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

const int MAXN = 10005;
const int MAXM = MAXN<<1;
const int MAXI = 0x3fffffff;
typedef long long int LLI;

int ehead[MAXN], edge[MAXM], enext[MAXM], wei[MAXM], revs[MAXM], ecnt;
int subsz[MAXN], mson[MAXN];  //原始子树大小，自身做根的最大子子树
int vis[MAXN];
int n, k;

//计算重心
void get_cent(int cur, int par, int &gc, int n)
{
	int j, ed;

	subsz[cur] = 1;
	mson[cur] = 0;
	for(j=ehead[cur];j>=0;j=enext[j])
	{
		ed = edge[j];
		if(ed == par || wei[j] >= MAXI || vis[ed])
			continue;
		get_cent(ed, cur, gc, n);
		mson[cur] = max(mson[cur], subsz[ed]);
		subsz[cur] += subsz[ed];
	}
	mson[cur] = max(mson[cur], n - subsz[cur]);
	if(mson[cur] < mson[gc])
		gc = cur;
}

int depth[MAXN], dpc;
//计算深度，完成排序数组，同时更新子节点个数
void get_dep(int cur, int par, int dis, bool used)
{
	int ed, j;

	subsz[cur] = 1;
	for(j=ehead[cur];j>=0;j=enext[j])
	{
		ed = edge[j];
		if(ed == par || vis[ed])
			continue;
		if(wei[j]<MAXI && used)
		{
			depth[dpc] = dis + wei[j];
			dpc++;
		}
		get_dep(ed, cur, dis + wei[j], used && (wei[j]<MAXI));
		subsz[cur] += subsz[ed];
	}
}

//计算小于等于dis的有多少组
LLI get_cnt(int cur, int par, int dis)
{
	int i, j;
	LLI ans = 0;

	dpc = 1;
	depth[0] = 0;
	get_dep(cur, par, 0, true);
	sort(depth, depth+dpc);
	i = 0;
	j = dpc - 1;
	while(i < j)
	{
		if(depth[j] + depth[i] <= dis)
		{
			ans += (j-i);
			i++;
		}else{
			j--;
		}
	}
	return ans;
}

//计算最终结果
LLI calc(int cent, int dis)
{
	LLI ans;
	int j, ed, nct;

	vis[cent] = true;
	//到根的点对数
	ans = get_cnt(cent, -1, dis);
	for(j=ehead[cent];j>=0;j=enext[j])
	{
		ed = edge[j];
		if(wei[j] >= MAXI || vis[ed])
			continue;
		//减去子树的点对数
		ans -= get_cnt(ed, cent, dis-wei[j]*2);
		wei[j] = MAXI;
		wei[revs[j]] = MAXI;
		nct = 0;
		mson[0] = MAXI;
		get_cent(ed, -1, nct, subsz[ed]);
		//加上子树的结果
		ans += calc(nct, dis);
	}
	return ans;
}

bool input()
{
	int i, a, b, w;
	scanf("%d%d",&n,&k);
	if(n==0 && k==0)
		return false;

	ecnt = 0;
	memset(ehead, -1, sizeof(ehead));
	memset(vis, 0, sizeof(vis));
	for(i=1;i<n;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		edge[ecnt] = b;
		enext[ecnt] = ehead[a];
		ehead[a] = ecnt;
		wei[ecnt] = w;
		revs[ecnt] = ecnt+1;
		ecnt++;
		edge[ecnt] = a;
		enext[ecnt] = ehead[b];
		ehead[b] = ecnt;
		wei[ecnt] = w;
		revs[ecnt] = ecnt-1;
		ecnt++;
	}
	
	return true;
}

int main()
{
	int nct;
	LLI ans;

	while(input())
	{
		nct = 0;
		mson[0] = MAXI;
		get_cent(1, -1, nct, n);
		ans = calc(nct, k);
		printf("%lld\n",ans);
	}
	return 0;
}
