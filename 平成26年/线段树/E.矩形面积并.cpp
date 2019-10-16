//POJ1151 用扫描线，见ppt
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

#define MAX 100005
using namespace std;

struct Tree{
	int left ,right;	
	double len;  //长度
	int cover;   //覆盖数
}tree[300005];
struct Line{
	double x, y1, y2;
	bool Left;
}lines[300];
int nl;
double y[300];
int ny, n;

int compi(const void* A, const void* B)
{
	double *a, *b;
	a = (double*)A;
	b = (double*)B;
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
		return 0;
	else
		return -1;
}

void build_tree(int id,int l,int r)
{
	int mid;

	tree[id].left = l;
	tree[id].right = r;
	tree[id].cover = 0;
	tree[id].len = 0;
	if (l==r){
		return;
	}
	mid = (l + r) >> 1;
	build_tree( (id<<1),l,mid);
	build_tree( (id<<1)+1,mid+1,r);
}

void Insert(int id,int l,int r)
{
	int mid;

	if (tree[id].left==l&&tree[id].right==r){
		tree[id].len = y[r+1] - y[l];
		tree[id].cover++;
		return;
	}
	mid = (tree[id].left + tree[id].right) >> 1;
	if (r<=mid){
		Insert(id<<1,l,r);
	}
	else if(l>mid){
		Insert( (id<<1)+1,l,r);
	}
	else{
		Insert( (id<<1),l,mid);
		Insert( (id<<1)+1,mid+1,r);
	}
	if(tree[id].cover > 0)
	{
		tree[id].len = y[tree[id].right+1] - y[tree[id].left];
	}else{
		tree[id].len = tree[(id<<1)].len + tree[(id<<1)+1].len;
	}
}

void Delete(int id,int l,int r)
{
	int mid;

	if (tree[id].left==l&&tree[id].right==r){
		tree[id].cover--;
		if(tree[id].cover == 0)
		{
			if(l == r)
			{
				tree[id].len = 0;
			}else{
				tree[id].len = tree[(id<<1)].len + tree[(id<<1)+1].len;
			}
		}
		return;
	}
	mid = (tree[id].left + tree[id].right) >> 1;
	if (r<=mid){
		Delete(id<<1,l,r);
	}
	else if(l>mid){
		Delete( (id<<1)+1,l,r);
	}
	else{
		Delete( (id<<1),l,mid);
		Delete( (id<<1)+1,mid+1,r);
	}
	if(tree[id].cover > 0)
	{
		tree[id].len = y[tree[id].right+1] - y[tree[id].left];
	}else{
		tree[id].len = tree[(id<<1)].len + tree[(id<<1)+1].len;
	}
}

bool input()
{
	int i, j;
	double x1, y1, x2, y2;

	if(scanf("%d",&n)==EOF)
		return false;
	if(n == 0)
		return false;

	nl = 0;
	ny = 0;
	for(i=0;i<n;i++)
	{
		scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
		y[ny] = y1;
		ny++;
		y[ny] = y2;
		ny++;
		lines[nl].x = x1;
		lines[nl].y1 = y1;
		lines[nl].y2 = y2;
		lines[nl].Left = true;
		nl++;
		lines[nl].x = x2;
		lines[nl].y1 = y1;
		lines[nl].y2 = y2;
		lines[nl].Left = false;
		nl++;
	}
	qsort(y, ny, sizeof(double), compi);
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
	build_tree(1, 0, ny - 1 - 1);  //区间数是线数-1

	return true;
}

int BiSearch(double x)
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

double calc()
{
	int i, l, r;
	double ret = 0;

	for(i=0;i<nl-1;i++)
	{
		l = BiSearch(lines[i].y1);
		r = BiSearch(lines[i].y2);
		if(lines[i].Left)
			Insert(1, l, r - 1);
		else
			Delete(1, l, r - 1);
		ret += tree[1].len * (lines[i+1].x - lines[i].x);
	}

	return ret;
}

int main()
{
	int i = 0;
	while(input())
	{
		i++;
		printf("Test case #%d\n",i);
		printf("Total explored area: %.2lf\n\n",calc());
	}
	return 0;
}