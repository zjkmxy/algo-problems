#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 200005
#define MAXM (MAXN<<1)
#define MAXD 30
#define lowbit(x) ((x)&(-(x)))

namespace pool
{
	int pool[MAXN * MAXD];
	int pos = 0;

	int* alloc(int size)
	{
		int *ret = pool+pos;
		pos += size;
		return ret;
	}
};
//多重树状数组，预先分配好内存
struct TARR
{
	int sz;
	int *arr;
	TARR(int size=0):sz(size)
	{
		arr = pool::alloc(size+1);
		memset(arr, 0, sizeof(int) * (size+1));
	};
	void update(int x, int val)
	{
		int i;
		x++;
		for(i=x;i<=sz;i+=lowbit(i))
		{
			arr[i] += val;
		}
	}
	int getsum(int x)
	{
		int sum = arr[0], i;
		x++;
		if(x < 0)
			return 0;
		if(x > sz)
			x = sz;
		for(i=x;i>0;i-=lowbit(i))
		{
			sum += arr[i];
		}
		return sum;
	}
};
//重心分治树
class TREE
{
private:
	struct NODE
	{
		int head;
		int par;		//父节点
		int subsz;		//原始子树大小
		int mson;		//自身做根的最大子树
		int w;			//权值
		bool vis;		//是否剖分过

		TARR tarr;		//自身距离的树状数组
		TARR parr;		//父亲距离的树状数组
		int path[MAXD];	//剖分路径
		int dis[MAXD];	//到达距离
	}node[MAXN];
	struct EDGE
	{
		int a, b;
		int next;
	}edge[MAXM];
	int queue[MAXN], head, tail, n, ecnt;

public:
	TREE(){};
	void init(int n)
	{
		int i;
		this->n = n;
		ecnt = 0;
		pool::pos = 0;
		for(i=0;i<=n;i++)
		{
			node[i].head = -1;
			node[i].par = -1;
			node[i].vis = false;
		}
	};
	//重心划分
	void divide(int cur, int ct)
	{
		int p, i, j, ed, sz;
		tail = 1;
		queue[0] = cur;
		node[cur].par = -1;
		node[cur].subsz = 1;   //和队列中的i>0一致
		node[cur].mson = 0;
		//划分重心
		for(head=0;head<tail;head++)
		{
			p = queue[head];
			for(j=node[p].head;j>=0;j=edge[j].next)
			{
				ed = edge[j].b;
				if(ed != node[p].par && !node[ed].vis)
				{
					node[ed].subsz = node[ed].mson = 0;
					node[ed].par = p;
					queue[tail] = ed;
					tail++;
				}
			}
		}
		for(i=tail-1;i>0;i--)
		{
			p = queue[i];
			ed = node[p].par;
			node[p].subsz++;
			node[ed].subsz += node[p].subsz;
			node[ed].mson = max(node[ed].mson, node[p].subsz);
		}
		sz = node[cur].subsz;
		for(i=0;i<tail;i++)
		{
			p = queue[i];
			node[p].mson = max(node[p].mson, sz - node[p].subsz);
			if(node[p].mson < node[cur].mson)
				cur = p;
		}
		//建立结构
		node[cur].tarr = TARR(tail);
		if(ct > 0)
			node[cur].parr = TARR(tail << 1);
		node[cur].vis = true;
		node[cur].path[ct] = cur;
		node[cur].dis[ct] = 0;
		node[cur].par = -1;
		//更新重心数据
		tail = 1;
		queue[0] = cur;
		for(head=0;head<tail;head++)
		{
			p = queue[head];
			for(j=node[p].head;j>=0;j=edge[j].next)
			{
				ed = edge[j].b;
				if(ed != node[p].par && !node[ed].vis)
				{
					node[ed].dis[ct] = node[p].dis[ct] + 1;
					node[ed].path[ct] = cur;
					node[ed].par = p;
					queue[tail] = ed;
					tail++;
				}
			}
			node[cur].tarr.update(node[p].dis[ct], node[p].w);
			if(ct > 0)
			{
				node[cur].parr.update(node[p].dis[ct-1], node[p].w);
			}
		}
		//递归划分子树
		for(j=node[cur].head;j>=0;j=edge[j].next)
		{
			ed = edge[j].b;
			if(!node[ed].vis)
				this->divide(ed, ct+1);
		}
	};

	void change(int cur, int w)
	{
		int plus = w - node[cur].w, i;

		node[cur].w = w;
		for(i=0;i==0||node[cur].path[i-1]!=cur;i++)
		{
			node[node[cur].path[i]].tarr.update(node[cur].dis[i], plus);
			if(i > 0)
			{
				node[node[cur].path[i]].parr.update(node[cur].dis[i-1], plus);
			}
		}
	};

	int query(int cur, int dis)
	{
		int i, sum = 0;

		for(i=0;i==0||node[cur].path[i-1]!=cur;i++)
		{
			sum += node[node[cur].path[i]].tarr.getsum(dis - node[cur].dis[i]);
			if(i > 0)
			{
				sum -= node[node[cur].path[i]].parr.getsum(dis - node[cur].dis[i-1]);
			}
		}
		return sum;
	};

	void addedge(int a, int b)
	{
		edge[ecnt].a = a;
		edge[ecnt].b = b;
		edge[ecnt].next = node[a].head;
		node[a].head = ecnt;
		ecnt++;
		edge[ecnt].a = b;
		edge[ecnt].b = a;
		edge[ecnt].next = node[b].head;
		node[b].head = ecnt;
		ecnt++;
	};
	
	inline void setw(int cur, int w)
	{
		node[cur].w = w;
	};
};
TREE tree;

bool domain()
{
	int i, a, b, n, q;
	char op[2];
	if(scanf("%d%d",&n,&q)==EOF)
		return false;
	tree.init(n);
	for(i=1;i<=n;i++)
	{
		scanf("%d",&a);
		tree.setw(i, a);
	}
	for(i=1;i<n;i++)
	{
		scanf("%d%d",&a,&b);
		tree.addedge(a, b);
	}
	tree.divide(1, 0);
	while(q--)
	{
		scanf("%s%d%d",op,&a,&b);
		if(op[0]=='!')
		{
			tree.change(a, b);
		}else{
			printf("%d\n",tree.query(a, b));
		}
	}
	return true;
}

int main()
{
	while(domain());
	return 0;
}
