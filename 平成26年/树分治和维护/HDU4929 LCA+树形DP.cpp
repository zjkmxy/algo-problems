/*
给定一颗字符树T和短字符串str，问某条路径上有多少次str的出现

做法：
考虑起点u和重点v的LCA为w，v是w的p儿子的后代，f是w的父亲
则答案是枚举x后cnt[u,w][1,x] * cnt[p,v][x+1,len]之和
设dp[z][x][c]为z到根的路径上出现x..c的次数（包含正着的和倒着的）
则cnt[u,w][1,x] = dp[u][1][x] - dp[f][1][x] - sum{cnt[u,w][1,a] * dp[w][a+1][x]}
时间复杂度是O(N*L*L + Q*L*L)
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MODN 10007
#define LOGMN 30
#define MAXN 50005
#define MAXL 35
const double lg2 = log(2.0);

struct NODE
{
	int head;
	char ch;
	int pfst;	//首次出现
	int par;
	int dp[MAXL][MAXL];//该节点到根节点的路径上[a,b]有多少个
	int depth;
};
struct EDGE
{
	int a, b;
	int next;
	bool out;
};
char str[MAXN];
int len;
int n, q;

class TREE
{
private:
	NODE node[MAXN];
	EDGE edge[MAXN<<1];
	int depth[MAXN<<1], anc[MAXN<<1], cnt, sig;
	int rmqarr[MAXN<<1][LOGMN], mvarr[MAXN][LOGMN];
	int ecnt;
	int tmpal[MAXL], tmpar[MAXL];

	//得到欧拉序列
	int DST, DFS_I[MAXN], DFS_ST[MAXN], DFS_NOW[MAXN];
	void DFS(int st)
	{
		int i, now;

		DST = 0;
begin1:
		cnt++;
		node[st].pfst = cnt;
		sig++;
		now = sig;
		anc[now] = st;
		depth[cnt] = now;
		for(i=node[st].head;i>=0;i=edge[i].next)
		{
			if(edge[i].b == node[st].par)
				continue;
			edge[i].out = true;
			node[edge[i].b].par = st;
			node[edge[i].b].depth = node[st].depth + 1;
			//DFS(edge[i].b);

			DFS_I[DST] = i;
			DFS_NOW[DST] = now;
			DFS_ST[DST] = st;
			st = edge[i].b;
			DST++;
			goto begin1;
ret1:
			cnt++;
			depth[cnt] = now;
		}
do_ret1:
		DST--;
		if(DST < 0)
			return ;
		i = DFS_I[DST];
		now = DFS_NOW[DST];
		st = DFS_ST[DST];
		goto ret1;
	};
	//DP预先计算，str移位至1~len
	void dp_pre()
	{
		int p, a, b, i, l;

		memset(node[1].dp, 0, sizeof(node[1].dp));
		for(a=1;a<=len;a++)
			node[1].dp[a][a] = (node[1].ch == str[a]);
		for(i=2;i<=n;i++)
		{
			p = anc[i];
			for(l=0;l<len;l++)
			{
				for(a=1;a<=len-l;a++)
				{
					b = a + l;
					node[p].dp[a][b] = node[node[p].par].dp[a][b];
					if(node[p].ch == str[a])
					{
						if(a == b)
							node[p].dp[a][b]++;
						else
							node[p].dp[a][b] += node[node[p].par].dp[a+1][b];
						if(node[p].dp[a][b] >= MODN)
							node[p].dp[a][b] -= MODN;
					}
				}
				for(a=len;a>=1+l;a--)
				{
					b = a - l;
					node[p].dp[a][b] = node[node[p].par].dp[a][b];
					if(node[p].ch == str[a])
					{
						if(a == b)
							node[p].dp[a][b]++;
						else
							node[p].dp[a][b] += node[node[p].par].dp[a-1][b];
						if(node[p].dp[a][b] >= MODN)
							node[p].dp[a][b] -= MODN;
					}
				}
			}
		}
	}
	inline int get_dp(int p, int a, int b)
	{
		return node[p].dp[a][b];
	}
	//u..w的1~x
	int query_l(int u, int w, int x)
	{
		int f = node[w].par, i, tmp;
		if(tmpal[x] >= 0)
			return tmpal[x];
		tmpal[x] = get_dp(u, 1, x);
		tmpal[x] = (tmpal[x] - get_dp(f, 1, x));
		while(tmpal[x] < 0)
			tmpal[x] += MODN;
		for(i=1;i<x;i++)
		{
			tmp = query_l(u, w, i) * get_dp(f, i+1, x) % MODN;
			tmpal[x] = (tmpal[x] - tmp);
			while(tmpal[x] < 0)
				tmpal[x] += MODN;
		}
		return tmpal[x];
	};
	//w..v的x~len
	int query_r(int w, int v, int x)
	{
		int f = node[w].par, i, tmp;
		if(tmpar[x] >= 0)
			return tmpar[x];
		tmpar[x] = get_dp(v, len, x);
		tmpar[x] = (tmpar[x] - get_dp(f, len, x));
		while(tmpar[x] < 0)
			tmpar[x] += MODN;
		for(i=len;i>x;i--)
		{
			tmp = query_r(w, v, i) * get_dp(f, i-1, x) % MODN;
			tmpar[x] = (tmpar[x] - tmp);
			while(tmpar[x] < 0)
				tmpar[x] += MODN;
		}
		return tmpar[x];
	};
public:
	TREE(){};
	//初始化
	void init(int n)
	{
		int i;
		ecnt = 0;
		for(i=0;i<=n;i++)
		{
			node[i].head = -1;
			node[i].depth = 0;
			memset(node[i].dp, -1, sizeof(node[i].dp));
		}
	};
	//加边
	void addedge(int a, int b)
	{
		edge[ecnt].a = a;
		edge[ecnt].b = b;
		edge[ecnt].out = false;
		edge[ecnt].next = node[a].head;
		node[a].head = ecnt;
		ecnt++;
		edge[ecnt].a = b;
		edge[ecnt].b = a;
		edge[ecnt].out = false;
		edge[ecnt].next = node[b].head;
		node[b].head = ecnt;
		ecnt++;
	}
	//LCA预备
	void pre_lca()
	{
		int i, j, m;
		cnt = sig = 0;
		node[1].par = 0;
		DFS(1);
		dp_pre();
		m = (int)(floor(log((double)cnt)/lg2));
		for(i=1;i<=cnt;i++)
		{
			rmqarr[i][0] = depth[i];
		}
		for(i=1;i<=n;i++)
		{
			mvarr[i][0] = node[i].par;
		}
		for(j=1;j<=m;j++)
		{
			for(i=1;i<=cnt-(1<<(j-1));i++)
			{
				rmqarr[i][j]=min(rmqarr[i][j-1],rmqarr[i + (1<<(j-1))][j-1]);
			}
			for(i=1;i<=n;i++)
			{
				mvarr[i][j] = mvarr[mvarr[i][j-1]][j-1];
			}
		}
	};
	//LCA计算
	int get_lca(int a, int b)
	{
		int aa = node[a].pfst, bb = node[b].pfst, m;
		if(aa > bb)
			std::swap(aa, bb);
		m = (int)(floor(log((double)(bb-aa+1))/lg2));
		return anc[min(rmqarr[aa][m], rmqarr[bb-(1<<m)+1][m])];
	};
	//移动计算
	int move(int p, int st)
	{
		int i;
		for(i=0;st>0;i++,st>>=1)
		{
			if(st & 1)
				p = mvarr[p][i];
		}
		return p;
	}
	inline void setch(int p, char ch)
	{
		node[p].ch = ch;
	};
	int query(int u, int v)
	{
		int x, ans = 0, w, p;

		memset(tmpal, -1, sizeof(tmpal));
		memset(tmpar, -1, sizeof(tmpar));
		w = get_lca(u, v);
		if(v == w)
			return query_l(u, v, len);
		p = move(v, node[v].depth - node[w].depth - 1);
		ans = (query_l(u, w, len) + query_r(p, v, 1));
		if(ans >= MODN)
			ans -= MODN;
		for(x=1;x<len;x++)
		{
			ans += query_l(u, w, x) * query_r(p, v, x+1) % MODN;
			if(ans >= MODN)
				ans -= MODN;
		}
		return ans;
	};
};
TREE tree;

void domain()
{
	int i, a, b;
	scanf("%d%d",&n,&q);
	tree.init(n);
	for(i=1;i<n;i++)
	{
		scanf("%d%d",&a,&b);
		tree.addedge(a, b);
	}
	scanf("%s",str+1);
	len = strlen(str+1);
	for(i=1;i<=len;i++)
	{
		tree.setch(i, str[i]);
	}
	scanf("%s",str+1);
	len = strlen(str+1);
	tree.pre_lca();
	for(i=0;i<q;i++)
	{
		scanf("%d%d",&a,&b);
		printf("%d\n",tree.query(a, b));
	}
}

int main()
{
	int t;

	scanf("%d",&t);
	while(t--)
	{
		domain();
	}
	return 0;
}
