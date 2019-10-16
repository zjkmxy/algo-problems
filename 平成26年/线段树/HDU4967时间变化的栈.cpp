#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<queue>
using namespace std;

#define MAXN 50005
#define LCHD(x) (((x)<<1))
#define RCHD(x) (((x)<<1)+1)
#define MID(l, r) (((l)+(r))>>1)
int tms[MAXN];
int N;

struct QUERY
{
	char op[6];
	int tm;
	int val;
}queris[MAXN];
int arr[MAXN];  //某时间插入的数值

int BiSearch(int x)
{
	int begin,end,mid;

	begin = 1;
	end = N;

	while(end > begin + 1)
	{
		mid = (begin + end) >> 1;
		if(tms[mid] == x)
			return mid;
		if(tms[mid] < x)
		{
			begin = mid;
		}else{
			end = mid;
		}
	}
	return (tms[end] > x) ? begin : end;
}

class TREE
{
private:
	struct NODE
	{
		int l;
		int r;
		int sum;	//区间和
		int rmax;	//区间后缀和
	}node[MAXN<<2];

	void popup(int id)
	{
		node[id].sum = node[LCHD(id)].sum + node[RCHD(id)].sum;
		node[id].rmax = max(node[LCHD(id)].rmax + node[RCHD(id)].sum, node[RCHD(id)].rmax);
	};
public:
	TREE(){};
	void build(int id, int l, int r)
	{
		int mid = MID(l, r);
		node[id].l = l;
		node[id].r = r;
		node[id].sum = node[id].rmax = 0;
		if(r > l)
		{
			build(LCHD(id), l, mid);
			build(RCHD(id), mid+1, r);
		}
	};
	void update(int id, int pos, int val)
	{
		int mid = MID(node[id].l, node[id].r);
		if(node[id].l == node[id].r)
		{
			node[id].sum += val;
			node[id].rmax += val;
			return ;
		}
		if(pos <= mid)
			update(LCHD(id), pos, val);
		else
			update(RCHD(id), pos, val);
		popup(id);
	};
	//传入右缀和，转出位点或者无法找到
	//成功，传出位置；不成功，加和
	/*
	int sech(int id, int t, int &ssum)
	{
		int mid = MID(node[id].l, node[id].r);
		int ret;
		if(node[id].l == node[id].r)
		{
			ssum += node[id].sum;
			if(ssum > 0)
				return node[id].l;
			else
				return -1;
		}
		//左劈分
		if(t <= mid)
			return sech(LCHD(id), t, ssum);
		//右跨立
		if(t > node[id].r)
		{
			if(node[id].rmax + ssum <= 0)
			{
				ssum += node[id].sum;
				return -1;
			}
		}
		//右端查询
		ret = sech(RCHD(id), t, ssum);
		if(ret > 0)
			return ret;
		//左端查询
		return sech(LCHD(id), t, ssum);
	};
	*/
	//和上面的等价
	int sech(int id, int t, int &ssum)
	{
		int mid = MID(node[id].l, node[id].r);
		int ret;
		if(t >= node[id].r)
		{
			if(node[id].rmax + ssum <= 0)
			{
				ssum += node[id].sum;
				return -1;
			}
		}
		if(node[id].l == node[id].r)
			return node[id].l;
		if(mid < t)
		{
			ret = sech(RCHD(id), t, ssum);
			if(ret > 0)
				return ret;
		}
		return sech(LCHD(id), t, ssum);
	};
};
TREE tree;

bool input()
{
	int i;
    scanf("%d",&N);
    if(N == 0)
        return false;
    for(i=1;i<=N;i++)
    {
		scanf("%s",&queris[i].op);
        if(queris[i].op[1] == 'u')
        {
            scanf("%d",&queris[i].val);
        }
        scanf("%d",&queris[i].tm);
		tms[i] = queris[i].tm;
    }
	sort(tms+1, tms+N+1);
	for(i=1;i<=N;i++)
	{
		queris[i].tm = BiSearch(queris[i].tm);
	}
	tree.build(1, 1, N);

    return true;
}

void domain()
{
	int i, ss;
	for(i=1;i<=N;i++)
    {
		if(queris[i].op[1] == 'u')
        {
			arr[queris[i].tm] = queris[i].val;
			tree.update(1, queris[i].tm, 1);
        }
        if(queris[i].op[1] == 'o')
        {
            tree.update(1, queris[i].tm, -1);
        }
        if(queris[i].op[1] == 'e')
        {
			ss = 0;
			queris[i].val = tree.sech(1, queris[i].tm, ss);
			if(queris[i].val > 0)
				queris[i].val = arr[queris[i].val];
        }
    }
	for(i=1;i<=N;i++)
	{
		if(queris[i].op[1] == 'e')
			printf("%d\n",queris[i].val);
	}
}

int main()
{
    int cas = 0;
    while(input())
    {
        printf("Case #%d:\n",++cas);
        domain();
    }
    return 0;
}
