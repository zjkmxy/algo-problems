#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

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

const int MAXN = 100005;
struct NODE;
struct EDGE;
struct NODE{
	EDGE *head, *pre;
	int dis;
}node[MAXN];
struct EDGE{
	int w;
	NODE *a, *b;
	EDGE *next;
}edge[MAXN];
int N, M;

void solve(){
	int l = 2, r = N, cnt = 0;
	EDGE *j;
	NODE *p;

	node[1].dis = 0;
	for(j = node[1].head; j; j = j->next)
		j->b->pre = j;
	while(l <= r){
		if(node[l].pre)
			p = &node[l ++];
		else
			p = &node[r --];
		p->dis = ++ cnt;
		p->pre->w = cnt - p->pre->a->dis;
		for(j = p->head; j; j = j->next)
			j->b->pre = j;
	}
}

void domain(){
	int i;

	N = ReadInt(), M = ReadInt();
	for(i = 1; i <= N; i ++){
		node[i].pre = node[i].head = nullptr;
	}
	for(i = 1; i <= M; i ++){
		edge[i].a = node + ReadInt();
		edge[i].b = node + ReadInt();
		edge[i].w = N;
		edge[i].next = edge[i].a->head;
		edge[i].a->head = edge + i;
	}

	solve();
	for(i = 1; i <= M; i ++){
		printf("%d\n", edge[i].w);
	}
}

int main(){
	int T = ReadInt();
	while(T --)
		domain();
	return 0;
}
