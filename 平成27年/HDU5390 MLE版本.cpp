//主席树的变形
//在线做法空间严重不足，没办法了
#pragma comment(linker, "/STACK:1024000000,1024000000")
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

inline int lowbit(int x){
	return x & (- x);
}

const int MAXN = 100002;
const int MAXN2 = 280 * MAXN;
const int LOG = 30;
const int INF = (1 << LOG) - 1;

//线段树节点
struct SN{
	SN *chd[2];
	int n;
	int ref;
}sn[MAXN2];
typedef SN* PSN;
int scnt;	//线段树节点的使用个数
PSN seri[MAXN];	//顺序存储的普通可持久化线段树
PSN cmt[MAXN];	//主席树
PSN recyc[200];
int ncyc;

inline void InitSn(){
	scnt = 1;
	sn->n = 0;
	sn->chd[0] = sn->chd[1] = sn;	//sn是哨兵
	sn->ref = 3;
	ncyc = 0;
}
inline void doref(PSN& var, PSN val){
	val->ref ++;
	var->ref --;
	if(var->ref == 0){
		doref(var->chd[0], sn);
		doref(var->chd[1], sn);
		recyc[ncyc ++] = var;
	}
	var = val;
}

inline PSN BuildSn(PSN x = sn){
	PSN ret;
	if(ncyc == 0){
		ret = &sn[scnt ++];
	}else{
		ret = recyc[-- ncyc];
	}
	*ret = *x;
	ret->chd[0]->ref ++;
	ret->chd[1]->ref ++;
	ret->ref = 0;
	return ret;
}

PSN UpdateSn(PSN rt, int val, int cnt){
	PSN ret = BuildSn(rt), tmp;
	int m, c;

	rt = ret;
	rt->n += cnt;
	for(m = LOG - 1; m >= 0; m --){
		c = (val >> m) & 1;
		doref(rt->chd[c], BuildSn(rt->chd[c]));
		rt = rt->chd[c];
		rt->n += cnt;
	}
	return ret;
}

int QuerySn(int pos, int val){
	PSN stk[100];
	int sp = 1, i, ans = 0;
	int m, c, cnt;

	stk[0] = seri[pos];
	for(; pos > 0; pos -= lowbit(pos))
		stk[sp ++] = cmt[pos];
	for(m = LOG - 1; m >= 0; m --){
		c = (val >> m) & 1;
		for(i = 0, cnt = 0; i < sp; i ++)
			cnt += stk[i]->chd[c ^ 1]->n;
		if(cnt > 0)
			c ^= 1;
		ans |= c << m;
		for(i = 0, cnt = 0; i < sp; i ++)
			stk[i] = stk[i]->chd[c];
	}
	return ans ^ val;
}

struct EDGE;
struct NODE{
	int val;
	int pfst, plast;
	EDGE *head;
}node[MAXN];
struct EDGE{
	NODE *b;
	EDGE *next;
}edge[MAXN];
int N, dfn, M;
pair<int, int> opt[MAXN << 1];

void DFS(NODE* cur){
	EDGE* j;
	cur->pfst = ++ dfn;
	for(j = cur->head; j; j = j->next)
		DFS(j->b);
	cur->plast = dfn + 1;
}

void Init(){
	int i, ops = 0, j;

	dfn = 0;
	seri[0] = sn;
	sn->ref ++;
	DFS(node + 1);
	ops = 0;
	for(i = 1; i <= N; i ++){
		cmt[i] = sn;
		sn->ref ++;
		opt[ops ++] = make_pair(node[i].pfst, 1 + node[i].val);
		opt[ops ++] = make_pair(node[i].plast, -1 - node[i].val);
	}
	sort(opt, opt + ops);
	for(i = 0, j = 0; i < ops; i ++){
		while(opt[i].first > j){
			seri[j + 1] = seri[j];
			j ++;
			seri[j]->ref ++;
		}
		if(opt[i].second > 0){
			doref(seri[j], UpdateSn(seri[j], opt[i].second - 1, 1));
		}else{
			doref(seri[j], UpdateSn(seri[j], - opt[i].second - 1, - 1));
		}
	}
}

void Change(NODE* cur, int val){
	int p;

	if(val == cur->val)
		return ;
	for(p = cur->pfst; p <= N; p += lowbit(p)){
		doref(cmt[p], UpdateSn(cmt[p], cur->val, -1));
		doref(cmt[p], UpdateSn(cmt[p], val, 1));
	}
	for(p = cur->plast; p <= N; p += lowbit(p)){
		doref(cmt[p], UpdateSn(cmt[p], cur->val, 1));
		doref(cmt[p], UpdateSn(cmt[p], val, -1));
	}
	cur->val = val;
}

inline int Query(NODE* cur){
	return QuerySn(cur->pfst, cur->val);
}

void domain(){
	int i, p, op, v;

	InitSn();
	N = ReadInt(), M = ReadInt();
	for(i = 0; i <= N; i ++){
		node[i].head = nullptr;
	}
	for(i = 0; i <= N - 2; i ++){
		p = ReadInt();
		edge[i].b = node + i + 2;
		edge[i].next = node[p].head;
		node[p].head = edge + i;
	}
	for(i = 1; i <= N; i ++)
		node[i].val = ReadInt();
	Init();
	while(M --){
		op = ReadInt(), p = ReadInt();
		if(op){
			v = Query(node + p);
			printf("%d\n", v);
		}else{
			v = ReadInt();
			Change(node + p, v);
		}
	}
}

int main(){
	int T = ReadInt();
	while(T --)
		domain();
	return 0;
}
