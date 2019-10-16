#include<cstdio>
#include<cstdlib>
#include<stack>
using namespace std;

#define MAXN	100005
#define MAXI	0x7fffffff
#define MODN	9901
#define MID(l, r)	(((l)+(r))>>1)
int arr[MAXN], dp[MAXN], N, D;
class SGETREE{
private:
	struct NODE
	{
		int lchd, rchd;
		int sum;
	}node[MAXN<<5];
	int cnt;
	int query(int id, int l, int r, int lbound, int rbound)
	{
		int mid = MID(lbound, rbound);
		if(id == -1)
			return 0;
		if(lbound == l && rbound == r)
			return node[id].sum;
		if(l > mid)
			return query(node[id].rchd, l, r, mid + 1, rbound);
		if(r <= mid)
			return query(node[id].lchd, l, r, lbound, mid);
		return (query(node[id].lchd, l, mid, lbound, mid) + query(node[id].rchd, mid + 1, r, mid + 1, rbound)) % MODN;
	}
	void update(int id, int pos, int val, int lbound, int rbound)
	{
		int mid;
		while(lbound != pos || rbound != pos){
			mid = MID(lbound, rbound);
			node[id].sum = (node[id].sum + val) % MODN;
			if(pos <= mid){
				if(node[id].lchd == -1){
					node[cnt].lchd = node[cnt].rchd = -1;
					node[cnt].sum = 0;
					node[id].lchd = cnt;
					cnt++;
				}
				id = node[id].lchd;
				rbound = mid;
			}else{
				if(node[id].rchd == -1){
					node[cnt].lchd = node[cnt].rchd = -1;
					node[cnt].sum = 0;
					node[id].rchd = cnt;
					cnt++;
				}
				id = node[id].rchd;
				lbound = mid + 1;
			}
		}
		node[id].sum = node[id].sum + val;
	}
public:
	void init()
	{
		cnt = 1;
		node[0].lchd = node[0].rchd = -1;
		node[0].sum = 0;
	};
	int query(int l, int r)
	{
		return query(0, l, r, 0, MAXI);
	}
	void update(int pos, int val)
	{
		update(0, pos, val, 0, MAXI);
	}
}tree;

int dp_calc()
{
	int i, ret = 0;

	tree.init();
	for(i=0;i<N;i++){
		dp[i] = tree.query(max(arr[i] - D, 0), arr[i] + D);
		tree.update(arr[i], (dp[i] + 1) % MODN);
		ret = (ret + dp[i]) % MODN;
	}
	return ret;
}

bool input()
{
	int i;
	if(scanf("%d%d",&N,&D)==EOF)
		return false;
	for(i=0;i<N;i++)
		scanf("%d",&arr[i]);
	return true;
}

int main()
{
	while(input())
		printf("%d\n", dp_calc());
	return 0;
}
