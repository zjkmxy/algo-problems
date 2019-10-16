#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<ctime>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXN 100005
const int MAXI = 0x3fffffff;
typedef long long LLI;
#define LCHD(x) ((x)<<1)
#define RCHD(x) (((x)<<1)+1)
#define MID(l, r) (((l)+(r))>>1)

//内存池
namespace pool
{
	char pool[MAXN*400];
	int pos;
	inline void init()
	{
		pos = 0;
	};
	void* alloc(int cnt)
	{
		void* ret = pool + pos;
		pos += cnt;
		return ret;
	};
};
//线段
struct Seg
{
	int K;
	LLI B;
	double x1, x2;
	Seg(int K=0, int B=0, double x1=-MAXI, double x2=MAXI)
	{
		this->K = K;
		this->B = B;
		this->x1 = x1;
		this->x2 = x2;
	};
	//排序
	bool operator<(const Seg& other)const
	{
		if(this->K == other.K)
			return this->B < other.B;
		else
			return this->K > other.K;
	};
	//交点横坐标
	inline double insec(const Seg& other)const
	{
		return double(other.B - this->B) / double(this->K - other.K);
	};
}segs[MAXN];
int inarr[MAXN];
LLI sum[MAXN];
//相对位置计算
int getposr(const Seg& p, const Seg& q, double insec)
{
	double a = p.x1, b = MAXI;
	//本来判定是否在上下应该是a=min(p.x1,q.x1),b=max(p.x2,q.x2)，但是这显然是不适宜的，因为q的区间是继承来的没有意义。
	if(insec > a && insec < b)
		return 0;
	if((insec >= b) == (p.B < q.B))
		return -1;
	else
		return +1;
}

//下凸包求值器
class Eval
{
private:
	Seg* arr;
	int cnt;
public:
	void init(int n)
	{
		arr = (Seg*)pool::alloc(n * sizeof(Seg));
	};
	Eval():arr(NULL),cnt(0){};
	Eval(int n):cnt(0){init(n);};
	//按序添加线段，保证s大于之前所有
	void add(const Seg& s)
	{
		double is = -MAXI;
		int flag;
		if(cnt > 0 && arr[cnt-1].K == s.K)
			return;
		while(cnt > 0)
		{
			is = arr[cnt-1].insec(s);
			flag = getposr(arr[cnt-1], s, is);
			if(flag == 0)
			{
				arr[cnt-1].x2 = is;
				break;
			}
			if(flag == 1)
			{
				return ;
			}else{
				cnt--;
			}
		}
		arr[cnt] = Seg(s);
		arr[cnt].x1 = is;
		arr[cnt].x2 = MAXI;
		cnt++;
	};
	//合并两个求值器
	Eval merge(const Eval& other)
	{
		int i, j;
		Eval ret = Eval(cnt + other.cnt);
		i = j = 0;
		while(i < cnt || j < other.cnt)
		{
			if(i == cnt || (j < other.cnt && other.arr[j] < arr[i]))
			{
				ret.add(other.arr[j]);
				j++;
			}else{
				ret.add(arr[i]);
				i++;
			}
		}
		return ret;
	};
	//求值
	LLI eval(int x)
	{
		int begin=0, end=cnt-1, mid;
		while(end > begin + 1)
		{
			mid = MID(begin, end);
			if(arr[mid].x1 <= x && arr[mid].x2 >= x)
				return LLI(arr[mid].K) * LLI(x) + arr[mid].B;
			if(arr[mid].x1 < x)
			{
				begin = mid;
			}else{
				end = mid;
			}
		}
		mid = (arr[end].x1 >= x) ? begin : end;
		return LLI(arr[mid].K) * LLI(x) + arr[mid].B;
	};
};

class Tree
{
private:
	struct NODE
	{
		int l, r;
		Eval val;
	}node[MAXN<<2];
public:
	Tree(){};
	void build(int id, int l, int r)
	{
		int mid = MID(l, r);
		node[id].l = l;
		node[id].r = r;
		if(l == r)
		{
			node[id].val = Eval(1);
			node[id].val.add(segs[l]);
		}else{
			build(LCHD(id), l, mid);
			build(RCHD(id), mid+1, r);
			node[id].val = node[LCHD(id)].val.merge(node[RCHD(id)].val);
		}
	};
	LLI calc(int id, int l, int r, int x)
	{
		int mid = MID(node[id].l, node[id].r);
		if(node[id].l >= l && node[id].r <= r)
		{
			return node[id].val.eval(x);
		}
		if(r <= mid)
		{
			return calc(LCHD(id), l, r, x);
		}
		if(l > mid)
		{
			return calc(RCHD(id), l, r, x);
		}
		return min(calc(LCHD(id), l, mid, x), calc(RCHD(id), mid+1, r, x));
	};
};
Tree tree;
int n, q;

bool input()
{
	int i;
	if(scanf("%d",&n)==EOF)
		return false;
	for(i=1;i<=n;i++)
	{
		scanf("%d",&inarr[i]);
		sum[i] = sum[i-1] + inarr[i];
		segs[i].K = inarr[i];
		segs[i].B = LLI(inarr[i]) * i - sum[i];
	}
	pool::init();
	tree.build(1, 1, n);
	return true;
}

void domain()
{
	int a, b, x;
	LLI ans;
	scanf("%d",&q);
	while(q--)
	{
		scanf("%d%d",&a,&b);
		x = a - b;
		a = max(b - a + 1, 1);
		ans = sum[b] + tree.calc(1, a, b, x);
		printf("%I64d\n",ans);
	}
}

int main()
{
	while(input())
		domain();
	return 0;
}
