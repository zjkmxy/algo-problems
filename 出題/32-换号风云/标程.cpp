#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-result"
#include<stdio.h>          
#include<math.h>   
#include<algorithm>

const int MAXN = 50005;
const int LOGN = 16;
const int MAXA = 10;
const int MAXI = 0x3fffffff;
const double lg2 = log(2.00);
#define nullptr NULL
struct EDGE{
	int a, b;
	EDGE* next;
}edge[MAXN << 1];
typedef EDGE* PEDGE;
struct NODE{
	int depth;			//深度
	int pf;
	PEDGE head;
	struct DATA{
		int par;		//祖先编号
		int cnt[MAXA];	//最小换乘次数（-1不可能）
	}data[LOGN+1];
}node[MAXN];
int cnt, sig;
int depth[MAXN<<1], anc[MAXN<<1], rmqarr[MAXN][LOGN+2];
int N, ecnt;

//a节点，k级父节点换乘次数
bool calc_cnt(int a, int k)
{
	int i, j, b;
	bool flag;
	for(i=0;i<MAXA;i++)
		node[a].data[k].cnt[i] = MAXI;
	if(k == 1){
		//直接父亲不需要设置
		b = node[a].data[1].par;
		if(b <= 0)
			return false;
		flag = false;
		for(i=0;i<MAXA;i++){
			if(node[a].data[0].cnt[i] == 0 && node[b].data[0].cnt[i] == 0){
				flag = true;
				break;
			}
		}
		if(!flag)
			return true;
		for(i=0;i<MAXA;i++){
			if(node[b].data[0].cnt[i] > 0){
				node[a].data[1].cnt[i] = MAXI;
			}else if(node[a].data[0].cnt[i] == 0){
				node[a].data[1].cnt[i] = 0;
			}else{
				node[a].data[1].cnt[i] = 1;
			}
		}
	}else{
		//间接父亲先设置再计算
		b = node[a].data[k - 1].par;
		if(b <= 0)
			return false;
		node[a].data[k].par = node[b].data[k - 1].par;
		if(node[a].data[k].par <= 0)
			return false;
		for(i=0;i<MAXA;i++){
			for(j=0;j<MAXA;j++){
				node[a].data[k].cnt[i] = std::min(node[a].data[k].cnt[i], node[a].data[k-1].cnt[j] + node[b].data[k-1].cnt[i] + (i != j));
			}
		}
	}
	return true;
}

void DFS(int st, int par)
{
	PEDGE j;
	int now, i;
	//LCA
	node[st].pf = ++cnt;
	now = ++sig;
	anc[now] = st;
	depth[cnt] = now;
	//计算数据
	node[st].data[0].par = st;
	node[st].data[1].par = par;
	i = 1;
	while(calc_cnt(st, i))
		++ i;
	//递归
	for(j=node[st].head;j != nullptr;j=j->next){
		if(j->b == par)
			continue;
		node[j->b].depth = node[st].depth + 1;
		DFS(j->b, st);
		depth[++cnt] = now;
	}
}

void rmqinit()
{
	int i, j, m = (int)floor(log(double(cnt)) / lg2);
	for(i=1;i<cnt;i++)
		rmqarr[i][0] = depth[i];
	for(j=1;j<=m;j++)
		for(i=1;i<=cnt-(1<<(j-1));i++)
			rmqarr[i][j] = std::min(rmqarr[i][j-1], rmqarr[i+(1<<(j-1))][j-1]);
}

int LCA(int a, int b)
{
	int m;
	a = node[a].pf;
	b = node[b].pf;
	if(a > b)
		std::swap(a, b);
	m = (int)floor(log(double(b - a + 1)) / lg2);
	return anc[std::min(rmqarr[a][m], rmqarr[b-(1<<m)+1][m])];
}

void sub_calc(int a, int rt, int arr[MAXA])
{
	int i, j, k;
	int tmp[MAXA];
	if(a == rt){
		for(i=0;i<MAXA;i++)
			arr[i] = 0;
	}else{
		k = (int)floor(log(double(node[a].depth - node[rt].depth + 1)) / lg2);
		for(i=0;i<MAXA;i++)
			arr[i] = node[a].data[k].cnt[i];
		a = node[a].data[k].par;
		while(a != rt){
			k = (int)floor(log(double(node[a].depth - node[rt].depth + 1)) / lg2);
			for(i=0;i<MAXA;i++){
				tmp[i] = MAXI;
				for(j=0;j<MAXA;j++){
					tmp[i] = std::min(tmp[i], arr[j] + node[a].data[k].cnt[i] + (i != j));
				}
			}
			for(i=0;i<MAXA;i++){
				arr[i] = tmp[i];
			}
			a = node[a].data[k].par;
		}
	}
}

int do_calc(int a, int b)
{
	int ca[MAXA], cb[MAXA], ans, i, j;
	int lca = LCA(a, b);
	sub_calc(a, lca, ca);
	sub_calc(b, lca, cb);
	ans = MAXI;
	for(i=0;i<MAXA;i++){
		for(j=0;j<MAXA;j++){
			ans = std::min(ans, ca[i] + cb[j] + (i != j));
		}
	}
	return ans;
}

void addedge(int a, int b)
{
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = node[a].head;
	node[a].head = edge + ecnt;
	ecnt++;
	edge[ecnt].a = b;
	edge[ecnt].b = a;
	edge[ecnt].next = node[b].head;
	node[b].head = edge + ecnt;
	ecnt++;
}

int main()
{
	int i, j, a, b, m;
	scanf("%d",&N);
	for(i=1;i<=N;i++){
		node[i].data[0].par = i;
		for(j=0;j<MAXA;j++){
			scanf("%d", &a);
			if(a > 0)
				node[i].data[0].cnt[j] = 0;
			else
				node[i].data[0].cnt[j] = MAXI;
		}
	}
	for(i=1;i<N;i++){
		scanf("%d%d",&a,&b);
		addedge(a, b);
	}
	DFS(1, -1);
	rmqinit();
	scanf("%d",&m);
	for(i=1;i<=m;i++){
		scanf("%d%d",&a,&b);
		if(a == b){
			printf("0\n");
		}else{
			a = do_calc(a, b);
			if(a >= MAXI)
				a = -1;
			else
				a++;
			printf("%d\n", a);
		}
	}
	return 0;
}