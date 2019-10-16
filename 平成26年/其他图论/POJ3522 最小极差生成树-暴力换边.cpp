#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
using namespace std;

#define MAXN 105
#define MAXM 10005
#define MAXI 0x3fffffff

int parset[MAXN];
struct EDGE
{
	int a, b, w;
	bool operator<(const EDGE other)const
	{
		return this->w < other.w;
	}
}edge[MAXM];
int ehead[MAXN], etail[MAXN], enext[MAXM], eback[MAXM], used[MAXM];
int pred[MAXN];
vector<int> vec;
int n, m;

int fdp(int x)
{
	if(parset[x] == 0)
		return x;
	else
	{
		parset[x] = fdp(parset[x]);
		return parset[x];
	}
}

bool merge(int a, int b)
{
	int ap = fdp(a), bp = fdp(b);
	if(ap == bp)
		return false;
	else
	{
		parset[ap] = bp;
		return true;
	}
}

bool DFS(int id, int dst, int par)
{
	int j;
	pred[id] = par;
	if(id==dst)
		return true;
	for(j=ehead[id];j>=0;j=enext[j])
	{
		if(j==par||!used[j])
			continue;
		if(DFS(edge[j].b, dst, j))
			return true;
	}
	for(j=etail[id];j>=0;j=eback[j])
	{
		if(j==par||!used[j])
			continue;
		if(DFS(edge[j].a, dst, j))
			return true;
	}
	return false;
}

int repl(int e)
{
	int me = -1, mw = MAXI, pos, cur;
	DFS(edge[e].a, edge[e].b, e);
	for(pos=edge[e].b;pos!=edge[e].a;)
	{
		cur=pred[pos];
		if(edge[cur].w < mw)
		{
			mw = edge[cur].w;
			me = cur;
		}
		if(edge[cur].a==pos)
			pos = edge[cur].b;
		else
			pos = edge[cur].a;
	}
	used[me] = false;
	return mw;
}

//ÊÂÏÈÅÅÐò
int Kruscal()
{
	int i, cnt = 0;
	int a, b, ret = MAXI;

	vec.clear();
	for(i=0;i<m;i++)
	{
		used[i] = true;
		vec.push_back(edge[i].w);
		if(merge(edge[i].a, edge[i].b))
		{
			cnt++;
		}else{
			vec.erase(lower_bound(vec.begin(), vec.end(), repl(i)));
		}
		if(cnt >= n-1)
		{
			a = *vec.begin();
			b = *(vec.end()-1);
			ret = min(ret, b - a);
		}
	}
	return ret;
}

bool input()
{
	int i, ret;
	cin >> n >> m;
	if(n == 0)
		return false;
	for(i=0;i<=n;i++)
	{
		ehead[i] = -1;
		etail[i] = -1;
		parset[i] = 0;
		pred[i] = -1;
	}
	for(i=0;i<m;i++)
	{
		cin >> edge[i].a >> edge[i].b >> edge[i].w;
	}
	sort(edge, edge+m);
	for(i=0;i<m;i++)
	{
		enext[i] = ehead[edge[i].a];
		eback[i] = etail[edge[i].b];
		ehead[edge[i].a] = i;
		etail[edge[i].b] = i;
		used[i] = false;
	}
	ret = Kruscal();
	if(ret >= MAXI)
		cout << -1 << endl;
	else
		cout << ret << endl;
	return true;
}

int main()
{
	while(input());
	return 0;
}
