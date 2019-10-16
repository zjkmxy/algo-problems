/*
һ��GG���ں����ĺ�֮һ��MM��������MMͻȻ˵��һ�䣬������Լ�����������������ǳԵ���high��ʱ�򣬳ǹܳ����ˣ���Ϊ��˵�е���ǿ���������������������С����������ǰҲֻ����ս��Ϊ��5�����ң�һ���Ӿͱ���ɱ�ˡ���

���ⳡ��ɱ�У�GG�����ĺ󹬱���ֻ��Χ��Ⱥ�ڣ����ǲ��ҵ��ǣ��ǹܿ�����GGθ��ļ������������Ҫ����GG������GG��Ȼ������������Ը������GG���ź󹬿�ʼ�������˴������֮�á�

������ͼ��n��·�ڣ����̯��0��·�ڣ����˴���n-1��·�ڡ���m��ֻ�ܵ���ͨ���ĵ�·������n��·�ڣ�ÿ����·��һ����������ʾ�߹���Ҫ��ʱ�䡣������ͼû�л�·��������·��֮������ж���ͨ·������GGϣ������̵�ʱ�䵽���˴壬�����ҵ��ǣ��ǹ�Ϊ��ץס�����������Ƕ������п��и��٣����һ���ĳһʱ�̿ս���ĳһ����·�Ͻ��з�������������˲����ɣ���ϧ����̫����GGҲ���ڵ�һʱ��֪��������·�������ˣ���֮������·���޷�ͨ���ˡ��������ж���ֻ�����һ�οս������Ҳ�����GG����������·��ʱ����з�����Ҳ����˵��������GG��ĳ��·������һ���ʱ���������·�����ң��ǹ���ϣ�������ܵ��ӻ�GG������˴��ʱ�䡣

����GGϣ��֪�����Լ�����ܵ�����˴壬���㰲��֮��������󹬵�Լ�ᡣ

ע��˫�����Բ��ĵ�˼��������ѡ�񣬼�GGϣ��ʱ����̣��ǹ�ϣ��ʱ������������Ƕ��ǳ�������������ѵ�ѡ��



�����һ��Ϊ��������T��T<=30����

ÿ�����ݵ�һ�а�����������n,m��2<=n<=10000��1<=m<=100000������ʾ·�����͵�·����֮��m�����������еĵ�·��ÿ������������u,v,w��0<=u,v<n��0<w<=1000������ʾ·��u��·��v��һ����Ҫwʱ���߹��ĵ�·��

����ÿ���������һ����������ʾGG��󵽴���˴���Ҫ��ʱ�䡣���GG�޷�������˴壬���-1��


*/

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
#include<stack>
using namespace std;

#define MAXP 10005
#define MAXI 0x0fffffff
#define MAXQ 100005

struct NODE
{
	int dis;  //��̾��룻dis=min{dis, dis'+w}
	int dis2; //�ζ̾���
	int fin;  //���յĽ����fin=max{dis2,all_min{fin'+w}}

	int tail;
	int head;
	bool inol;
	int indeg;
}node[MAXP];

struct EDGE
{
	int st;
	int ed;
	int w;
	int next;
	int ne;
}edge[MAXQ];

int N, M;
int oplst[MAXP];
int cnt;

void input()
{
	int i,a,b,w;

	scanf("%d%d",&N,&M);
	cnt = 0;
	for(i=0;i<N;i++)
	{
		node[i].dis = MAXI;
		node[i].fin = MAXI;
		node[i].dis2 = MAXI;
		node[i].tail = -1;
		node[i].head = -1;
		node[i].indeg = 0;
		node[i].inol = false;
	}
	for(i=0;i<M;i++)
	{
		scanf("%d%d%d",&a,&b,&w);
		edge[i].st = a;
		edge[i].ed = b;
		edge[i].w = w;
		edge[i].next = node[b].tail;
		node[b].tail = i;
		edge[i].ne = node[a].head;
		node[a].head = i;
		node[a].indeg++;
	}
}

void topo()
{
	queue<int> cq;
	int cur, j, st;

	for(j=0;j<N;j++)
	{
		if(node[j].indeg == 0)
		{
			oplst[cnt] = j;
			cnt++;
			cq.push(j);
		}
	}
	while(!cq.empty())
	{
		cur = cq.front();
		cq.pop();
		for(j=node[cur].tail;j>=0;j=edge[j].next)
		{
			st = edge[j].st;
			node[st].indeg--;
			if(!node[st].inol && node[st].indeg == 0)
			{
				oplst[cnt] = st;
				cnt++;
				cq.push(st);
				node[st].inol = true;
			}
		}
	}
}

void calc1()
{
	int cur, j, st;
	int ptr;

	node[N-1].dis = 0;
	node[N-1].dis2 = 0;
	for(ptr=0;ptr<cnt;ptr++)
	{
		cur = oplst[ptr];
		for(j=node[cur].tail;j>=0;j=edge[j].next)
		{
			st = edge[j].st;

			if(node[cur].dis + edge[j].w <= node[st].dis)
			{
				node[st].dis2 = node[st].dis;
				node[st].dis = node[cur].dis + edge[j].w;
			}else if(node[cur].dis + edge[j].w <= node[st].dis2)
			{
				node[st].dis2 = node[cur].dis + edge[j].w;
			}
		}
	}
}

int calc2()
{
	int cur, j, ed, tmp;
	int ptr;

	node[N-1].fin = 0;
	for(ptr=0;ptr<cnt;ptr++)
	{
		cur = oplst[ptr];
		if(cur == N-1)
			continue;
		node[cur].fin = node[cur].dis2;
		tmp = MAXI;
		for(j=node[cur].head;j>=0;j=edge[j].ne)
		{
			ed = edge[j].ed;
			tmp = min(tmp, node[ed].fin + edge[j].w);
		}
		node[cur].fin = max(node[cur].fin, tmp);
	}
	if(node[0].fin >= MAXI)
		node[0].fin = -1;
	return node[0].fin;
}

int main()
{
	int T;

	scanf("%d",&T);
	while(T--)
	{
		input();
		topo();
		calc1();
		printf("%d\n",calc2());
	}
	return 0;
}
