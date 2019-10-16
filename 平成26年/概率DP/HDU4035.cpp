/*
HDU 4035

    dp求期望的题。
    题意：
    有n个房间，由n-1条隧道连通起来，实际上就形成了一棵树，
    从结点1出发，开始走，在每个结点i都有3种可能：
        1.被杀死，回到结点1处（概率为ki）
        2.找到出口，走出迷宫 （概率为ei）
        3.和该点相连有m条边，随机走一条
    求：走出迷宫所要走的边数的期望值。

    设 E[i]表示在结点i处，要走出迷宫所要走的边数的期望。E[1]即为所求。

    叶子结点：
    E[i] = ki*E[1] + ei*0 + (1-ki-ei)*(E[father[i]] + 1);
         = ki*E[1] + (1-ki-ei)*E[father[i]] + (1-ki-ei);

    非叶子结点：（m为与结点相连的边数）
    E[i] = ki*E[1] + ei*0 + (1-ki-ei)/m*( E[father[i]]+1 + ∑( E[child[i]]+1 ) );
         = ki*E[1] + (1-ki-ei)/m*E[father[i]] + (1-ki-ei)/m*∑(E[child[i]]) + (1-ki-ei);

    设对每个结点：E[i] = Ai*E[1] + Bi*E[father[i]] + Ci;

    对于非叶子结点i，设j为i的孩子结点，则
    ∑(E[child[i]]) = ∑E[j]
                   = ∑(Aj*E[1] + Bj*E[father[j]] + Cj)
                   = ∑(Aj*E[1] + Bj*E[i] + Cj)
    带入上面的式子得
    (1 - (1-ki-ei)/m*∑Bj)*E[i] = (ki+(1-ki-ei)/m*∑Aj)*E[1] + (1-ki-ei)/m*E[father[i]] + (1-ki-ei) + (1-ki-ei)/m*∑Cj;
    由此可得
    Ai =        (ki+(1-ki-ei)/m*∑Aj)   / (1 - (1-ki-ei)/m*∑Bj);
    Bi =        (1-ki-ei)/m            / (1 - (1-ki-ei)/m*∑Bj);
    Ci = ( (1-ki-ei)+(1-ki-ei)/m*∑Cj ) / (1 - (1-ki-ei)/m*∑Bj);

    对于叶子结点
    Ai = ki;
    Bi = 1 - ki - ei;
    Ci = 1 - ki - ei;

    从叶子结点开始，直到算出 A1,B1,C1;

    E[1] = A1*E[1] + B1*0 + C1;
    所以
    E[1] = C1 / (1 - A1);
    若 A1趋近于1则无解...

*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXN 10005
#define eps 1e-9

struct NODE{
	int head;
	int deg;
	double k, e;
	double A, B, C;
}node[MAXN];
struct EDGE{
	int ed;
	int next;
}edge[MAXN<<1];
int n;

bool calc_dp(int i, int par)
{
	double divs = 1;
	int j;

	node[i].A = node[i].k;
	node[i].B = (1.00 - node[i].k - node[i].e) / node[i].deg;
	node[i].C = 1.00 - node[i].k - node[i].e;
	for(j=node[i].head;j>=0;j=edge[j].next)
	{
		if(edge[j].ed == par)
			continue;

		if(!calc_dp(edge[j].ed, i))
			return false;

		node[i].A += node[edge[j].ed].A  * node[i].B;
		node[i].C += node[edge[j].ed].C  * node[i].B;
		divs -= node[edge[j].ed].B  * node[i].B;
	}
	if(fabs(divs) < eps)
		return false;

	node[i].A /= divs;
	node[i].B /= divs;
	node[i].C /= divs;

	return true;
}

void input()
{
	int i, a, b;

	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		node[i].deg = 0;
		node[i].head = -1;
	}
	for(i=0;i<n-1;i++)
	{
		scanf("%d%d",&a,&b);
		edge[i<<1].ed = b;
		edge[i<<1].next = node[a].head;
		node[a].head = (i << 1);
		node[a].deg++;
		edge[(i<<1)+1].ed = a;
		edge[(i<<1)+1].next = node[b].head;
		node[b].head = (i << 1) + 1;
		node[b].deg++;
	}
	for(i=1;i<=n;i++)
	{
		scanf("%lf%lf",&node[i].k,&node[i].e);
		node[i].k /= 100.0;
		node[i].e /= 100.0;
	}
}

int main()
{
	int t, i;
	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		input();
		printf("Case %d: ",i);
		if(calc_dp(1, -1) && fabs(1 - node[1].A) >= eps)
		{
			printf("%.6lf\n",node[1].C / (1 - node[1].A));
		}else{
			printf("impossible\n");
		}
	}
	return 0;
}
