#include <cstdio>
#include <algorithm>
#include <cctype>
#include <queue>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#define MAXP 1005
#define MAXQ 15005
#define MAXI 0x7fffffff
#define MAXC 0x0fffffff
#define BIGM MAXC

struct EDGE{
	int st, ed;
	int flow;
	int low;
	int upf;
	int cost;
	int hnext;
	int tnext;
	int hprev;
	int tprev;

	char set;
}edge[MAXQ + MAXP];
int ecnt;
int ecnt2;
int sece;
bool search_method;

struct NODE{
	int head;
	int tail;
	int d;
	int pi;

	int pred;
	int depth;
}node[MAXP];
int ncnt;

struct LOOP{
	int e;
	int prev;
	int next;
}loop[MAXP + 3];
int lcnt;

int addedge(int a, int b, int low, int upf, int cost, int &cnt)
{
	edge[cnt].st = a;
	edge[cnt].ed = b;
	edge[cnt].low = low;
	edge[cnt].upf = upf;
	edge[cnt].cost = cost;
	edge[cnt].hnext = -1;
	edge[cnt].tnext = -1;
	edge[cnt].hprev = -1;
	edge[cnt].tprev = -1;
	cnt++;

	return (cnt - 1);
}

void insert_tree(int j)
{
	int a, b;

	a = edge[j].st;
	b = edge[j].ed;

	edge[j].hprev = -1;
	edge[j].hnext = node[a].head;
	if(node[a].head >= 0)
		edge[node[a].head].hprev = j;
	node[a].head = j;

	edge[j].tprev = -1;
	edge[j].tnext = node[b].tail;
	if(node[b].tail >= 0)
		edge[node[b].tail].tprev = j;
	node[b].tail = j;
}

void delete_tree(int j)
{
	int a, b;

	a = edge[j].st;
	b = edge[j].ed;

	if(edge[j].hprev >= 0)
	{
		edge[edge[j].hprev].hnext = edge[j].hnext;
	} else{
		node[a].head = edge[j].hnext;
	}
	if(edge[j].hnext >= 0)
	{
		edge[edge[j].hnext].hprev = edge[j].hprev;
	}
	edge[j].hprev = -1;
	edge[j].hnext = -1;

	if(edge[j].tprev >= 0)
	{
		edge[edge[j].tprev].tnext = edge[j].tnext;
	} else{
		node[b].tail = edge[j].tnext;
	}
	if(edge[j].tnext >= 0)
	{
		edge[edge[j].tnext].tprev = edge[j].tprev;
	}
	edge[j].tprev = -1;
	edge[j].tnext = -1;
}

void M_init()
{
	int i, j;

	node[0].d = 0;
	node[0].head = -1;
	node[0].tail = -1;

	for(i = 0; i < ecnt; i++)
	{
		edge[i].flow = edge[i].low;
		edge[i].set = 'L';
		node[edge[i].st].d -= edge[i].low;
		node[edge[i].ed].d += edge[i].low;
	}

	ecnt2 = ecnt;
	for(i = 1; i <= ncnt; i++)
	{
		if(node[i].d > 0)
		{
			j = addedge(i, 0, 0, MAXC, BIGM, ecnt2);
			edge[j].flow = node[i].d;
		} else{
			j = addedge(0, i, 0, MAXC, BIGM, ecnt2);
			edge[j].flow = -node[i].d;
		}
		edge[j].set = 'T';
		insert_tree(j);
	}
}

void calc_pi(int root, int val, int dep, int pred)
{
	int j;

	node[root].pi = val;
	node[root].depth = dep;
	node[root].pred = pred;
	for(j = node[root].head; j != -1; j = edge[j].hnext)
	{
		if(j != pred)
		{
			calc_pi(edge[j].ed, val - edge[j].cost, dep + 1, j);
		}
	}
	for(j = node[root].tail; j != -1; j = edge[j].tnext)
	{
		if(j != pred)
		{
			calc_pi(edge[j].st, val + edge[j].cost, dep + 1, j);
		}
	}
}

int calc_dual1()
{
	int i, ret = -1, dual0 = -1, dual;

	for(i = 0; i<ecnt2; i++)
	{
		dual = edge[i].cost - node[edge[i].st].pi + node[edge[i].ed].pi;
		if((edge[i].set == 'L' && dual < 0) || (edge[i].set == 'U' && dual > 0))
		{
			dual = labs(dual);
			if(dual > dual0)
			{
				ret = i;
				dual0 = dual;
			}
		}
	}

	return ret;
}

int calc_dual2()
{
	int i, dual;

	for(i = sece; i < ecnt2; i++)
	{
		dual = edge[i].cost - node[edge[i].st].pi + node[edge[i].ed].pi;
		if((edge[i].set == 'L' && dual < 0) || (edge[i].set == 'U' && dual > 0))
		{
			sece = i + 1;
			return i;
		}
	}
	for(i = 0; i < sece; i++)
	{
		dual = edge[i].cost - node[edge[i].st].pi + node[edge[i].ed].pi;
		if((edge[i].set == 'L' && dual < 0) || (edge[i].set == 'U' && dual > 0))
		{
			sece = i + 1;
			return i;
		}
	}

	return -1;
}

bool augment(int rda)
{
	int p, q;
	int pl, ql, pp, qp, maxflow;
	bool flagp, flagq, flag, ret;

	lcnt = 0;
	if(edge[rda].set == 'L')
	{
		p = edge[rda].st;
		q = edge[rda].ed;
	} else{
		q = edge[rda].st;
		p = edge[rda].ed;
	}
	loop[lcnt].e = rda;
	loop[lcnt].prev = 1;
	loop[lcnt].next = 2;
	pl = 1;
	ql = 2;
	loop[pl].e = -1;
	loop[pl].next = 0;
	loop[pl].prev = -1;
	loop[ql].e = -1;
	loop[ql].next = -1;
	loop[ql].prev = 0;
	lcnt = 3;

	maxflow = edge[rda].upf - edge[rda].low;
	while(p != q)
	{
		if(node[p].depth > node[q].depth)
		{
			flagp = true;
			flagq = false;
		} else if(node[p].depth < node[q].depth)
		{
			flagp = false;
			flagq = true;
		} else{
			flagp = true;
			flagq = true;
		}

		if(flagp)
		{
			pp = node[p].pred;
			if(edge[pp].ed == p)
			{
				maxflow = min(maxflow, edge[pp].upf - edge[pp].flow);
				p = edge[pp].st;
			} else{
				maxflow = min(maxflow, edge[pp].flow - edge[pp].low);
				p = edge[pp].ed;
			}
			loop[pl].e = pp;
			loop[pl].prev = lcnt;
			loop[lcnt].e = -1;
			loop[lcnt].next = pl;
			loop[lcnt].prev = -1;
			pl = lcnt;
			lcnt++;
		}

		if(flagq)
		{
			qp = node[q].pred;
			if(edge[qp].st == q)
			{
				maxflow = min(maxflow, edge[qp].upf - edge[qp].flow);
				q = edge[qp].ed;
			} else{
				maxflow = min(maxflow, edge[qp].flow - edge[qp].low);
				q = edge[qp].st;
			}
			loop[ql].e = qp;
			loop[ql].next = lcnt;
			loop[lcnt].e = -1;
			loop[lcnt].next = -1;
			loop[lcnt].prev = ql;
			ql = lcnt;
			lcnt++;
		}
	}

	flag = false;
	while(loop[pl].next >= 0)
	{
		pl = loop[pl].next;
		ql = loop[pl].e;
		if(ql == -1)
			break;
		if(ql == rda)
		{
			ret = (flag == (edge[rda].set == 'L'));
		}

		if(edge[ql].st == p)
		{
			edge[ql].flow += maxflow;
			if(!flag && (edge[ql].flow == edge[ql].upf))
			{
				edge[ql].set = 'U';
				flag = true;
				delete_tree(ql);
			} else{
				edge[ql].set = 'T';
			}
			p = edge[ql].ed;
		} else{
			edge[ql].flow -= maxflow;
			if(!flag && (edge[ql].flow == edge[ql].low))
			{
				edge[ql].set = 'L';
				flag = true;
				delete_tree(ql);
			} else{
				edge[ql].set = 'T';
			}
			p = edge[ql].st;
		}
	}

	return ret;
}

bool NSMCalc(int &cost)
{
	int rda, i;

	M_init();
	calc_pi(0, 0, 0, -1);
	rda = (search_method ? calc_dual1() : calc_dual2());
	while(rda >= 0)
	{
		int j;

		insert_tree(rda);
		if(augment(rda))
		{
			i = edge[rda].st;
			j = edge[rda].ed;
			calc_pi(i, node[j].pi + edge[rda].cost, node[j].depth + 1, rda);
		} else{
			i = edge[rda].ed;
			j = edge[rda].st;
			calc_pi(i, node[j].pi - edge[rda].cost, node[j].depth + 1, rda);
		}

		rda = (search_method ? calc_dual1() : calc_dual2());
	}

	for(i = ecnt; i<ecnt2; i++)
	{
		if(edge[i].flow > 0)
			return false;
	}
	cost = 0;
	for(i = 0; i < ecnt; i++)
	{
		cost += edge[i].flow * edge[i].cost;
	}

	return true;
}

void init_graph(int pointnum, bool method)
{
	int i;

	ncnt = pointnum;
	ecnt = 0;
	sece = 0;
	for(i = 0; i <= ncnt; i++)
	{
		node[i].d = 0;
		node[i].head = -1;
		node[i].tail = -1;
		node[i].pred = -1;
		node[i].pi = 0;
		node[i].depth = 0;
	}
	search_method = method;
}
//////////////////////////////////////////////////////////////////////////

typedef long long ll;
const int MAXN = 55;
ll pos[MAXN];	//For each num, the possible position
int ans[MAXN];
int N, M1, M2;

//restrain a num to a range, 1 <= l <= r <= N
//masks start with 1<<1, end with 1<<N
void restrain(int l, int r, int v, bool bMin){
	ll mask = (1ll << ll(r + 1)) - (1ll << ll(l));
	pos[v] &= mask;
	if(bMin){
		for(-- v; v > 0; -- v){
			pos[v] &= ~mask;
		}
	} else{
		for(++ v; v <= N; ++ v){
			pos[v] &= ~mask;
		}
	}
}

bool solve(){
	int i, j, num;
	int __tmp;
	bool ret;


	//possible ?
	init_graph(2 * N, false);
	for(i = 1; i <= N; i ++){
		node[i].d = 1;
		node[i + N].d = -1;
		for(j = 1; j <= N; j ++){
			if(! (pos[i] & (1ll << ll(j))))
				continue;
			addedge(i, N + j, 0, 1, 0, ecnt);
		}
	}
	ret = NSMCalc(__tmp);
	if(!ret){
		return false;
	}

	//for every pos
	for(num = 1; num <= N; num ++){
		init_graph(2 * N, false);
		for(i = 1; i <= N; i ++){
			node[i].d = 1;
			node[i + N].d = -1;
			for(j = 1; j <= N; j ++){
				if(! (pos[i] & (1ll << ll(j))))
					continue;
				addedge(i, N + j, 0, 1, (j == num ? i : 0), ecnt);
			}
		}
		NSMCalc(__tmp);

		for(i = 0; i < ecnt; i ++){
			if(edge[i].flow > 0 && edge[i].ed == num + N){
				ans[num] = edge[i].st;
				pos[edge[i].st] = 1ll << ll(num);
			}
		}
	}

	return true;
}

bool domain(){
	int i, l, r;

	if(scanf("%d%d%d", &N, &M1, &M2) == EOF)
		return false;

	for(i = 1; i <= N; i ++){
		pos[i] = (1ll << ll(N + 1)) - (1ll << 1);
	}
	while(M1 --){
		scanf("%d%d%d", &l, &r, &i);
		restrain(l, r, i, true);
	}
	while(M2 --){
		scanf("%d%d%d", &l, &r, &i);
		restrain(l, r, i, false);
	}

	if(solve()){
		for(i = 1; i < N; i ++)
			printf("%d ", ans[i]);
		printf("%d\n", ans[N]);
	} else{
		puts("-1");
	}

	return true;
}

int main(){
	while(domain());
	return 0;
}