#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<algorithm>

#define MAXN 100005
#define MAXM 200005
#define MID(l, r)	(((l)+(r))>>1)
#define LCHD(x)		((x)<<1)
#define RCHD(x)		(((x)<<1)+1)

struct NODE
{
	int head;
	int siz;	//子树尺寸
	int son;	//重儿子
	int par;	//父节点
	int dep;	//子树深度

	int w;		//映射在链中的位置
	int top;	//重链的链条头
}node[MAXN];
int enext[MAXM], edge[MAXM];
int N, M, ecnt, tz;

struct DATA
{
	int pos, type, cnt;
}data[MAXN*100];
int dcnt;

bool operator<(const DATA& a, const DATA& b)
{
	if(a.pos != b.pos)
		return a.pos < b.pos;
	else
		return a.cnt > b.cnt;
}

void addedge(int x, int y)
{
	edge[ecnt] = y;
	enext[ecnt] = node[x].head;
	node[x].head = ecnt;
	ecnt++;
	edge[ecnt] = x;
	enext[ecnt] = node[y].head;
	node[y].head = ecnt;
	ecnt++;
}

void DFS(int x)
{
	int j;
	node[x].siz = 1;
	node[x].son = 0;
	for(j=node[x].head;j>=0;j=enext[j])
	{
		if(edge[j] == node[x].par)
			continue;
		node[edge[j]].par = x;
		node[edge[j]].dep = node[x].dep + 1;
		DFS(edge[j]);
		if(node[edge[j]].siz > node[node[x].son].siz)
			node[x].son = edge[j];
		node[x].siz += node[edge[j]].siz;
	}
}

void build_tree(int x, int tp)
{
	int j;
	node[x].w = ++tz;	//分配编号
	node[x].top = tp;
	if(node[x].son > 0)
		build_tree(node[x].son, tp);
	for(j=node[x].head;j>=0;j=enext[j])
	{
		if(edge[j] == node[x].son || edge[j] == node[x].par)
			continue;
		build_tree(edge[j], edge[j]);
	}
}

//原则是先加后减
void adddata(int pos, int type, int cnt)
{
	data[dcnt].pos = pos;
	data[dcnt].type = type;
	data[dcnt].cnt = cnt;
	dcnt++;
}

//这里防止可能的问题
void breakdown(int va, int vb, int type)
{
	int f1 = node[va].top, f2 = node[vb].top;
	while(f1 != f2)
	{
		if(node[f1].dep < node[f2].dep)
		{
			std::swap(f1, f2);
			std::swap(va, vb);
		}
		adddata(node[va].w, type, -1);
		adddata(node[f1].w, type, 1);
		va = node[f1].par;
		f1 = node[va].top;
	}
	if(node[va].dep > node[vb].dep)
		std::swap(va, vb);
	adddata(node[vb].w, type, -1);
	adddata(node[va].w, type, 1);
}

int answer[MAXN];
class TREE
{
private:
	struct STNODE
	{
		int l, r;
		int maxp, maxn;
	}tree[MAXN<<3];
public:
	void init()
	{
		int i;
		ecnt = tz = dcnt = 0;
		build(1, 1, 100000);
		for(i=0;i<=N;i++)
		{
			node[i].head = -1;
			node[i].par = 0;
			node[i].son = 0;
		}
	}
	void build(int id, int l, int r)
	{
		int mid = MID(l, r);
		tree[id].maxp = l;
		tree[id].maxn = 0;
		tree[id].l = l;
		tree[id].r = r;
		if(l != r)
		{
			build(LCHD(id), l, mid);
			build(RCHD(id), mid+1, r);
		}
	}
	void update(int id, int pos, int cnt)
	{
		int mid = MID(tree[id].l, tree[id].r);
		if(tree[id].l == pos && tree[id].r == pos)
		{
			tree[id].maxn += cnt;
			return ;
		}
		if(pos <= mid)
			update(LCHD(id), pos, cnt);
		else
			update(RCHD(id), pos, cnt);
		if(tree[LCHD(id)].maxn >= tree[RCHD(id)].maxn)
		{
			tree[id].maxp = tree[LCHD(id)].maxp;
			tree[id].maxn = tree[LCHD(id)].maxn;
		}else{
			tree[id].maxp = tree[RCHD(id)].maxp;
			tree[id].maxn = tree[RCHD(id)].maxn;
		}
	}
	inline int query()
	{
		if(tree[1].maxn == 0)
			return 0;
		else
			return tree[1].maxp;
	}
} segtree;

bool domain()
{
	int a, b, i, t;
	scanf("%d%d",&N,&M);
	if(N == 0)
		return false;
	segtree.init();
	for(i=0;i<N-1;i++)
	{
		scanf("%d%d",&a,&b);
		addedge(a, b);
	}
	addedge(0, 1);
	DFS(1);
	build_tree(1, 1);
	for(i=0;i<M;i++)
	{
		scanf("%d%d%d",&a,&b,&t);
		breakdown(a, b, t);
	}
	std::sort(data, data+dcnt);
	for(i=0,a=0;i<=tz;i++)
	{
		while(a<dcnt && data[a].pos==i && data[a].cnt>0)
		{
			segtree.update(1, data[a].type, data[a].cnt);
			a++;
		}
		answer[i] = segtree.query();
		while(a<dcnt && data[a].pos==i && data[a].cnt<0)
		{
			segtree.update(1, data[a].type, data[a].cnt);
			a++;
		}
	}
	for(i=1;i<=N;i++)
	{
		printf("%d\n",answer[node[i].w]);
	}

	return true;
}

int main()
{
	while(domain());
	return 0;
}
