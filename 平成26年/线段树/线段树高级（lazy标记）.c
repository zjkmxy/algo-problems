#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#define MAX 100005
using namespace std;
struct Tree{
	int left ,right;	
	__int64 sum;
	__int64 lnc;
}tree[300005];
__int64 num[100005];
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
};
void update(int id,int l,int r, __int64 val){
	if (tree[id].left==l&&tree[id].right==r){
		tree[id].lnc += val;
		return;
	}
	tree[id].sum += ((r - l + 1) * val);
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
};
__int64 query(int id,int l,int r){
	if (tree[id].left==l&&tree[id].right==r){
		return tree[id].sum + (r - l + 1) * tree[id].lnc;//Ñ¯ÎÊ×ÜºÍ
	}
	else{
		tree[id<<1].lnc += tree[id].lnc;
		tree[(id<<1)+1].lnc += tree[id].lnc;
		tree[id].sum += (tree[id].lnc * (tree[id].right - tree[id].left + 1));
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
};