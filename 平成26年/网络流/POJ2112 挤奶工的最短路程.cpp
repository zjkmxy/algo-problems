/*
POJ2112 Optimal Milking

��K̨���̻�����Cͷţ(ͳ��Ϊ���壩��ÿ̨���̻���ֻ������Mͷţ���м��̡�
���ڸ���dis[K + C][K + C]�ľ���dis[i][j]����Ϊ0���ʾ��i�����嵽��j������֮����·��dis[i][j]���Ǹ�·�ĳ��ȡ���1 <= K <= 30,1 <= C <= 200��

����������ô������Cͷţ��K̨�������̣�ʹ����Ҫ���·�̵����̻�������ţ���ߵ�·�����٣���������Сֵ��


������ģ�ͣ�
ÿ����ţ���ն�ֻ�ܵ���һ����������ÿ��������ֻ����M����ţ���ɰ���ţ�����������е�����
ÿ����ţ�ͼ���������һ���ڵ㣬���һ��Դ�����ߵ�������ţ�ڵ㣬��Щ����������1��
���һ����㣬ÿ�������������ߵ�������Щ�ߵ���������M��
���ִ𰸣���ţ�ͻ���������ͼ���ɡ�

*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 500
#define MAXI 0x7fffffff
#define MAXL 0x0fffffff

int graph[MAXP][MAXP]; //�����ڽӾ���
int depth[MAXP];       //�ڵ���
int sp,tp,np;          //Դ���㣬������

int K, C, M, mdis;
int dis[MAXP][MAXP];

int build_depth()
{
	queue<int> oplst;
	int a, b;

	memset(depth, -1, sizeof(int) * np);
	depth[sp] = 0;
	oplst.push(sp);
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		for(b=0;b<np;b++)
		{
			if((depth[b] >= 0) || (graph[a][b] <= 0))
				continue;

			depth[b] = depth[a] + 1;
			if(b == tp)
				break;
			else 
				oplst.push(b);
		}
	}

	return depth[tp];
}

int find(int a, int low)
{
	int ret, b;

	if(a == tp)
		return low;
	for(b=0;b<np;b++)
	{
		if((depth[b] != depth[a] + 1)||(graph[a][b] <= 0))  //������b=a+1�ſ��ԣ�����಻����
			continue;
		ret = find(b, min(low, graph[a][b]));
		if(ret <= 0)
			continue;
		graph[a][b] -= ret;
		graph[b][a] += ret;
		return ret;
	}
	return 0;
}

int Dinic()
{
	int ret, ad;

	ret = 0;
	while(build_depth() > 0)
	{
		ad = find(sp, MAXI);
		while(ad > 0)
		{
			ret += ad;
			ad = find(sp, MAXI);
		}
	}

	return ret;
}

void Floyd(int n)   //Floyd�������������
{
	int i,j,k;
	for(k=1;k<=n;k++)
	{
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				if((i != k)&&(j != k))
				{
					if(dis[i][j] > dis[i][k] + dis[k][j])
						dis[i][j] = dis[i][k] + dis[k][j];
				}
			}
		}
	}
}

bool judge(int mindis)
{
	int i, j;

	memset(graph, 0, sizeof(graph));
	sp = 0;
	tp = K + C + 1;
	np = K + C + 2;
	for(i=K+1;i<=K+C;i++)
	{
		graph[0][i] = 1;
	}
	for(j=1;j<=K;j++)
	{
		graph[j][tp] = M;
	}
	for(i=K+1;i<=K+C;i++)
	{
		for(j=1;j<=K;j++)
		{
			graph[i][j] = (dis[i][j] <= mindis) ? 1 : 0;
		}
	}

	return (Dinic() == C);
}

int bisearch()
{
	int start,end,mid;

	start = 0;
	end = mdis;

	while(end > start + 1)
	{
		mid = (start + end) / 2;
		if(judge(mid))
		{
			end = mid;
		}else{
			start = mid;
		}
	}

	if(judge(start))
		return start;
	return end;
}

bool input()
{
	int i, j;

	if(scanf("%d%d%d",&K,&C,&M)==EOF)
		return false;

	for(i=1;i<=K+C;i++)
	{
		for(j=1;j<=K+C;j++)
		{
			scanf("%d",&dis[i][j]);
			if((i != j)&&(dis[i][j] == 0))
				dis[i][j] = MAXL;
			mdis = max(mdis, dis[i][j]);
		}
	}
	Floyd(K+C);
	mdis = 0;
	for(i=1;i<=K+C;i++)
	{
		for(j=1;j<=K+C;j++)
		{
			mdis = max(mdis, dis[i][j]);
		}
	}

	return true;
}

int main()
{
	while(input())
	{
		printf("%d\n",bisearch());
	}

	return 0;
}