#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN   65536
#define MAXT 2000005
#define LEN(tx) (tx.right - tx.left + 1)

struct Tree{
	int left ,right;
	int lchd, rchd;
	int num;       //区间1的个数
	char mark;     //标记(N常态，R清零，S置位，I取反)
}tree[MAXT];  //区间扩大2倍
int cnt;
int root;
bool bin[MAXN<<1];  //是否含有

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
		tree[id].num = 0;
		return id;
	}
	tree[id].lchd = build_tree(l, mid);
	tree[id].rchd = build_tree(mid+1, r);
	tree[id].num = 0;
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

void query4(int id)  //将情况反映到bin里
{
	int mid = (tree[id].left + tree[id].right)>>1;
	int i;
	if(tree[id].mark=='S')
	{
		for(i=tree[id].left;i<=tree[id].right;i++)
			bin[i] = true;
		return ;
	}else if(tree[id].mark=='R')
	{
		return ;
	}
	if(tree[id].left==tree[id].right)
	{
		if(tree[id].num > 0)
			bin[tree[id].left] = true;
		return ;
	}
	operate(tree[id].lchd, tree[id].mark);
	operate(tree[id].rchd, tree[id].mark);
	tree[id].mark = 'N';
	query4(tree[id].lchd);
	query4(tree[id].rchd);
}

int main()
{
	char op, lo, ro;
	int l, r, i, st, ed, cnt;

	root = build_tree(0, MAXN << 1);
	while(scanf("%c %c%d,%d%c%*c",&op,&lo,&l,&r,&ro)!=EOF)
	{
		l <<= 1;
		r <<= 1;
		if(lo == '(')
			l++;
		if(ro == ')')
			r--;
		if(l > r)
		{
			if((op=='I')||(op=='C'))
			{
				update(root, 0, MAXN<<1, 'R');
			}
		}else{
			switch(op)
			{
			case 'U':
				update(root, l, r, 'S');
				break;
			case 'D':
				update(root, l, r, 'R');
				break;
			case 'S':
				update(root, l, r, 'I');
				break;
			case 'I':
				if(l > 0)
					update(root, 0, l-1, 'R');
				if(r < MAXN<<1)
					update(root, r+1, MAXN<<1, 'R');
				break;
			case 'C':
				
				if(l > 0)
					update(root, 0, l-1, 'R');
				if(r < MAXN<<1)
					update(root, r+1, MAXN<<1, 'R');
				update(root, l, r, 'I');
				break;
			}
		}
	}

	cnt = 0;
	memset(bin, 0, sizeof(bin));
	if(query3(root, 0, MAXN<<1)==0)
	{
		printf("empty set\n");
		return 0;
	}else{
		query4(root);
		st = -1;
		for(i=0;i<MAXN<<1;i++)
		{
			if(bin[i])
			{
				if(st == -1)
					st = i;
				ed = i;
			}else{
				if(st == -1)
					continue;
				l = st >> 1;
				r = (ed + 1) >> 1;
				lo = (st&1)?'(':'[';
				ro = (ed&1)?')':']';
				if(cnt > 0)
					printf(" ");
				printf("%c%d,%d%c",lo,l,r,ro);
				cnt++;
				st = -1;
			}
		}
	}
	if(cnt == 0)
	{
		printf("empty set\n");
	}else{
		printf("\n");
	}

	return 0;
}
