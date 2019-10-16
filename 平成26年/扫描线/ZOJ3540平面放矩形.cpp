//如果在利用延伸矩形的尺寸的方法，我们可以把矩形改成axb的
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
#include<cctype>
using namespace std;

#define MAXN 200005
#define MID(l, r) (((l)+(r))>>1)
#define LCHD(id)  ((id)<<1)
#define RCHD(id)  (((id)<<1)+1)

typedef long long LLI;
int W, H, N, M;
int xs[MAXN<<1], xcnt;
int ys[MAXN<<1], ycnt;
struct OBJ
{
	int x1, y1, x2, y2;  //离散时做加减处理
}obj[MAXN];
LLI ans;
struct EVENT
{
	int pos, l, r;
	int flag;
	bool operator<(const EVENT& other)const
	{
		if(pos == other.pos)
		{
			return flag < other.flag;
		}
		return pos < other.pos;
	}
}eve[MAXN<<1];
int ecnt;

void assert(bool exp)
{
	if(!exp)
		exp = *(bool*)NULL;
}

class TREE
{
private:
	struct NODE
	{
		int l, r;
		int over;
		int lmax, rmax;
		int cnt;
	}node[MAXN<<2];
	int *set;

	inline void setpos(int id, int flag)
	{
		node[id].over += flag;
		if(node[id].over > 0)
		{
			node[id].lmax = node[id].rmax = node[id].cnt = 0;
		}else{
			node[id].lmax = node[id].rmax = set[node[id].r] - set[node[id].l];
			node[id].cnt = max(set[node[id].r] - set[node[id].l] - M + 1, 0);
		}
	}
public:
	void build(int id, int l, int r)
	{
		int mid = MID(l, r);
		node[id].l = l;
		node[id].r = r;
		setpos(id, 0);
		if(r > l + 1)
		{
			build(LCHD(id), l, mid);
			build(RCHD(id), mid, r);
		}
	}
	void pushdown(int id)
	{
		if(node[id].over)
		{
			node[LCHD(id)].over = true;
			node[RCHD(id)].over = true;
			node[id].over = false;
			node[LCHD(id)].lmax = node[LCHD(id)].rmax = node[LCHD(id)].cnt = 0;
			node[RCHD(id)].lmax = node[RCHD(id)].rmax = node[RCHD(id)].cnt = 0;
		}
	}
	inline int cross(int a, int b)
	{
		int ret, ret2;
		if(a + b < M)
			return 0;
		ret = min(min(min(a, b), a+b-M+1), M-1);
		ret2 = (a + b - M + 1) - max(a - M + 1, 0) - max(b - M + 1, 0);
		return ret;
	}
	void popup(int id)
	{
		node[id].lmax = node[LCHD(id)].lmax;
		node[id].rmax = node[RCHD(id)].rmax;
		node[id].cnt = node[LCHD(id)].cnt + node[RCHD(id)].cnt;	//并行
		node[id].cnt += cross(node[LCHD(id)].rmax, node[RCHD(id)].lmax);
		if(node[id].lmax == set[node[LCHD(id)].r] - set[node[LCHD(id)].l])
			node[id].lmax += node[RCHD(id)].lmax;
		if(node[id].rmax == set[node[RCHD(id)].r] - set[node[RCHD(id)].l])
			node[id].rmax += node[LCHD(id)].rmax;
	};
	void update(int id, int l, int r, int flag)
	{
		int mid = MID(node[id].l, node[id].r);
		if(node[id].l >= l && node[id].r <= r)
		{
			setpos(id, flag);
			return ;
		}
		pushdown(id);
		if(r <= mid){
			update(LCHD(id), l, r, flag);
		}else if(l >= mid){
			update(RCHD(id), l, r, flag);
		}else{
			update(LCHD(id), l, mid, flag);
			update(RCHD(id), mid, r, flag);
		}
		popup(id);
	}
	inline LLI query()
	{
		return node[1].cnt;
	}
	void init(int cnt, int* st)
	{
		set = st;
		build(1, 0, cnt-1);
	}
} tree;

bool input()
{
	int i;
	if(scanf("%d%d%d%d",&W,&H,&N,&M)==EOF)
		return false;
	xcnt = ycnt = 2;
	xs[0] = ys[0] = 0;
	xs[1] = W;
	ys[1] = H;
	for(i=0;i<N;i++)
	{
		scanf("%d%d%d%d",&obj[i].x1,&obj[i].y1,&obj[i].x2,&obj[i].y2);
		obj[i].x1--;
		obj[i].y1--;
		xs[xcnt++] = obj[i].x1;
		xs[xcnt++] = obj[i].x2;
		ys[ycnt++] = obj[i].y1;
		ys[ycnt++] = obj[i].y2;
	}
	sort(xs, xs+xcnt);
	xcnt = unique(xs, xs+xcnt) - xs;
	sort(ys, ys+ycnt);
	ycnt = unique(ys, ys+ycnt) - ys;
	for(i=0;i<N;i++)
	{
		obj[i].x1 = lower_bound(xs, xs+xcnt, obj[i].x1) - xs;
		obj[i].x2 = lower_bound(xs, xs+xcnt, obj[i].x2) - xs;
		obj[i].y1 = lower_bound(ys, ys+ycnt, obj[i].y1) - ys;
		obj[i].y2 = lower_bound(ys, ys+ycnt, obj[i].y2) - ys;
	}
	return true;
}

void addeve(int x1, int x2, int y, int flag)
{
	eve[ecnt].l = x1;
	eve[ecnt].r = x2;
	eve[ecnt].flag = flag;
	eve[ecnt].pos = y;
	ecnt++;
}

void calc()
{
	int i, ppos;
	LLI pre;
	ans = 0;

	tree.init(xcnt, xs);
	ecnt = 0;
	for(i=0;i<N;i++)
	{
		addeve(obj[i].x1, obj[i].x2, obj[i].y1, 1);
		addeve(obj[i].x1, obj[i].x2, obj[i].y2, -1);
	}
	sort(eve, eve+ecnt);
	pre = tree.query();
	ppos = 0;
	for(i=0;i<ecnt;i++)
	{
		ans += pre * (ys[eve[i].pos] - ys[ppos]);
		tree.update(1, eve[i].l, eve[i].r, eve[i].flag);
		ppos = eve[i].pos;
		pre = tree.query();
	}
	ans += pre * (H - ys[ppos]);

	tree.init(ycnt, ys);
	ecnt = 0;
	for(i=0;i<N;i++)
	{
		addeve(obj[i].y1, obj[i].y2, obj[i].x1, 1);
		addeve(obj[i].y1, obj[i].y2, obj[i].x2, -1);
	}
	sort(eve, eve+ecnt);
	pre = tree.query();
	ppos = 0;
	for(i=0;i<ecnt;i++)
	{
		ans += pre * (xs[eve[i].pos] - xs[ppos]);
		tree.update(1, eve[i].l, eve[i].r, eve[i].flag);
		ppos = eve[i].pos;
		pre = tree.query();
	}
	ans += pre * (W - xs[ppos]);

	if(M == 1)
		ans /= 2;
	printf("%lld\n",ans);
}

int main()
{
	while(input())
	{
		calc();
	}
	return 0;
}
