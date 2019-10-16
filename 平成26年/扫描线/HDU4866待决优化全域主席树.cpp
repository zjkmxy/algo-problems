#include <iostream>
#include <cmath>
#include <cstdio>
#include<cstring>
#include<algorithm>
#include<cstdlib>
#include<vector>
using namespace std;

#define MID(l, r) (((l) + (r)) >> 1)
#define MAXN 100005
#define MAXK 100005
#define MAXD 10000000
#define MAXT 6000005
typedef long long LLI;

struct NODE
{
	int lchd;  //左右孩子指针，-1不存在，-2待决（0节点）
	int rchd;
	LLI sum;   //距离的和
	int cnt;   //个数的和
}tree[MAXT];

int tcnt;
int N, M, X, P;
int root[MAXK];

struct SEG
{
	int L, D;
	bool add;
	bool operator<(const SEG other)const
	{
		if(L != other.L)
			return L < other.L;
		return add > other.add;
	};
}segs[MAXN<<1];
int nseg;

//建树：使用待决优化
int build_tree()
{
	tcnt = 1;
	tree[0].sum = 0;
	tree[0].cnt = 0;
	tree[0].lchd = tree[0].rchd = -2;
	return 0;
}

//上推送：利用子节点信息计算父节点
inline void pop_up(int id)
{
	if(id < 0)
		return ;
	tree[id].sum = 0;
	if(tree[id].lchd >= 0)
		tree[id].sum += tree[tree[id].lchd].sum;
	if(tree[id].rchd >= 0)
		tree[id].sum += tree[tree[id].rchd].sum;
	tree[id].cnt = 0;
	if(tree[id].lchd >= 0)
		tree[id].cnt += tree[tree[id].lchd].cnt;
	if(tree[id].rchd >= 0)
		tree[id].cnt += tree[tree[id].rchd].cnt;
}

//修改：插入或者删除线段
int modify(int id, int L, int R, int D, int cnt)
{
	int ret = tcnt;
	int mid = MID(L, R);

	tcnt++;
	if(L == D && R == D)
	{
		tree[ret].lchd = tree[ret].rchd = -1;
		tree[ret].sum = D * cnt;
		if(id >= 0)
			tree[ret].sum  += tree[id].sum;
		tree[ret].cnt = cnt;
		if(id >= 0)
			tree[ret].cnt  += tree[id].cnt;
		return ret;
	}
	if(id >= 0)
		tree[ret] = tree[id];
	else{
		tree[ret].lchd = tree[ret].rchd = -2;
		tree[ret].cnt = 0;
		tree[ret].sum = 0;
	}
	//注意这里不要写tree[id].lchd
	if(D <= MID(L, R))
	{
		tree[ret].lchd = modify(tree[ret].lchd, L, mid, D, cnt);
	}else{
		tree[ret].rchd = modify(tree[ret].rchd, mid+1, R, D, cnt);
	}
	pop_up(ret);
	return ret;
}

LLI query(int id, int L, int R, int k)
{
	int cnt1 = 0;
	int mid = MID(L, R);
	
	if(id < 0)
		return 0;
	if(tree[id].cnt <= k)
		return tree[id].sum;
	//重复覆盖的位置
	if(tree[id].lchd == -1)
	{
		return L * k;
	}
	if(tree[id].lchd >= 0)
		cnt1 = tree[tree[id].lchd].cnt;
	if(cnt1 > k)
	{
		return query(tree[id].lchd, L, mid, k);
	}else{
		return query(tree[id].lchd, L, mid, cnt1) + query(tree[id].rchd, mid+1, R, k-cnt1);
	}
}

bool input()
{
	int i, j;
	int l, r, d;

	nseg = 0;
	if(!(cin >> N >> M >> X >> P))
		return false;
	for(i=0;i<N;i++)
	{
		cin >> l >> r >> d;
		segs[nseg].L = l;
		segs[nseg].D = d;
		segs[nseg].add = true;
		nseg++;
		segs[nseg].L = r;
		segs[nseg].D = d;
		segs[nseg].add = false;
		nseg++;
	}
	sort(segs, segs+nseg);
	root[0] = build_tree();
	root[1] = root[0];
	for(i=1, j=0;i<=X;i++)
	{
		while(j<nseg&&(segs[j].L < i || (segs[j].L == i && segs[j].add)))
		{
			root[i] = modify(root[i], 1, MAXD, segs[j].D, segs[j].add?1:-1);
			j++;
		}
		root[i+1] = root[i];
		while(j<nseg&&(segs[j].L == i && !segs[j].add))
		{
			root[i+1] = modify(root[i+1], 1, MAXD, segs[j].D, segs[j].add?1:-1);
			j++;
		}
	}

	return true;
}

void domain()
{
	int i, x, a, b, c;
	LLI pre = 1;

	for(i=0;i<M;i++)
	{
		cin >> x >> a >> b >> c;
		if(pre > P)
			pre = 2 * query(root[x], 1, MAXD, (LLI)(a * pre + b) % c);
		else
			pre = query(root[x], 1, MAXD, (LLI)(a * pre + b) % c);
		cout << pre << endl;
	}
}

int main()
{
	while(input())
	{
		domain();
	}
	return 0;
}
