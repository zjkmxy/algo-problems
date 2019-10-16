//POJ3468
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#define MAX 100005
using namespace std;

typedef signed long long int SLINT;

struct Tree{
	int left ,right;	
	SLINT sum;
	SLINT lnc;
}tree[300005];
int num[100005];
int n, q;

void build_tree(int id,int l,int r){
	tree[id].left = l;
	tree[id].right = r;
	tree[id].lnc = 0;
	if (l==r){
		tree[id].sum = num[l];
		return;
	}
	int mid = (l + r)>>1;
	build_tree( (id<<1),l,mid);
	build_tree( (id<<1)+1,mid+1,r);
	tree[id].sum = tree[id<<1].sum + tree[(id<<1)+1].sum;
}
void update(int id,int l,int r, SLINT val){
	if (tree[id].left==l&&tree[id].right==r){
		tree[id].lnc += val;
		return;
	}
	tree[id].sum += ((SLINT)(r - l + 1) * val);
	int mid = (tree[id].left + tree[id].right)>>1;
	if (r<=mid){
		update(id<<1,l,r,val);
	}
	else if(l>mid){
		update( (id<<1)+1,l,r,val);
	}
	else{
		update( (id<<1),l,mid,val);
		update( (id<<1)+1,mid+1,r,val);
	}
}
SLINT query(int id,int l,int r){
	if (tree[id].left==l&&tree[id].right==r){
		return tree[id].sum + (SLINT)(r - l + 1) * tree[id].lnc;//Ñ¯ÎÊ×ÜºÍ
	}
	else{
		tree[id<<1].lnc += tree[id].lnc;
		tree[(id<<1)+1].lnc += tree[id].lnc;
		tree[id].sum += (tree[id].lnc * (SLINT)(tree[id].right - tree[id].left + 1));
		tree[id].lnc = 0;
	}
	int mid = (tree[id].left+tree[id].right)>>1;
	if (r<=mid){
		return query(id<<1,l,r);
	}
	else if(l>mid){
		return query( (id<<1)+1,l,r);
	}
	else{
		return query(id<<1,l,mid) + query( (id<<1)+1,mid+1,r);
	}
}

bool input()
{
	int i;

	if(scanf("%d%d",&n,&q)==EOF)
	{
		return false;
	}
	for(i=1;i<=n;i++)
	{
		scanf("%d",&num[i]);
	}
	build_tree(1, 1, n);

	return true;
}

void calc()
{
	char op[8];
	int a, b, c;

	scanf("%s",op);
	if(op[0] == 'Q')
	{
		scanf("%d%d",&a,&b);
		printf("%lld\n",query(1, a, b));
	}else if(op[0] == 'C')
	{
		scanf("%d%d%d",&a,&b,&c);
		update(1, a, b, (SLINT)c);
	}
}

int main()
{
	int i;

	while(input())
	{
		for(i=0;i<q;i++)
		{
			calc();
		}
	}

	return 0;
}