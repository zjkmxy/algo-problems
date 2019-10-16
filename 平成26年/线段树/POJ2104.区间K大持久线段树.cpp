#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#define MAX   100005
#define MAX2 3000000
using namespace std;

struct Tree{
	int left ,right;
	int lchd, rchd;
	int num;
}tree[MAX2];
int root[MAX]; //存储根节点
int cnt;       //tree用了多少

int n, m;      //数字数，查询数
int arr[MAX];  //原数列
int num[MAX];  //离散数列
int nn;        //离散个数（建树用）

int compi(const void* A, const void* B)
{
	int *a, *b;
	a = (int*)A;
	b = (int*)B;
	return (*a - *b);
}

int BiSearch(int x)
{
	int begin,end,mid;

	begin = 0;
	end = nn - 1;

	while(end > begin + 1)
	{
		mid = (begin + end) >> 1;
		if(num[mid] == x)
			return mid;
		if(num[mid] < x)
		{
			begin = mid;
		}else{
			end = mid;
		}
	}

	if(num[end] > x)
	{
		return begin;
	}else{
		return end;
	}
}

int build_tree(int l,int r){
	int id = cnt, mid = (l + r) >> 1;

	cnt++;
	tree[id].left = l;
	tree[id].right = r;
	tree[id].num = 0;
	if (l==r){
		tree[id].lchd = 0;
		tree[id].rchd = 0;
		return id;
	}
	tree[id].lchd = build_tree(l,mid);
	tree[id].rchd = build_tree(mid+1,r);
	return id;
}

int insert(int id,int l,int r, int val)
{
	int ret = cnt, mid = (tree[id].left + tree[id].right)>>1;
	cnt++;
	memcpy(&tree[ret], &tree[id], sizeof(Tree));
	if (tree[id].left==l&&tree[id].right==r){
		tree[ret].num += val;
		return ret;
	}
	tree[ret].num += val;
	if (r<=mid){
		tree[ret].lchd = insert(tree[id].lchd,l,r,val);
	}
	else if(l>mid){
		tree[ret].rchd = insert(tree[id].rchd,l,r,val);
	}
	else{
		tree[ret].lchd = insert(tree[id].lchd,l,mid,val);
		tree[ret].rchd = insert(tree[id].rchd,mid+1,r,val);
	}
	return ret;
}

int query(int id1, int id2, int k)
{
	int tcnt;

	while(tree[id1].left != tree[id1].right)
	{
		tcnt = tree[tree[id2].lchd].num - tree[tree[id1].lchd].num;
		if(tcnt >= k)
		{
			id1 = tree[id1].lchd;
			id2 = tree[id2].lchd;
		}else{
			id1 = tree[id1].rchd;
			id2 = tree[id2].rchd;
			k -= tcnt;
		}
	}
	return tree[id1].left;
}

void input()
{
	int i, j;

	scanf("%d%d",&n,&m);
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
		num[i] = arr[i];
	}
	qsort(num, n, sizeof(int), compi);
	i = 0;
	j = 0;
	while(j < n)
	{
		num[i] = num[j];
		do{
			j++;
		}while((num[i]==num[j])&&(j < n));
		i++;
	}
	nn = i;
	cnt = 0;
	root[0] = build_tree(0, nn - 1);
	for(i=1;i<=n;i++)
	{
		j = BiSearch(arr[i-1]);
		root[i] = insert(root[i-1], j, j, 1);
	}
}

int main()
{
	int i, t, j;
	int x, y, k;


	input();
	for(j=0;j<m;j++)
	{
		scanf("%d%d%d",&x,&y,&k);
		printf("%d\n",num[query(root[x-1],root[y],k)]);
	}


	return 0;
}