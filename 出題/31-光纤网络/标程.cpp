//注意：此标称为本题的最简解法而非最优解法，另写一个BFS计算答案可以更快
#include<cstdio>
#include<cctype>
#include<queue>
using namespace std;

#define MAXN 1000000
typedef long long int LL;

long FastScan() {
	long ret = 0;
	char c;

	while(!isdigit(c = getchar()));
	ret = c - '0';
	while(isdigit(c = getchar())) {
		ret = ret * 10 + (c - '0');
	}
	return ret;
}

void Init(int N);	//初始化
void AddEdge(int a, int b);	//增加新的边
LL Calc();	//计算答案

int main()
{
	int i, a, b, n;

	n = FastScan();
	Init(n);
	for(i=1;i<n;i++){
		a = FastScan();
		b = FastScan();
		AddEdge(a, b);
	}
	printf("%lld\n", Calc());
	return 0;
}

struct NODE{
	//邻接表用
	int head;
	//查找直径和继续搜索用
	int dis;
	int par;
	//直径链上的编号
	int no;
}node[MAXN];
struct EDGE{
	int b, next;
}edge[MAXN<<1];
int lin[MAXN];	//直径链，展开的点
int arr[MAXN];	//最长路条数
int ecnt, lcnt, lcnt2;	//边数，直径长，展开点数目

int FindPath(int st, int &cnt)	//返回距离st最远的点以及个数
{
	int cur, j, ed;
	queue<int> oplst;

	node[st].dis = 0;
	node[st].par = -1;
	oplst.push(st);
	cnt = 0;
	while(!oplst.empty()){
		cur = oplst.front();
		oplst.pop();
		if(node[cur].dis > node[st].dis){
			st = cur;
			cnt = 1;
		}else if(node[cur].dis == node[st].dis){
			++cnt;
		}
		for(j=node[cur].head;j>=0;j=edge[j].next){
			ed = edge[j].b;
			if(ed != node[cur].par && node[ed].no < 0){
				node[ed].dis = node[cur].dis + 1;
				node[ed].par = cur;
				oplst.push(ed);
			}
		}
	}
	return st;
}

void Init(int N)
{
	int i;

	for(i=0;i<N;i++){
		node[i].head = -1;
		node[i].no = -1;
	}
	ecnt = 0;
}

void AddEdge(int a, int b)
{
	edge[ecnt].b = b;
	edge[ecnt].next = node[a].head;
	node[a].head = ecnt;
	++ecnt;
	edge[ecnt].b = a;
	edge[ecnt].next = node[b].head;
	node[b].head = ecnt;
	++ecnt;
}

LL Calc()
{
	int cnt, st, i;
	LL ret, sum = 0;

	st = FindPath(1, cnt);
	st = FindPath(st, cnt);
	for(i=0;st>=0;i++,st=node[st].par){
		lin[i] = st;
	}
	lcnt = i;
	if((lcnt & 1) == 0){
		lin[0] = lin[(lcnt >> 1) - 1];
		lin[1] = lin[lcnt >> 1];
		node[lin[0]].no = 0;
		node[lin[1]].no = 1;
		lcnt2 = 2;
	}else{
		st = lin[lcnt >> 1];
		lcnt2 = 0;
		node[st].no = 0;
		for(i=node[st].head;i>=0;i=edge[i].next){
			lin[lcnt2] = edge[i].b;
			node[edge[i].b].no = lcnt2;
			lcnt2++;
		}
	}
	for(sum = 0, i = 0; i < lcnt2; i++){
		st = FindPath(lin[i], cnt);
		arr[i] = (node[st].dis == (lcnt >> 1) - 1) ? cnt : 0;
		sum += arr[i];
	}
	for(ret = 0, i = 0; i < lcnt2; i++)
		ret += LL(arr[i]) * (sum - arr[i]);
	return ret / 2;
}