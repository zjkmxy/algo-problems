#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 300
#define MAXI 0x7fffffff

int graph[MAXP][MAXP]; //�����ڽӾ���
int dist[MAXP];        //����ڵ�������
int gap[MAXP];         //dist=i�Ľڵ����
int pprev[MAXP];       //ǰ�����
int lowf[MAXP];        //��С����������ΪҪ���ݣ����Ա���������ǣ�
int sp,tp,np;          //Դ���㣬������

int build_dist()
{
	queue<int> oplst;
	int a, b;

	memset(dist, -1, sizeof(int) * np);
	memset(gap, 0, sizeof(int) * np);
	dist[tp] = 0;
	gap[0]++;
	oplst.push(tp);
	while(!oplst.empty())
	{
		a = oplst.front();
		oplst.pop();
		for(b=0;b<np;b++)
		{
			if((dist[b] >= 0) || (graph[b][a] <= 0))
				continue;

			dist[b] = dist[a] + 1;
			gap[dist[b]]++;
			oplst.push(b);
		}
	}

	return dist[sp];
}

int SAPCalc()
{
	int a, b, ret, mindis;

	ret = 0;
	if(build_dist() == -1)
		return 0;
	a = sp;
	memset(lowf, 0, sizeof(int) * np);
	memset(pprev, -1, sizeof(int) * np);
	while(dist[sp] < np)
	{
		lowf[sp] = MAXI;
		for(b=0;b<np;b++)
		{
			if((graph[a][b] > 0)&&(dist[a] == dist[b] + 1)&&(dist[b] >= 0))
			{
				break;
			}
		}
		if(b < np)  //�ҵ������
		{
			lowf[b] = min(lowf[a], graph[a][b]);
			pprev[b] = a;
			a = b;
			if(a == tp) //�ҵ�����·
			{
				ret += lowf[tp];
				while(pprev[a] != -1)
				{
					graph[pprev[a]][a] -= lowf[tp];
					graph[a][pprev[a]] += lowf[tp];
					a = pprev[a];
				}//��ʼ��һ�ε�Ѱ��
				a = sp;
				memset(lowf, 0, sizeof(int) * np);
				memset(pprev, -1, sizeof(int) * np);
			}
		}else{      //û�������
			//�ر��dist
			mindis = MAXI;
			for(b=0;b<np;b++)
			{
				if((graph[a][b] > 0)&&(mindis > dist[b] + 1)&&(dist[b] >= 0))
				{
					mindis = dist[b] + 1;
				}
			}
			gap[dist[a]]--;
			if(gap[dist[a]] == 0)
				break;
			dist[a] = mindis;
			if(mindis >= MAXP)  //���ﲻ�ӵĻ���RE����
				break;
			gap[mindis]++;
			if(a != sp)
				a = pprev[a];
		}
	}

	return ret;
}

bool input()
{
	int n, i, a, b, w;

	if(scanf("%d%d",&n,&np)==EOF)
		return false;

	sp = 0;
	tp = np - 1;
	memset(graph, 0, sizeof(graph));
	for(i=0;i<n;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		a--;
		b--;
		graph[a][b] += w;
	}
	return true;
}

int main()
{
	while(input())
	{
		printf("%d\n",SAPCalc());
	}

	return 0;
}