#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>

#define MAX  2500000
using namespace std;
typedef __int64 LINT;

#define ROOT                  1
#define LCHD(id)       (id << 1)
#define RCHD(id) ((id << 1) + 1)

int tree[MAX];
int n, m;       //数字数，查询数

void build_tree(int id, int l,int r){
	int mid = (l + r) >> 1;

	if (l==r){
		tree[id] = 1;
		return;
	}
	build_tree(LCHD(id),l,mid);
	build_tree(RCHD(id),mid+1,r);
	tree[id] = tree[LCHD(id)]+ tree[RCHD(id)];
}

int take(int k)
{
	int tcnt;
	int l = 1, r = n, mid;
	int id = ROOT;

	while(l != r)
	{
		tcnt = tree[LCHD(id)];
		tree[id]--;
		mid = (l + r) >> 1;
		if(tcnt >= k)
		{
			id = LCHD(id);
			r = mid;
		}else{
			id = RCHD(id);
			l = mid + 1;
			k -= tcnt;
		}
	}
	tree[id]--;
	return l;
}

void domain()
{
	int i, k;
	LINT ret = 0;

	scanf("%d%d",&n,&m);
	build_tree(ROOT, 1, n);
	for(i=0;i<m;i++)
	{
		scanf("%d",&k);
		ret += (LINT)take(k);
	}
	printf("%I64d\n", ret);
}

int main()
{
	int i, t;

	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		printf("Case %d: ",i);
		domain();
	}

	return 0;
}