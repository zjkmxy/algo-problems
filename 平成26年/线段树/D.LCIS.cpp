//和C类似，左右中思想
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 100005
#define MAXT 1000005
#define LCHD(x) (x << 1)
#define RCHD(x) ((x << 1) + 1)
#define LEN(x)  (tree[x].right - tree[x].left + 1)

struct Tree{
	int left ,right;	
	int lmax, rmax, allmax; //左始LCS,右终LCS,段最大LCS
}tree[MAXT];
int num[MAXN], n, m;

void build_tree(int id,int l,int r)
{
	int mid = (l + r) >> 1;
	tree[id].left = l;
	tree[id].right = r;
	if (l==r){
		tree[id].lmax = tree[id].rmax = tree[id].allmax = 1;
		return;
	}
	build_tree(LCHD(id),l,mid);
	build_tree(RCHD(id),mid+1,r);
	tree[id].lmax = tree[LCHD(id)].lmax;
	tree[id].rmax = tree[RCHD(id)].rmax;
	tree[id].allmax = max(tree[LCHD(id)].allmax, tree[RCHD(id)].allmax);
	if(num[mid] < num[mid+1])
	{
		if(tree[id].lmax == LEN(LCHD(id)))
			tree[id].lmax += tree[RCHD(id)].lmax;
		if(tree[id].rmax == LEN(RCHD(id)))
			tree[id].rmax += tree[LCHD(id)].rmax;
		tree[id].allmax = max(tree[id].allmax, tree[LCHD(id)].rmax + tree[RCHD(id)].lmax);
	}
}

void update(int id, int pos, int val)
{
	int mid = (tree[id].left + tree[id].right)>>1;
	if (tree[id].left==tree[id].right)
	{
		tree[id].lmax = tree[id].rmax = tree[id].allmax = 1; //啥也不用干
		return;
	}
	if (pos<=mid)
	{
		update(LCHD(id),pos,val);
	}else if(pos>mid){
		update(RCHD(id),pos,val);
	}
	tree[id].lmax = tree[LCHD(id)].lmax;
	tree[id].rmax = tree[RCHD(id)].rmax;
	tree[id].allmax = max(tree[LCHD(id)].allmax, tree[RCHD(id)].allmax);
	if(num[mid] < num[mid+1])
	{
		if(tree[id].lmax == LEN(LCHD(id)))
			tree[id].lmax += tree[RCHD(id)].lmax;
		if(tree[id].rmax == LEN(RCHD(id)))
			tree[id].rmax += tree[LCHD(id)].rmax;
		tree[id].allmax = max(tree[id].allmax, tree[LCHD(id)].rmax + tree[RCHD(id)].lmax);
	}
}

int query(int id, int l, int r)
{
	int mid = (tree[id].left + tree[id].right)>>1;
	int maxv;
	if((tree[id].left==l)&&(tree[id].right==r))
	{
		return tree[id].allmax;
	}
	if(r <= mid)
		return query(LCHD(id), l, r);
	if(l > mid)
		return query(RCHD(id), l, r);
	maxv = max(query(LCHD(id), l, mid), query(RCHD(id), mid+1, r));
	if(num[mid] < num[mid+1])
	{
		maxv = max(maxv, min(mid - l + 1, tree[LCHD(id)].rmax) + min(r - mid, tree[RCHD(id)].lmax));
	}
	return maxv;
}

void domain()
{
	int i, a, b;
	char op[5];
	
	scanf("%d%d",&n,&m);
	for(i=0;i<n;i++)
	{
		scanf("%d",&num[i]);
	}
	build_tree(1, 0, n-1);
	for(i=0;i<m;i++)
	{
		scanf("%s%d%d",op,&a,&b);
		if(op[0] == 'U')
		{
			num[a] = b;
			update(1, a, b);
		}else{
			printf("%d\n",query(1, a, b));
		}
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