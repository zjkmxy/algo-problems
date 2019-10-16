/*
矩形覆盖点权值和

本题的线段树是点树：
[1-4]{[1-2]{[1],[2]},[3-4]{[3],[4]}}
所以需要拆点以保证边界条件不会冲突（因为框是不含边的）

但是如果用段树的话：
(1-4){(1-2),(2-4){(2-3),(3-4)}}
可以不用拆点做
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define MAX 100005
#define LCHD(x) (((x)<<1))
#define RCHD(x) (((x)<<1)+1)

typedef signed long long int SLINT;

struct Tree{
	int left ,right;	
	SLINT maxv;
	SLINT lnc;
}tree[MAX<<3];
int N, H, W;

struct Line{
	SLINT x, y1, y2;
	SLINT c;
}lines[MAX<<1];
int nl;
SLINT y[MAX<<1];
int ny;

int compi(const void* A, const void* B)
{
	SLINT *a, *b;
	a = (SLINT*)A;
	b = (SLINT*)B;
	if(*a > *b)
		return 1;
	else if(*a == *b)
		return 0;
	else
		return -1;
}

int compll(const void* A, const void* B)
{
	Line *a, *b;
	a = (Line*)A;
	b = (Line*)B;
	if(a->x > b->x)
		return 1;
	else if(a->x == b->x)
	{
		return a->c - b->c;
	}
	else
		return -1;
}

void build_tree(int id,int l,int r)
{
	tree[id].left = l;
	tree[id].right = r;
	tree[id].lnc = 0;
	tree[id].maxv = 0;
	if (l==r){
		return;
	}
	int mid = (l + r)>>1;
	build_tree(LCHD(id), l, mid);
	build_tree(RCHD(id), mid+1, r);
}

void push_down(int id)
{
	tree[LCHD(id)].lnc += tree[id].lnc;
	tree[RCHD(id)].lnc += tree[id].lnc;
	tree[id].maxv += tree[id].lnc;
	tree[id].lnc = 0;
}

void pop_up(int id)
{
	tree[id].maxv = max(tree[LCHD(id)].maxv + tree[LCHD(id)].lnc, tree[RCHD(id)].maxv + tree[RCHD(id)].lnc);
}

void update(int id, int l, int r, SLINT val)
{
	if (tree[id].left==l&&tree[id].right==r)
	{
		tree[id].lnc += val;
		return;
	}
	int mid = (tree[id].left + tree[id].right)>>1;
	push_down(id);
	if (r<=mid){
		update(LCHD(id), l, r, val);
	}
	else if(l>mid){
		update(RCHD(id), l, r, val);
	}
	else{
		update(LCHD(id), l, mid, val);
		update(RCHD(id), mid+1, r, val);
	}
	pop_up(id);
}

SLINT query(int id,int l,int r){
	if (tree[id].left==l&&tree[id].right==r)
	{
		return (tree[id].maxv + tree[id].lnc);
	}else{
		push_down(id);
	}
	int mid = (tree[id].left+tree[id].right)>>1;
	if (r<=mid){
		return query(LCHD(id), l, r);
	}
	else if(l>mid){
		return query(RCHD(id), l, r);
	}
	else{
		return max(query(LCHD(id), l, mid), query(RCHD(id), mid+1, r));
	}
}

int BiSearch(SLINT x)
{
	int begin,end,mid;

	begin = 0;
	end = ny - 1;

	while(end > begin + 1)
	{
		mid = (begin + end) >> 1;
		if(y[mid] == x)
			return mid;
		if(y[mid] < x)
		{
			begin = mid;
		}else{
			end = mid;
		}
	}

	if(y[end] > x)
	{
		return begin;
	}else{
		return end;
	}
}

bool input()
{
	int i, j;
	SLINT x1, y1, c;

	if(scanf("%d%d%d",&N,&W,&H)==EOF)
		return false;

	nl = 0;
	ny = 0;
	for(i=0;i<N;i++)
	{
		scanf("%lld%lld%lld",&x1,&y1,&c);
		y[ny] = y1;
		ny++;
		y[ny] = y1 + H;
		ny++;
		lines[nl].x = x1;
		lines[nl].y1 = y1;
		lines[nl].y2 = y1 + H;
		lines[nl].c = c;
		nl++;
		lines[nl].x = x1 + W;
		lines[nl].y1 = y1;
		lines[nl].y2 = y1 + H;
		lines[nl].c = -c;
		nl++;
	}

	if(N == 0 || W == 0 || H == 0)
		return true;

	qsort(y, ny, sizeof(SLINT), compi);
	i = 0;
	j = 0;
	while(j < ny)
	{
		y[i] = y[j];
		do{
			j++;
		}while((y[i]==y[j])&&(j < ny));
		i++;
	}
	ny = i;
	qsort(lines, nl, sizeof(Line), compll);
	build_tree(1, 0, ny * 2);  //二分点

	return true;
}

SLINT domain()
{
	int i, l, r;
	SLINT ret = 0;

	if(N == 0 || W == 0 || H == 0)
		return 0;

	for(i=0;i<nl;i++)
	{
		l = BiSearch(lines[i].y1);
		l = l * 2;
		r = BiSearch(lines[i].y2);
		r = r * 2 - 1;
		update(1, l, r, lines[i].c);
		ret = max(ret, query(1, 0, ny*2));
	}

	return ret;
}

int main()
{
	while(input())
	{
		printf("%lld\n",domain());
	}
	return 0;
}
