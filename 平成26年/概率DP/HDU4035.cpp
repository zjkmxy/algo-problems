/*
HDU 4035

    dp���������⡣
    ���⣺
    ��n�����䣬��n-1�������ͨ������ʵ���Ͼ��γ���һ������
    �ӽ��1��������ʼ�ߣ���ÿ�����i����3�ֿ��ܣ�
        1.��ɱ�����ص����1��������Ϊki��
        2.�ҵ����ڣ��߳��Թ� ������Ϊei��
        3.�͸õ�������m���ߣ������һ��
    ���߳��Թ���Ҫ�ߵı���������ֵ��

    �� E[i]��ʾ�ڽ��i����Ҫ�߳��Թ���Ҫ�ߵı�����������E[1]��Ϊ����

    Ҷ�ӽ�㣺
    E[i] = ki*E[1] + ei*0 + (1-ki-ei)*(E[father[i]] + 1);
         = ki*E[1] + (1-ki-ei)*E[father[i]] + (1-ki-ei);

    ��Ҷ�ӽ�㣺��mΪ���������ı�����
    E[i] = ki*E[1] + ei*0 + (1-ki-ei)/m*( E[father[i]]+1 + ��( E[child[i]]+1 ) );
         = ki*E[1] + (1-ki-ei)/m*E[father[i]] + (1-ki-ei)/m*��(E[child[i]]) + (1-ki-ei);

    ���ÿ����㣺E[i] = Ai*E[1] + Bi*E[father[i]] + Ci;

    ���ڷ�Ҷ�ӽ��i����jΪi�ĺ��ӽ�㣬��
    ��(E[child[i]]) = ��E[j]
                   = ��(Aj*E[1] + Bj*E[father[j]] + Cj)
                   = ��(Aj*E[1] + Bj*E[i] + Cj)
    ���������ʽ�ӵ�
    (1 - (1-ki-ei)/m*��Bj)*E[i] = (ki+(1-ki-ei)/m*��Aj)*E[1] + (1-ki-ei)/m*E[father[i]] + (1-ki-ei) + (1-ki-ei)/m*��Cj;
    �ɴ˿ɵ�
    Ai =        (ki+(1-ki-ei)/m*��Aj)   / (1 - (1-ki-ei)/m*��Bj);
    Bi =        (1-ki-ei)/m            / (1 - (1-ki-ei)/m*��Bj);
    Ci = ( (1-ki-ei)+(1-ki-ei)/m*��Cj ) / (1 - (1-ki-ei)/m*��Bj);

    ����Ҷ�ӽ��
    Ai = ki;
    Bi = 1 - ki - ei;
    Ci = 1 - ki - ei;

    ��Ҷ�ӽ�㿪ʼ��ֱ����� A1,B1,C1;

    E[1] = A1*E[1] + B1*0 + C1;
    ����
    E[1] = C1 / (1 - A1);
    �� A1������1���޽�...

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
