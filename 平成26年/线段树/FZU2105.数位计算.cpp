//fzu 2105 Digits Count 
/*
长度为n（1~10^6）的数组A[]（0<=A[i]<16），有4种操作。
  1：and opn l r  l<=i<=r A[i]&opn
  2：or  opn l r  l<=i<=r A[i]|opn
  3：xor opn l r  l<=i<=r A[i]^opn
  4：sum l r      l<=i<=r sum{A[l~r]}

操作总数为m（1~10^5）。
很裸的线段树，维护区间中每个数各位上1的个数。
就是要细心，不然可能会写错。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 1000005
#define MAXT 4000005
#define LEN(tx) (tx.right - tx.left + 1)

struct Tree{
	int left ,right;
	int lchd, rchd;
	int num;       //各个数位上1的个数
	char mark;     //标记(N常态，R清零，S置位，I取反)
}tree[MAXT];
int cnt;
int n, m, root[4];
bool num[MAXN][4];

int build_tree(int l,int r, int k)  //k是数位数
{
	int mid = (l + r) >> 1;
	int id = cnt;
	cnt++;
	tree[id].left = l;
	tree[id].right = r;
	tree[id].mark = 'N';
	if (l==r){
		tree[id].lchd = 0;
		tree[id].rchd = 0;
		tree[id].num = 0;
		if(num[l][k])
			tree[id].num++;
		return id;
	}
	tree[id].lchd = build_tree(l, mid, k);
	tree[id].rchd = build_tree(mid+1, r, k);
	tree[id].num = tree[tree[id].lchd].num + tree[tree[id].rchd].num;
	return id;
}

void operate(int id, char op)
{
	if(op == 'N')
		return ;
	if(op == 'R')
	{
		tree[id].num = 0;
		tree[id].mark = 'R';
		return ;
	}
	if(op == 'S')
	{
		tree[id].num = LEN(tree[id]);
		tree[id].mark = 'S';
		return ;
	}
	if(op == 'I')
	{
		tree[id].num = LEN(tree[id]) - tree[id].num;
		switch(tree[id].mark)
		{
		case 'N':
			tree[id].mark = 'I';
			break;
		case 'I':
			tree[id].mark = 'N';
			break;
		case 'R':
			tree[id].mark = 'S';
			break;
		case 'S':
			tree[id].mark = 'R';
			break;
		}
		return ;
	}
}

void update(int id,int l,int r, char op)
{
	int mid = (tree[id].left + tree[id].right)>>1;
	if (tree[id].left==l&&tree[id].right==r)
	{
		operate(id, op);
		return;
	}
	operate(tree[id].lchd, tree[id].mark);
	operate(tree[id].rchd, tree[id].mark);
	tree[id].mark = 'N';
	if (r<=mid){
		update(tree[id].lchd,l,r,op);
	}
	else if(l>mid){
		update(tree[id].rchd,l,r,op);
	}
	else{
		update(tree[id].lchd,l,mid,op);
		update(tree[id].rchd,mid+1,r,op);
	}
	tree[id].num = tree[tree[id].lchd].num + tree[tree[id].rchd].num;
}

int query(int id,int l,int r)
{
	int mid = (tree[id].left + tree[id].right)>>1;
	if (tree[id].left==l&&tree[id].right==r)
	{
		return tree[id].num;
	}
	operate(tree[id].lchd, tree[id].mark);
	operate(tree[id].rchd, tree[id].mark);
	tree[id].mark = 'N';
	if (r<=mid){
		return query(tree[id].lchd, l, r);
	}
	else if(l>mid){
		return query(tree[id].rchd, l, r);
	}
	else{
		return query(tree[id].lchd, l, mid) + query(tree[id].rchd, mid+1, r);
	}
}

void domain()
{
	int l, r, cur, i, ret, j;
	bool opn[4];
	int tmp[4];
	char op[4];

	scanf("%d%d",&n,&m);
	for(i=0;i<n;i++)
	{
		scanf("%d",&cur);
		num[i][0] = ((cur & 1) > 0);
		num[i][1] = ((cur & 2) > 0);
		num[i][2] = ((cur & 4) > 0);
		num[i][3] = ((cur & 8) > 0);
	}
	cnt = 0;
	for(j=0;j<4;j++)
		root[j] = build_tree(0, n-1, j);
	for(i=0;i<m;i++)
	{
		scanf("%s",op);
		switch(op[0])
		{
		case 'S':
			scanf("%d%d",&l,&r);
			for(j=0;j<4;j++)
				tmp[j] = query(root[j], l, r);
			ret = tmp[0] + (tmp[1] << 1) + (tmp[2] << 2) + (tmp[3] << 3);
			printf("%d\n",ret);
			break;
		case 'A':
			scanf("%d%d%d",&cur,&l,&r);
			opn[0] = ((cur & 1) > 0);
			opn[1] = ((cur & 2) > 0);
			opn[2] = ((cur & 4) > 0);
			opn[3] = ((cur & 8) > 0);
			for(j=0;j<4;j++)
			{
				if(!opn[j])
					update(root[j], l, r, 'R'); //a&0=0
			}
			break;
		case 'O':
			scanf("%d%d%d",&cur,&l,&r);
			opn[0] = ((cur & 1) > 0);
			opn[1] = ((cur & 2) > 0);
			opn[2] = ((cur & 4) > 0);
			opn[3] = ((cur & 8) > 0);
			for(j=0;j<4;j++)
			{
				if(opn[j])
					update(root[j], l, r, 'S');  //a|1=1
			}
			break;
		case 'X':
			scanf("%d%d%d",&cur,&l,&r);
			opn[0] = ((cur & 1) > 0);
			opn[1] = ((cur & 2) > 0);
			opn[2] = ((cur & 4) > 0);
			opn[3] = ((cur & 8) > 0);
			for(j=0;j<4;j++)
			{
				if(opn[j])
					update(root[j], l, r, 'I');  //a^1=~a
			}
			break;
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