#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 502
#define MAXM 40002

int n, m;
int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos, root, rtcnt;
int cut;
int ehead[MAXN], edge[MAXM], enext[MAXM];

void Tarjan(int cur, int dep, int par)  //Tarjan算法查割点
{
	int i, j;

	DFN[cur] = low[cur] = dep;
	vis[cur] = 1;
	for(i=ehead[cur];i>=0;i=enext[i])
	{
		j = edge[i];
		if(vis[j] == 2)  //废除顶点
			continue;
		if(vis[j] == 0)
		{
			Tarjan(j, dep+1, cur);
			if(cur == root)
			{
				rtcnt++;
			}else{
				low[cur] = min(low[cur], low[j]);
				if(low[j] >= DFN[cur])
					cut = cur;  //此时cur是割点
				//if(low[j] > DFN[cur])  时cur是桥
			}
		}else if(j != par)
		{
			low[cur] = min(low[cur], DFN[j]);
		}
	}
}

bool judge()
{
	int j;

	memset(vis, 0, sizeof(vis));
	rtcnt = 0;
	root = 0;  //任选根
	cut = -1;
	Tarjan(root, 1, -1);
	if(cut >= 0)  //cut是割点
		return false;
	if(rtcnt > 1)  //根是割点
		return false;
	for(j=0;j<n;j++)
	{
		if(vis[j]==0)  //本身不连通
			return false;
	}

	return true;
}
/*
一个顶点u是割点，当且仅当满足(1)或(2)
  (1) u为树根，且u有多于一个子树。 
  (2) u不为树根，且满足存在(u,v)为树枝边(或称父子边，即u为v在搜索树中的父亲)，使得DFS(u)<=Low(v)。
一条无向边(u,v)是桥，当且仅当(u,v)为树枝边，且满足DFS(u)<Low(v)。

一个有桥的连通图，如何把它通过加边变成边双连通图？方法为首先求出所有的桥，然后删除这些桥边，剩下的每个连通块都是一个双连通子图。
把每个双连通子图收缩为一个顶点，再把桥边加回来，最后的这个图一定是一棵树，边连通度为1。
统计出树中度为1的节点的个数，即为叶节点的个数，记为leaf。则至少在树上添加(leaf+1)/2条边，就能使树达到边二连通，所以至少添加的边数就是(leaf+1)/2。
具体方法为，首先把两个最近公共祖先最远的两个叶节点之间连接一条边，这样可以把这两个点到祖先的路径上所有点收缩到一起，因为一个形成的环一定是双连通的。
然后再找两个最近公共祖先最远的两个叶节点，这样一对一对找完，恰好是(leaf+1)/2次，把所有点收缩到了一起。
*/
