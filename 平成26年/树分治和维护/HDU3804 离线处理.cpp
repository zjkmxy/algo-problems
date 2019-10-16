/*
给一课树，每次询问1~X的路径上不超过Y的最大边权
离线,先读入所有查询,再深度遍历这棵树,遍历到一条边就把边的权值扔到set里,返回的时候再把这条边的权值删掉.
如果遍历到的当前点有询问,则对set进行upbounder,找到比当前y大的第一个位置,对迭代器自减即可.
*/
#include<cstdio>
#include<cstdlib>
#include<stack>
#include<set>
#include<algorithm>
using namespace std;

#define MAXN	100005
#define MAXM	200005
struct NODE{
	int head;	//边起始
	int par;	//父亲节点
	int pe;		//父边
	int qhead;	//查询起点
	bool vis;
}node[MAXN];
struct EDGE{
	int a, b, next;
	int val;
}edge[MAXM];
struct QUERY{
	int x, y;
	int qnext;
	int val;
}query[MAXN];
struct STATE{
	int p;
	int e;
	STATE(int _p, int _e){
		p = _p;
		e = _e;
	};
	STATE(const STATE& other){
		p = other.p;
		e = other.e;
	}
};
int N, ecnt, Q;

stack<STATE> oplst;
multiset<int> data;
void DFS()
{
	STATE *cur, _tmp(1, node[1].head);
	int j, ed;
	multiset<int>::iterator it;

	for(j=1;j<=N;j++){
		node[j].vis = false;
		node[j].par = -1;
		node[j].pe = -1;
	}
	node[1].vis = true;
	oplst.push(_tmp);
	data.clear();
	data.insert(-1);
	while(!oplst.empty()){
		cur = &oplst.top();

		for(j=node[cur->p].qhead;j>=0;j=query[j].qnext){
			it = data.upper_bound(query[j].y);
			it--;
			query[j].val = (*it);
		}

		j = cur->e;
		if(j > 0 && edge[j].b == node[cur->p].par){
			cur->e = edge[j].next;
			j = cur->e;
		}
		if(j >= 0){
			cur->e = edge[j].next;
			ed = edge[j].b;
			oplst.push(STATE(ed, node[ed].head));
			data.insert(edge[j].val);
			node[ed].par = cur->p;
			node[ed].vis = true;
			node[ed].pe = j;
		}else{
			oplst.pop();
			if(node[cur->p].pe >= 0)
				data.erase(data.find(edge[node[cur->p].pe].val));
		}
	}
}

void addedge(int a, int b, int val)
{
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = node[a].head;
	edge[ecnt].val = val;
	node[a].head = ecnt;
	ecnt++;
	edge[ecnt].a = b;
	edge[ecnt].b = a;
	edge[ecnt].next = node[b].head;
	edge[ecnt].val = val;
	node[b].head = ecnt;
	ecnt++;
}

void domain()
{
	int i, M;
	int a, b, v;
	scanf("%d",&N);
	M = N - 1;
	for(i=1;i<=N;i++)
		node[i].head = node[i].qhead = -1;
	ecnt = 0;
	while(M--){
		scanf("%d%d%d",&a,&b,&v);
		addedge(a, b, v);
	}
	scanf("%d",&Q);
	for(i=0;i<Q;i++){
		scanf("%d%d",&a,&b);
		query[i].x = a;
		query[i].y = b;
		query[i].qnext = node[a].qhead;
		node[a].qhead = i;
	}
	DFS();
	for(i=0;i<Q;i++){
		printf("%d\n",query[i].val);
	}
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
		domain();
	return 0;
}
