/*
SGU 326 Perspective
���ж���򼸳�������ÿ����������һ֧����Ӯ����һ֧���������
���ж��鱻�ֳɼ���С�飬һЩ��������ͬһС������н��еĽ���������һЩ��������С����С��Ľ�����
����������ӵķ����������������ж���μӵı�����Ŀ����ÿ����������֮��ʣ�µı���������ȷ������Ƿ�����С�����еõ�һ��

��Ϊ2��Ϊ��������ӡ�
Դ�������������ô���������
������2ֻ���������������������ô���������
�����������һ֧��������ʤ������ȥ�Լ���ǰʤ������������
��������Ϊ�ܱ���������������������ɹ������
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXP 1000
#define MAXI 0x7fffffff

int graph[MAXP][MAXP]; //�����ڽӾ���
int dist[MAXP];        //����ڵ�������
int gap[MAXP];         //dist=i�Ľڵ����
int pprev[MAXP];       //ǰ�����
int lowf[MAXP];        //��С����������ΪҪ���ݣ����Ա���������ǣ�
int sp,tp,np;          //Դ���㣬������

struct MATCH{
	int st;
	int ed;
	int cnt;
};

int w[30],r[30],s[30]; //�Ѿ�ʤ���ı����������ı�����������������
MATCH mat[MAXP];       //ʣ�µ����ڱ���
int cnt;
int N;

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
			gap[mindis]++;
			if(a != sp)
				a = pprev[a];
		}
	}

	return ret;
}

bool input()
{
	int i, j, tmp;

	if(scanf("%d",&N)==EOF)
		return false;
	for(i=0;i<N;i++)
	{
		scanf("%d",&w[i]);
	}
	for(i=0;i<N;i++)
	{
		scanf("%d",&r[i]);
	}
	cnt = 0;
	//��0�ӵ����ڱ���ȫ��ֱ��Ӯ
	for(j=0;j<N;j++)
	{
		scanf("%d",&tmp);
		w[0] += tmp;
		r[0] -= tmp;
		r[j] -= tmp;
	}
	w[0] += r[0];
	r[0] = 0;
	for(i=1;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			scanf("%d",&tmp);
			//��¼���ı�������
			if((j <= i)||(tmp == 0))
				continue;
			mat[cnt].st = i;
			mat[cnt].ed = j;
			mat[cnt].cnt = tmp;
			cnt++;
		}
	}

	return true;
}

bool calc()
{
	int i, sum, flow;

	memset(graph, 0, sizeof(graph));
	sum = 0;
	np = cnt + N + 1;
	sp = 0;
	tp = cnt + N;
	for(i=0;i<cnt;i++)
	{
		graph[sp][N+i] = mat[i].cnt;
		graph[N+i][mat[i].st] = mat[i].cnt;
		graph[N+i][mat[i].ed] = mat[i].cnt;
		sum += mat[i].cnt;
	}
	for(i=1;i<N;i++)
	{
		graph[i][tp] = w[0] - w[i];
		if(graph[i][tp] < 0)
			return false;
	}
	flow = SAPCalc();
	if(flow == sum)
		return true;
	else
		return false;
}

int main()
{
	while(input())
	{
		if(calc())
			printf("YES\n");
		else
			printf("NO\n");
	}

	return 0;
}