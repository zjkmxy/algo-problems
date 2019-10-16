/*
LIKE vs CANDLE 
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<list>
using namespace std;

struct NODE{
	int V;
	int F;
	int S;
	int P;
	int dp[2];
	list<int> *chl; //子节点列表
};

NODE tree[50001];
int N,X,Y;

bool input()
{
	int i;

	if(scanf("%d %d %d",&N, &X, &Y) == EOF)
	{
		return false;
	}
	for(i=0;i<=N;i++)
		tree[i].chl = new list<int>();
	for(i=1;i<=N;i++)
	{
		scanf("%d %d %d %d", &tree[i].V, &tree[i].F, &tree[i].S, &tree[i].P);
		tree[tree[i].F].chl->push_back(i);
	}
	return true;
}

void clean()
{
	int i;
	for(i=0;i<=N;i++)
	{
		delete tree[i].chl;
	}
}

void calc1(int cur) //算法1中，dp[0]代表最大正向价值，dp[1]代表最大逆向价值，每一个节点的价值都包括自身的反转及其代价
{
	list<int>::iterator ptr;
	int sign,cost;

	sign = tree[cur].P ? -1 : 1;
	tree[cur].dp[0] = tree[cur].V * sign;
	tree[cur].dp[1] = - tree[cur].V * sign;
	if(!tree[cur].chl->empty()) //非叶子节点
	{
		for(ptr = tree[cur].chl->begin(); ptr != tree[cur].chl->end(); ptr++)
		{
			calc1(*ptr);
			tree[cur].dp[0] += tree[*ptr].dp[0];
			tree[cur].dp[1] += tree[*ptr].dp[1];
		}
	}
	if(tree[cur].S)
	{
		cost = Y;
		swap(tree[cur].dp[0], tree[cur].dp[1]);
	}else{
		cost = X;
	}
	if(cur > 0)
	{
		tree[cur].dp[0] = max(tree[cur].dp[0], tree[cur].dp[1] - cost);
		tree[cur].dp[1] = max(tree[cur].dp[1], tree[cur].dp[0] - cost);
	}
}

void calc2(int cur) //算法2中，dp[0/1]分别代表本节点反转与否所能得到的最大正向价值，而且不包括自己反转的代价
{
	list<int>::iterator ptr;
	int sign,cost;

	sign = tree[cur].P ? -1 : 1;
	tree[cur].dp[0] = tree[cur].V * sign;
	tree[cur].dp[1] = - tree[cur].V * sign;
	if(!tree[cur].chl->empty()) //非叶子节点
	{
		for(ptr = tree[cur].chl->begin(); ptr != tree[cur].chl->end(); ptr++)
		{
			calc2(*ptr);
			cost = tree[*ptr].S ? Y  : X;
			tree[cur].dp[0] += max(tree[*ptr].dp[0], tree[*ptr].dp[1] - cost);
			tree[cur].dp[1] += max(tree[*ptr].dp[1], tree[*ptr].dp[0] - cost);
		}
	}
	if(tree[cur].S)
	{
		swap(tree[cur].dp[0], tree[cur].dp[1]);
	}
}

int main()
{
	while(input())
	{
		calc2(0);
		if(tree[0].dp[0] >= 0)
		{
			printf("%d\n", tree[0].dp[0]);
		}else{
			printf("HAHAHAOMG\n");
		}
		clean();
	}
	return 0;
}
