/*
POJ-1947

我们考虑，一个根节点u，一个u的子节点w。一个dp[i][K],表示以节点i为根的子树有k个节点，删除边数最少。

1、如果删除u->w的边，那么我们只考虑以u为根的子树；

2、否则，我们考虑u、w都在一个子树的情况；

第一种情况下：我们考虑将dp[u][k] +1，因为删除操作是对于u树而言；

第二种情况下：我们考虑背包dp，如果u为根的子树取i个节点，w为根的子树取k-i个节点，那么将u和w合并为以u为根节点的子树，那么我们只考虑dp[u][i] + dp[w][k-i]；

方程构造完成只有一半功劳，下面还要考虑边界。

如果对于有1个节点的情况，那么应该为0，因为一个节点表示了单独存在，即使它有子节点（可以认为这时还没有删除任何一条边，所以没有删除操作）。

然而对于最后一颗整树而言，就是枚举每个节点为根节点的情况，找到最少删除边的能够有p个节点，这里还要注意的是，对于整棵树的根，删除数就是其本身，然而对于其他节点当做根节点时，必须加1，因为当把它当做根时，其实必须删除它指向父节点的边。
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
using namespace std;

#define NSIZE 151

struct NODE
{
	int lchd; //左孩子
	int rbro; //右兄弟
	int par;  //父节点
	int dp[NSIZE]; //表示以此为根的子树中要有i个节点最少可以剪多少条
};
NODE tree[NSIZE];
int n,p;

void calc_dp(int u)
{
	int i,w,k;

	tree[u].dp[1] = 0; //这里边界条件设置成0的原因是：如果是叶子，自然0；否则会在逐步合并的过程中给他增加子节点，每增加一个子节点都强迫增加1单位的dp。
	for(w=tree[u].lchd;w>0;w=tree[w].rbro) //子节点的循环必须在外面，因为是以“加入子节点->合并背包”为核心的DP，而不是以计算i为核心的dp
	{
		calc_dp(w);
		for(i=p;i>=0;i--)
		{
			tree[u].dp[i]++;  //删除边。因为加入了子树w，所以这不是一种DP选择，而是强制删除。
			for(k=0;k<=i;k++) //不删除边，合并泛化背包
			{
				tree[u].dp[i] = min(tree[u].dp[i], tree[u].dp[k] + tree[w].dp[i-k]);
			}
		}
	}
}

void input()
{
	int i,l,f,j;

	scanf("%d %d", &n, &p);
	for(i=1;i<=n-1;i++)
	{
		scanf("%d %d", &f, &l);
		tree[l].rbro = tree[f].lchd;
		tree[f].lchd = l;
		tree[l].par  = f;

		for(j=0;j<=p;j++)
		{
			tree[i].dp[j] = 0x0fffffff;
		}
	}
}

int main()
{
	int i,j,ret;

	input();
	for(i=1;i<=n;i++)
	{
		if(tree[i].par == 0)
		{
			calc_dp(i);
			break;
		}
	}
	ret = tree[i].dp[p];
	for(j=1;j<=n;j++)
	{
		ret = min(ret, (j == i) ? tree[j].dp[p] : (tree[j].dp[p]+1));
	}
	printf("%d\n",ret);

	return 0;
}
