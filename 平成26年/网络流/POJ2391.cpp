#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

#define MAXP 405
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

//////////////////////////////
typedef long long LLI;
#define MAXN 201
const LLI maxi = 0x3fffffffffffffff;
LLI map[MAXN][MAXN];
int num[MAXN], cap[MAXN];
int F, P, NC;

void Floyd()
{
	int i, j, k;

	for(k=1;k<=F;k++)
	{
		for(i=1;i<=F;i++)
		{
			for(j=1;j<=F;j++)
			{
				if(i!=k && j!=k)
				{
					map[i][j] = min(map[i][j], map[i][k]+map[k][j]);
				}
			}
		}
	}
}

bool judge(LLI dist)
{
	int i, j;

	for(i=1;i<=F;i++)
	{
		graph[0][i] = num[i];
		graph[i][0] = 0;
		graph[F+i][F*2+1] = cap[i];
		graph[F*2+1][F+i] = 0;
	}
	sp = 0;
	tp = F*2+1;
	np = F * 2 + 2;
	for(i=1;i<=F;i++)
	{
		for(j=1;j<=F;j++)
		{
			if(map[i][j] <= dist)
				graph[i][j+F] = 0x3fffffff;
			else
				graph[i][j+F] = 0;
			graph[j+F][i] = 0;
		}
	}
	return SAPCalc() == NC;
}

LLI BiSearch()
{
	LLI begin,end,mid;

	begin = 0;
	end = maxi-1;
	
	while(end > begin + 1)
	{
		mid = (begin + end) >> 1;
		if(!judge(mid))
		{
			begin = mid;
		}else{
			end = mid;
		}
	}

	if(judge(begin))
	{
		return begin;
	}else{
		return end;
	}
}

void input()
{
	int i, j, k, w;

	scanf("%d%d",&F,&P);
	NC = 0;
	for(i=1;i<=F;i++)
	{
		scanf("%d%d",&num[i],&cap[i]);
		NC += num[i];
	}
	for(i=1;i<=F;i++)
	{
		for(j=1;j<=F;j++)
		{
			map[i][j] = (i==j)?0:maxi;
		}
	}
	for(i=1;i<=P;i++)
	{
		scanf("%d%d%d",&j,&k,&w);
		map[j][k] = map[k][j] = min(map[j][k], LLI(w));
	}
	Floyd();
}

int main()
{
	input();
	if(!judge(maxi-1))
		printf("-1\n");
	else
		printf("%I64d\n",BiSearch());
	return 0;
}
