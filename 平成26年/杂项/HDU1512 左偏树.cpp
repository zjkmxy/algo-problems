#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<string>
#include<cstdlib>
#include<queue>
#include<stack>
#include<utility>
#include<map>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;

const int MAXN = 100005;
#define NIL -1
struct NODE
{
	int lchd, rchd, par;
	int val, dis;
}node[MAXN];
int n, m;

int merge(int a, int b)
{
	if(a == NIL)
		return b;
	if(b == NIL)
		return a;
	if(node[b].val > node[a].val)
		swap(a, b);
	node[a].rchd = merge(node[a].rchd, b);
	node[node[a].rchd].par = a;
	if(node[a].lchd == NIL || node[node[a].rchd].dis > node[node[a].lchd].dis)
		swap(node[a].rchd, node[a].lchd);
	node[a].dis = (node[a].rchd==NIL) ? 0 : (node[node[a].rchd].dis + 1);
	return a;
}

int pop(int a)
{
	int l, r;
	l = node[a].lchd;
	r = node[a].rchd;
	if(l != NIL)
		node[l].par = l;
	if(r != NIL)
		node[r].par = r;
	node[a].lchd = node[a].rchd = NIL;
	node[a].dis = 0;
	return merge(l, r);
}

int root(int a)
{
	if(node[a].par != a)
		node[a].par = root(node[a].par);
	return node[a].par;
}

int main()
{
	int i, a, b, p, q;
	while(scanf( "%d", &n ) != EOF)
	{
		
		for(i=1;i<=n;i++)
		{
			scanf("%d",&node[i].val);
			node[i].dis = 0;
			node[i].lchd = node[i].rchd = NIL;
			node[i].par = i;
		}
		scanf("%d",&m);
		for(i=0;i<m;i++)
		{
			scanf("%d%d",&a,&b);
			a = root(a);
			b = root(b);
			if(a == b)
				printf("-1\n");
			else{
				node[a].val >>= 1;
				p = pop(a);
				p = merge(p, a);
				node[b].val >>= 1;
				q = pop(b);
				q = merge(q, b);
				printf("%d\n",node[merge(p, q)].val);
			}
		}
	}
	return 0;
}
