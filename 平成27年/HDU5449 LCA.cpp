#include <cstdio>
#include <algorithm>
#include <cctype>
using namespace std;

const int MAXN = 50005;
typedef long long ll;
struct EDGE;
struct NODE{
	NODE *par;
	EDGE *head;
	int pfst;
	ll siz;
	ll up, down;
}node[MAXN];
typedef NODE *PNODE;
struct EDGE{
	NODE *b;
	EDGE *next;
}edge[MAXN << 1];
int N, ecnt;
PNODE anc[MAXN << 1];
int rmqarr[MAXN << 1][17], lb[MAXN << 1];
int sig, cnt;

void DFS(NODE* x){
	EDGE *j;
	int now = ++ sig;

	x->siz = 1;
	x->pfst = ++ cnt;
	anc[now] = x;
	rmqarr[cnt][0] = now;
	for(j = x->head; j; j = j->next){
		if(j->b == x->par)
			continue;
		j->b->par = x;
		DFS(j->b);
		x->siz += j->b->siz;
		rmqarr[++ cnt][0] = now;
	}
	//自己的
	x->up = 2 * x->siz - 1;
	x->down = 2 * N - 2 * x->siz - 1;
}

//加成前缀和
void DFS2(PNODE x){
	EDGE *j;
	if(x->par){
		x->up += x->par->up;
		x->down += x->par->down;
	}
	for(j = x->head; j; j = j->next){
		if(j->b == x->par)
			continue;
		DFS2(j->b);
	}
}

inline void PreRmq(){
	int i, j, m = lb[(cnt)];
	for(j = 1; j <= m; j ++){
		for(i = 1; i <= cnt - (1 << (j - 1)); i ++){
			rmqarr[i][j] = min(rmqarr[i][j - 1], rmqarr[i + (1 << (j - 1))][j - 1]);
		}
	}
}

inline PNODE LCA(PNODE a, PNODE b){
	int aa = a->pfst, bb = b->pfst, m;
	if(aa > bb)
		swap(aa, bb);
	m = lb[(bb - aa + 1)];
	return anc[min(rmqarr[aa][m], rmqarr[bb - (1 << m) + 1][m])];
}

inline void addedge(PNODE a, PNODE b){
	EDGE *j = &edge[ecnt ++];
	j->b = b;
	j->next = a->head;
	a->head = j;
}

char B[1 << 15], *S = B, *T = B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
	while(ch = getc(), !isdigit(ch) && ch);
	if(!ch) return 0;
	int aa = ch - '0';
	while(ch = getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
	return aa;
}

void domain(){
	int i, a, b, Q, P;
	ll ans;
	PNODE lca;

	N = ReadInt();
	ecnt = 0;
	for(i = 0; i <= N; i ++){
		node[i].head = nullptr;
		node[i].par = nullptr;
	}
	for(i = 1; i < N; i ++){
		a = ReadInt(), b = ReadInt();
		addedge(node + a, node + b);
		addedge(node + b, node + a);
	}

	sig = cnt = 0;
	DFS(node + 0);
	DFS2(node + 0);
	PreRmq();

	Q = ReadInt();
	while(Q --){
		P = ReadInt(), a = ReadInt();
		ans = 0;
		for(i = 1; i <= P; i ++, a = b){
			b = ReadInt();
			lca = LCA(node + a, node + b);
			ans += node[a].up - lca->up + node[b].down - lca->down;
		}
		printf("%lld.0000\n", ans);
	}
}

int main(){
	int i, T = ReadInt();
	lb[0] = lb[1] = 0;
	for(i = 2; i < MAXN * 2; i ++)
		lb[i] = lb[i >> 1] + 1;
	while(T --){
		domain();
		if(T > 0)
			printf("\n");
	}
	return 0;
}