#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
using namespace std;

const int MAXN = 100000;
const int MAXT = (MAXN*30);
#define MID(l, r) (((l)+(r))>>1)
#define NIL 0

class TREE
{
private:
	struct NODE
	{
		int lchd, rchd;
		bool full;
	}node[MAXT];
	int cnt;

	int build()
	{
		node[cnt].full = false;
		node[cnt].lchd = node[cnt].rchd = NIL;
		cnt++;
		return (cnt - 1);
	}
	inline void popup(int id)
	{
		node[id].full = node[id].lchd && node[id].rchd && node[node[id].lchd].full && node[node[id].rchd].full;
	}
public:
	TREE():cnt(0)
	{
		build();
	};
	int query(int id, int pos, int l, int r)
	{
		int mid, ret;
		if(node[id].full)
		{
			return -1;
		}
		if(l == r)
		{
			node[id].full = true;
			return l;
		}
		mid = MID(l, r);
		if(node[id].lchd == NIL)
			node[id].lchd = build();
		if(pos <= mid)
		{
			ret = query(node[id].lchd, pos, l, mid);
		}else{
			if(node[id].rchd == NIL)
				node[id].rchd = build();
			ret = query(node[id].rchd, pos, mid+1, r);
			if(ret < 0)
				ret = query(node[id].lchd, mid, l, mid);
		}
		popup(id);
		return ret;
	}
};
TREE *tree;

int arr[MAXN];
int fee[MAXN+1];
int n;
int main()
{
	int i, j, ans = 0, mfee = 0, ene;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	sort(arr, arr+n);
	tree = new TREE;
	memset(fee, 0, sizeof(fee));
	ene = arr[n-1];
	for(i=0;i<n;i++)
	{
		j = tree->query(0, arr[i], 1, ene);
		if(j == -1)
			continue;
		j = arr[i] - j + 1;
		fee[j]++;
		mfee = max(mfee, j);
	}
	for(i=1;i<=mfee;i++)
	{
		if(ene > i * fee[i])
		{
			ene -= i * fee[i];
			ans += fee[i];
		}else{
			ans += ene / i;
			break;
		};
	}
	printf("%d\n",ans);
	delete tree;
	return 0;
}