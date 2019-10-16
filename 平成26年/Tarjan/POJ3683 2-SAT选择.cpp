#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 2005
#define MAXM 5000005

int DFN[MAXN], low[MAXN], vis[MAXN];
int oplst[MAXN], pos = 0, now = 0;
int parset[MAXN], setcnt[MAXN];
int ehead[MAXN], edge[MAXM], enext[MAXM], ecnt;

int ehead2[MAXN], edge2[MAXM], enext2[MAXM], ecnt2, n2, indeg[MAXN];
int ops[MAXN], chose[MAXN];

void addedge(int st, int ed)
{
	edge[ecnt] = ed;
	enext[ecnt] = ehead[st];
	ehead[st] = ecnt;
	ecnt++;
}

void addedge2(int st, int ed)
{
	edge2[ecnt2] = ed;
	enext2[ecnt2] = ehead2[st];
	ehead2[st] = ecnt2;
	ecnt2++;
}

void Tarjan(int cur)  //Tarjan����
{
	int i, j;

	DFN[cur] = low[cur] = now;
	now++;
	oplst[pos] = cur;
	pos++;
	vis[cur] = 1;
	for(i=ehead[cur];i>=0;i=enext[i])
	{
		j = edge[i];
		if(vis[j] == 2)  //�ϳ�����
			continue;
		if(vis[j] == 0)
		{
			Tarjan(j);
			low[cur] = min(low[cur], low[j]);
		}else{
			low[cur] = min(low[cur], DFN[j]);
		}
	}
	if(DFN[cur] == low[cur])
	{
		do{
			pos--;
			j = oplst[pos];
			vis[j] = 2;
			parset[j] = n2;
			setcnt[cur]++;
		}while(cur != j);
		n2++;
	}
}

//����ѡ��
void topsel()
{
	queue<int> oplst;
	int i, j;

	for(i=0;i<n2;i++)
	{
		if(indeg[i] == 0)
			oplst.push(i);
	}
	while(!oplst.empty())
	{
		i = oplst.front();
		oplst.pop();
		if(chose[i] == 0)
		{
			chose[i] = 1;
			chose[ops[i]] = -1; //ѡ��һ���㣬��ô�����㼰�����ｫ����ѡ��
			for(j=ehead2[i];j>=0;j=enext2[j])
			{
				indeg[edge2[j]]--;
				if(indeg[edge2[j]] == 0)
				{
					oplst.push(edge2[j]);
				}
			}
		}
	}
}

int n;
struct CERE{
	int beg;
	int end;
	int las;
}cere[MAXN];

int chgtime(int hh, int mm)
{
	return (hh * 60 + mm);
}

bool paradox(int st1, int ed1, int st2, int ed2)
{
	return (st1 < ed2 && st2 < ed1);
}

void input()
{
	int i, a, b, c, d, e;

	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d:%d %d:%d %d", &a, &b, &c, &d, &e);
		cere[i].beg = chgtime(a, b);
		cere[i].end = chgtime(c, d);
		cere[i].las = e;
	}
	memset(ehead, -1, sizeof(ehead));
	ecnt = 0;
	for(a=0;a<n;a++)
	{
		for(b=0;b<n;b++)  //˫��ѡ�񣬵���addedge�ǵ����
				  //ԭ���ǣ�a��bì�ܣ���ôѡ��aһ������ѡ��b����Ӧ��ѡ��bһ������ѡ��a�����ǲ�ѡ��b��һ����Ҫѡ��a
		{
			if(a == b)
				continue;
			//��ÿһ��ì��������ԭ�����������ض�ͬʱѡ��
			if(paradox(cere[a].beg, cere[a].beg+cere[a].las, cere[b].beg, cere[b].beg+cere[b].las))
			{
				addedge(a, b+n);
			}
			if(paradox(cere[a].beg, cere[a].beg+cere[a].las, cere[b].end-cere[b].las, cere[b].end))
			{
				addedge(a, b);
			}
			if(paradox(cere[a].end-cere[a].las, cere[a].end, cere[b].end-cere[b].las, cere[b].end))
			{
				addedge(a+n, b);
			}
			if(paradox(cere[a].end-cere[a].las, cere[a].end, cere[b].beg, cere[b].beg+cere[b].las))
			{
				addedge(a+n, b+n);
			}
		}
	}
}

bool judge()
{
	int i, j;

	//Tarjan����
	n2 = 0;
	for(i=0;i<2*n;i++)
	{
		if(vis[i] == 0)
			Tarjan(i);
	}
	for(i=0;i<n;i++)
	{
		if(parset[i] == parset[i+n])  //ͬһ��Ĳ�����ͬһ��ǿ��ͨ������
			return false;
		ops[parset[i]] = parset[i+n];
		ops[parset[i+n]] = parset[i];
	}
	//����ͼ
	memset(ehead2, -1, sizeof(ehead2));
	ecnt2 = 0;
	for(i=0;i<2*n;i++)
	{
		for(j=ehead[i];j>=0;j=enext[j])
		{
			if(parset[i] == parset[edge[j]])
				continue;

			addedge2(parset[edge[j]], parset[i]);
			indeg[parset[i]]++;
		}
	}
	//����ѡ��
	topsel();
	return true;
}

int main()
{
	int i, a, b;

	input();
	if(!judge())
	{
		printf("NO\n");
	}else{
		printf("YES\n");
		for(i=0;i<n;i++)
		{
			if(chose[parset[i]]==1)
			{
				a = cere[i].beg;
				b = cere[i].beg + cere[i].las;
			}else{
				a = cere[i].end - cere[i].las;
				b = cere[i].end;
			}
			printf("%02d:%02d %02d:%02d\n",a/60,a%60,b/60,b%60);
		}
	}

	return 0;
}
