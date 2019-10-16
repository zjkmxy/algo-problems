#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 502
#define MAXM 40002

int n, m;
int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos, root, rtcnt;
int cut;
int ehead[MAXN], edge[MAXM], enext[MAXM];

void Tarjan(int cur, int dep, int par)  //Tarjan�㷨����
{
	int i, j;

	DFN[cur] = low[cur] = dep;
	vis[cur] = 1;
	for(i=ehead[cur];i>=0;i=enext[i])
	{
		j = edge[i];
		if(vis[j] == 2)  //�ϳ�����
			continue;
		if(vis[j] == 0)
		{
			Tarjan(j, dep+1, cur);
			if(cur == root)
			{
				rtcnt++;
			}else{
				low[cur] = min(low[cur], low[j]);
				if(low[j] >= DFN[cur])
					cut = cur;  //��ʱcur�Ǹ��
				//if(low[j] > DFN[cur])  ʱcur����
			}
		}else if(j != par)
		{
			low[cur] = min(low[cur], DFN[j]);
		}
	}
}

bool judge()
{
	int j;

	memset(vis, 0, sizeof(vis));
	rtcnt = 0;
	root = 0;  //��ѡ��
	cut = -1;
	Tarjan(root, 1, -1);
	if(cut >= 0)  //cut�Ǹ��
		return false;
	if(rtcnt > 1)  //���Ǹ��
		return false;
	for(j=0;j<n;j++)
	{
		if(vis[j]==0)  //������ͨ
			return false;
	}

	return true;
}
/*
һ������u�Ǹ�㣬���ҽ�������(1)��(2)
  (1) uΪ��������u�ж���һ�������� 
  (2) u��Ϊ���������������(u,v)Ϊ��֦��(��Ƹ��ӱߣ���uΪv���������еĸ���)��ʹ��DFS(u)<=Low(v)��
һ�������(u,v)���ţ����ҽ���(u,v)Ϊ��֦�ߣ�������DFS(u)<Low(v)��

һ�����ŵ���ͨͼ����ΰ���ͨ���ӱ߱�ɱ�˫��ͨͼ������Ϊ����������е��ţ�Ȼ��ɾ����Щ�űߣ�ʣ�µ�ÿ����ͨ�鶼��һ��˫��ͨ��ͼ��
��ÿ��˫��ͨ��ͼ����Ϊһ�����㣬�ٰ��ű߼ӻ������������ͼһ����һ����������ͨ��Ϊ1��
ͳ�Ƴ����ж�Ϊ1�Ľڵ�ĸ�������ΪҶ�ڵ�ĸ�������Ϊleaf�����������������(leaf+1)/2���ߣ�����ʹ���ﵽ�߶���ͨ������������ӵı�������(leaf+1)/2��
���巽��Ϊ�����Ȱ������������������Զ������Ҷ�ڵ�֮������һ���ߣ��������԰��������㵽���ȵ�·�������е�������һ����Ϊһ���γɵĻ�һ����˫��ͨ�ġ�
Ȼ�����������������������Զ������Ҷ�ڵ㣬����һ��һ�����꣬ǡ����(leaf+1)/2�Σ������е���������һ��
*/
