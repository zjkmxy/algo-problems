#include<cstdio>
#include<cctype>
#include<algorithm>
using namespace std;
//这题交G++不用扩栈也能递归……

const int MAXN = 100005;
struct NODE;
struct EDGE{
	NODE *a, *b;
	EDGE *next;
	int u;	//答案
	bool bridge;	//是不是桥
}edge[MAXN << 1];
struct NODE{
	EDGE *head;
	NODE *par;
	int DFN, low;
	int mv;	//最小值
}node[MAXN];
int ecnt, dep, n, m;

void Tarjan(NODE *x){
	x->DFN = x->low = ++ dep;
	for(EDGE *j = x->head; j; j = j->next){
		NODE *p = j->b;
		if(p == x->par)
			continue;
		if(!p->DFN){
			p->par = x;
			Tarjan(p);
			x->low = min(x->low, p->low);
			j->bridge = p->low > x->DFN;
		}else{
			x->low = min(x->low, p->DFN);
		}

		x->mv = max(x->mv, p->mv);
		j->u = j->b->mv;
	}
}

void addedge(NODE *a, NODE *b, int& ecnt){
	EDGE *j = &edge[ecnt ++];
	j->a = a;
	j->b = b;
	j->next = a->head;
	a->head = j;
	j->bridge = false;
}

inline char getc(){
	static const int BUFLEN = 1 << 15;
	static char B[BUFLEN], *S = B, *T = B;
	if(S == T){
		S = B;
		T = S + fread(B, 1, BUFLEN, stdin);
	}
	return (S == T) ? 0 : *(S ++);
}
int ReadInt(){
	char ch;
	do ch = getc(); while(!isdigit(ch));
	int aa = ch - '0';
	for(ch = getc(); isdigit(ch); ch = getc())
		aa = aa * 10 + ch - '0';
	return aa;
}

void input(){
	int i, a, b;

	n = ReadInt(), m = ReadInt();
	ecnt = 0;
	for(i = 0; i <= n; i ++){
		node[i].mv = i;
		node[i].par = nullptr;
		node[i].DFN = 0;
		node[i].head = nullptr;
	}
	for(i = 0; i < m; i ++){
		a = ReadInt(), b = ReadInt();
		addedge(node + a, node + b, ecnt);
		addedge(node + b, node + a, ecnt);
	}
}

void domain(){
	int u;
	input();
	dep = 0;
	Tarjan(node + n);	//N很烦对不对？好，我们选择N做根节点DFS……
	for(int i = 0; i < (m << 1); ){
		u = 0;
		if(edge[i].bridge)
			u = edge[i].u;
		i ++;
		if(edge[i].bridge)
			u = edge[i].u;
		i ++;
		printf("%d %d\n", u, u ? u + 1 : 0);
	}
}

int main(){
	int T = ReadInt();
	while(T --)
		domain();
	return 0;
}