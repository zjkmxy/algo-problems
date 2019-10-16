//假设根是0层，问一棵有n个节点的二叉树的第m层最多有多少个节点。
#pragma GCC diagnostic ignored "-Wunused-result"
#include <stdio.h>

int log2b(int x)
{
	int ret = 0;
	while(x > 0){
		x >>= 1;
		++ ret;
	}
	return ret;
}

int domain(int n, int m)
{
	int depth, a;
	int ans = 0;
	if(m <= 30 && n >= (1 << (m + 1)) - 1)
		return (1 << m);
	if(m == 0)
		return (n == 0 ? 0 : 1);
	for(; n > m; m = depth - 1){
		depth = log2b(n + 1) - 1;
		a = (1 << depth) - 1;
		n -= (m - depth);
		if(n > a){
			ans += ((a + 1) >> 1);
			n = n - a - 1;
		}else{
			-- n;
		}
	}
	return ans;
}

int domain2(int n, int m)
{
	int a, k, ans = 0;

	if(m <= 30 && n >= (1 << (m + 1)) - 1)
		return (1 << m);
	if(m == 0)
		return (n == 0 ? 0 : 1);
	while(n > m){
		n -= m + 1;
		ans += 1;
		for(a = 2, k = 0; a <= n + 1; a <<= 1, ++ k){
			n -= a - 1;
			ans += (a >> 1);
		}
		m = k;
	}
	return ans;
}

int main()
{
	int t, n, m;
	scanf("%d",&t);
	while(t--){
		scanf("%d%d",&n,&m);
		printf("%d\n",domain(n,m));
	}
	return 0;
}
