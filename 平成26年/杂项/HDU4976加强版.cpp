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

const int MAXN = 1000000;
const int MAXT = (MAXN*80);
const int MAXI = 0x3fffffff;
typedef long long LL;
#define MID(l, r) (((l)+(r))>>1)

class TREE
{
private:
	struct NODE
	{
		int l, r;
		int lchd, rchd;
		bool full;
	}node[MAXT];
	int cnt;

	int build(int l, int r)
	{
		node[cnt].full = false;
		node[cnt].l = l;
		node[cnt].r = r;
		node[cnt].lchd = node[cnt].rchd = NULL;
		cnt++;
		return (cnt - 1);
	}
	inline void popup(int id)
	{
		node[id].full = node[id].lchd && node[id].rchd && node[node[id].lchd].full && node[node[id].rchd].full;
	}
public:
	TREE():cnt(0){};
	void init()
	{
		cnt = 0;
		build(1, MAXI);
	}
	int query(int id, int pos)
	{
		int mid, ret;
		if(node[id].full)
		{
			return -1;
		}
		if(node[id].l == node[id].r)
		{
			node[id].full = true;
			return node[id].l;
		}
		mid = MID(node[id].l, node[id].r);
		if(node[id].lchd == NULL)
			node[id].lchd = build(node[id].l, mid);
		if(pos <= mid)
		{
			ret = query(node[id].lchd, pos);
		}else{
			if(node[id].rchd == NULL)
				node[id].rchd = build(mid + 1, node[id].r);
			ret = query(node[id].rchd, pos);
			if(ret < 0)
				ret = query(node[id].lchd, mid);
		}
		popup(id);
		return ret;
	}
};
TREE tree;

int arr[MAXN];
int fee[MAXN+1];
int n;
void domain()
{
	int i, j, ans = 0, mfee = 0, ene;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	sort(arr, arr+n);
	tree.init();
	memset(fee, 0, sizeof(fee));
	for(i=0;i<n;i++)
	{
		j = tree.query(0, arr[i]);
		if(j == -1)
			continue;
		j = arr[i] - j + 1;
		fee[j]++;
		mfee = max(mfee, j);
	}
	ene = arr[n-1];
	for(i=1;i<=mfee;i++)
	{
		if(ene > LL(i) * LL(fee[i]))
		{
			ene -= i * fee[i];
			ans += fee[i];
		}else{
			ans += ene / i;
			break;
		};
	}
	printf("%d\n",ans);
}

int main()
{
	int t, cas = 0;

	scanf("%d",&t);
	for(cas = 1;cas <= t; cas++)
	{
		printf("Case #%d: ",cas);
		domain();
	}
	return 0;
}
