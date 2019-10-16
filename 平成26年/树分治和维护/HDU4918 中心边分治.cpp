#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 400005
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
		int edge[3];
		int ecnt;
		int par;		//父节点
		int subsz;		//原始子树大小
		int mson;		//自身做根的最大子树
		int w;			//权值

		int de[MAXD];	//剖分边
		int path[MAXD];	//剖分子树根
		int dis[MAXD];	//到达距离
	}node[MAXN];
	int pcnt;
	struct EDGE
	{
		int a, b;
		int w;
		int rv;
		bool vis;		//是否剖分过
		TARR tarr;		//A点距离的树状数组
	}edge[MAXM];
	int queue[MAXN], head, tail, n, ecnt;

	//不合并连边
	void do_addedge(int a, int b, int w)
	{
		edge[ecnt].a = a;
		edge[ecnt].b = b;
		edge[ecnt].vis = false;
		edge[ecnt].w = w;
		edge[ecnt].rv = ecnt+1;
		node[a].edge[node[a].ecnt] = ecnt;
		ecnt++;
		node[a].ecnt++;
		edge[ecnt].a = b;
		edge[ecnt].b = a;
		edge[ecnt].vis = false;
		edge[ecnt].w = w;
		edge[ecnt].rv = ecnt-1;
		node[b].edge[node[b].ecnt] = ecnt;
		ecnt++;
		node[b].ecnt++;
	};
	//分离顶点
	void release(int p)
	{
		pcnt++;
		edge[node[p].edge[1]].a = pcnt;
		edge[edge[node[p].edge[1]].rv].b = pcnt;
		edge[node[p].edge[2]].a = pcnt;
		edge[edge[node[p].edge[2]].rv].b = pcnt;
		node[pcnt].ecnt = 2;
		node[pcnt].edge[0] = node[p].edge[1];
		node[pcnt].edge[1] = node[p].edge[2];
		node[p].ecnt = 1;
		do_addedge(p, pcnt, 0);
		node[pcnt].w = 0;
	}

public:
	TREE(){};
	//初始化
	void init(int n)
	{
		int i;
		this->n = n;
		ecnt = 0;
		pcnt = n;
		pool::pos = 0;
		for(i=0;i<=(n<<1);i++)
		{
			node[i].ecnt = 0;
			node[i].par = -1;
			node[i].w = 0;
		}
	};
	//有偿加边
	void addedge(int a, int b, int w)
	{
		if(node[a].ecnt == 3)
		{
			release(a);
		}
		if(node[b].ecnt == 3)
		{
			release(b);
		}
		do_addedge(a, b, w);
	};
	//中心边划分
	void divide(int cur, int ct, int pth)
	{
		int p, i, j, ed, sz;
		int de, tmp;
		tail = 1;
		queue[0] = cur;
		tmp = cur;
		node[cur].par = -1;
		node[cur].subsz = 1;   //和队列中的i>0一致
		node[cur].mson = 0;
		//初始化DFS
		for(head=0;head<tail;head++)
		{
			p = queue[head];
			for(i=0;i<node[p].ecnt;i++)
			{
				j = node[p].edge[i];
				ed = edge[j].b;
				if(ed != node[p].par && !edge[j].vis)
				{
					node[ed].subsz = node[ed].mson = 0;
					node[ed].par = p;
					queue[tail] = ed;
					tail++;
				}
			}
		}
		//计算子树尺寸
		for(i=tail-1;i>0;i--)
		{
			p = queue[i];
			ed = node[p].par;
			node[p].subsz++;
			node[ed].subsz += node[p].subsz;
			node[ed].mson = max(node[ed].mson, node[p].subsz);
		}
		sz = node[cur].subsz;
		//寻找重心和中心边
		for(i=0;i<tail;i++)
		{
			p = queue[i];
			node[p].mson = max(node[p].mson, sz - node[p].subsz);
			if(node[p].mson < node[cur].mson)
				cur = p;
		}
		de = -1;
		for(i=0;i<node[cur].ecnt;i++)
		{
			if(edge[node[cur].edge[i]].vis)
				continue;
			if(de < 0 || node[edge[node[cur].edge[i]].b].mson < node[edge[de].b].mson)
				de = node[cur].edge[i];
		}
		//加入树状数组
		if(ct > 0)
		{
			cur = tmp;
			tail = 1;
			queue[0] = cur;
			edge[pth].tarr = TARR(sz);
			node[cur].path[ct-1] = cur;
			node[cur].dis[ct-1] = 0;
			node[cur].de[ct-1] = pth;
			for(head=0;head<tail;head++)
			{
				p = queue[head];
				for(i=0;i<node[p].ecnt;i++)
				{
					j = node[p].edge[i];
					ed = edge[j].b;
					if(ed != node[p].par && !edge[j].vis)
					{
						if(ct > 0)
						{
							node[ed].path[ct-1] = cur;
							node[ed].dis[ct-1] = node[p].dis[ct-1] + edge[j].w;
							node[ed].de[ct-1] = pth;
						}
						queue[tail] = ed;
						tail++;
					}
				}
				if(ct > 0)
					edge[pth].tarr.update(node[p].dis[ct-1], node[p].w);
			}
		}
		//分治递归子树
		if(de == -1)
		{
			node[cur].de[ct] = -1;
			return;
		}
		edge[de].vis = true;
		edge[edge[de].rv].vis = true;
		this->divide(edge[de].a, ct+1, de);
		this->divide(edge[de].b, ct+1, edge[de].rv);
	};
	int query(int cur, int dis)
	{
		int i, sum = 0, tar;

		for(i=0;i==0||node[cur].de[i]>=0;i++)
		{
			tar = edge[node[cur].de[i]].rv;
			sum += edge[tar].tarr.getsum(dis - node[cur].dis[i] - edge[node[cur].de[i]].w);
		}
		sum += node[cur].w;
		return sum;
	};
	inline void setw(int cur, int w)
	{
		node[cur].w = w;
	};
	void change(int cur, int w)
	{
		int plus = w - node[cur].w, i, tar;

		node[cur].w = w;
		for(i=0;i==0||node[cur].de[i]>=0;i++)
		{
			tar = node[cur].de[i];
			edge[tar].tarr.update(node[cur].dis[i], plus);
		}
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
		tree.addedge(a, b, 1);
	}
	tree.divide(1, 0, -1);
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
