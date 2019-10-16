/*
题目大意： 
0 a b将区间[a,b]所有数全部变成0 
1 a b将区间[a,b]所有数全部变成1 
2 a b将区间[a,b]中所有数0 1互换，0变1,1变0 
3 a b输出区间[a,b]中1的个数 
4 a b输出区间[a,b]中最长连续1的个数 

嘛，我是没有做任何合并操作，也没有双lazy，而是记录一堆东西，跑得反而比网上的更快……
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN  100005
#define MAXT 2000005
#define LEN(tx) (tx.right - tx.left + 1)

struct Tree{
	int left ,right;
	int lchd, rchd;
	int num;       //区间1的个数
	char mark;     //标记(N常态，R清零，S置位，I取反)
	int nl,nr,nm;  //左右中最长连续0序列
	int el,er,em;  //左右中最长连续1序列
}tree[MAXT];
int cnt;
int n, m, root;
bool num[MAXN];

void popup(int id)
{
	tree[id].num = tree[tree[id].lchd].num + tree[tree[id].rchd].num;

	tree[id].nl = tree[tree[id].lchd].nl;
	if(tree[id].nl == LEN(tree[tree[id].lchd]))
		tree[id].nl += tree[tree[id].rchd].nl;
	tree[id].nr = tree[tree[id].rchd].nr;
	if(tree[id].nr == LEN(tree[tree[id].rchd]))
		tree[id].nr += tree[tree[id].lchd].nr;
	tree[id].nm = max(tree[tree[id].lchd].nr + tree[tree[id].rchd].nl, max(tree[tree[id].lchd].nm, tree[tree[id].rchd].nm));

	tree[id].el = tree[tree[id].lchd].el;
	if(tree[id].el == LEN(tree[tree[id].lchd]))
		tree[id].el += tree[tree[id].rchd].el;
	tree[id].er = tree[tree[id].rchd].er;
	if(tree[id].er == LEN(tree[tree[id].rchd]))
		tree[id].er += tree[tree[id].lchd].er;
	tree[id].em = max(tree[tree[id].lchd].er + tree[tree[id].rchd].el, max(tree[tree[id].lchd].em, tree[tree[id].rchd].em));
}

int build_tree(int l,int r)
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
		if(num[l])
		{
			tree[id].num = 1;
			tree[id].nl = tree[id].nr = tree[id].nm = 0;
			tree[id].el = tree[id].er = tree[id].em = 1;
		}else{
			tree[id].num = 0;
			tree[id].nl = tree[id].nr = tree[id].nm = 1;
			tree[id].el = tree[id].er = tree[id].em = 0;
		}
		return id;
	}
	tree[id].lchd = build_tree(l, mid);
	tree[id].rchd = build_tree(mid+1, r);
	popup(id);
	return id;
}

void operate(int id, char op)
{
	if(op == 'N')
		return ;
	if(op == 'R')
	{
		tree[id].num = 0;
		tree[id].nl = tree[id].nr = tree[id].nm = LEN(tree[id]);
		tree[id].el = tree[id].er = tree[id].em = 0;
		tree[id].mark = 'R';
		return ;
	}
	if(op == 'S')
	{
		tree[id].num = LEN(tree[id]);
		tree[id].nl = tree[id].nr = tree[id].nm = 0;
		tree[id].el = tree[id].er = tree[id].em = LEN(tree[id]);
		tree[id].mark = 'S';
		return ;
	}
	if(op == 'I')
	{
		tree[id].num = LEN(tree[id]) - tree[id].num;
		swap(tree[id].nl, tree[id].el);
		swap(tree[id].nr, tree[id].er);
		swap(tree[id].nm, tree[id].em);
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
	popup(id);
}

int query3(int id,int l,int r)   //查询个数
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
		return query3(tree[id].lchd, l, r);
	}
	else if(l>mid){
		return query3(tree[id].rchd, l, r);
	}
	else{
		return query3(tree[id].lchd, l, mid) + query3(tree[id].rchd, mid+1, r);
	}
}

int query4(int id,int l,int r)   //查询连1
{
	int mid = (tree[id].left + tree[id].right)>>1;
	int mmax;
	if (tree[id].left==l&&tree[id].right==r)
	{
		return tree[id].em;
	}
	operate(tree[id].lchd, tree[id].mark);
	operate(tree[id].rchd, tree[id].mark);
	tree[id].mark = 'N';
	if (r<=mid){
		return query4(tree[id].lchd, l, r);
	}
	else if(l>mid){
		return query4(tree[id].rchd, l, r);
	}
	else{
		mmax = max(query4(tree[id].lchd, l, mid), query4(tree[id].rchd, mid+1, r));
		mmax = max(mmax, min(mid - l + 1, tree[tree[id].lchd].er) + min(r - mid, tree[tree[id].rchd].el));
		return mmax;
	}
}

void domain()
{
	int i, tmp, op, a, b;

	scanf("%d%d",&n,&m);
	cnt = 0;
	for(i=0;i<n;i++)
	{
		scanf("%d",&tmp);
		if(tmp == 1)
		{
			num[i] = true;
		}else{
			num[i] = false;
		}
	}
	root = build_tree(0, n-1);
	for(i=0;i<m;i++)
	{
		scanf("%d%d%d",&op,&a,&b);
		switch(op)
		{
		case 0:
			update(root, a, b, 'R');
			break;
		case 1:
			update(root, a, b, 'S');
			break;
		case 2:
			update(root, a, b, 'I');
			break;
		case 3:
			printf("%d\n",query3(root, a, b));
			break;
		case 4:
			printf("%d\n",query4(root, a, b));
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
