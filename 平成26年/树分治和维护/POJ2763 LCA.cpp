#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

#define MAXN 200005
#define LOGMN 30
#define lowbit(x) (x&(-x))

inline int log2(int x){
	int ret = 0;
	for(; x > 1; x >>= 1, ++ ret);
	return ret;
}

struct NODE;
struct EDGE;
typedef struct NODE
{
	NODE *par[LOGMN];
	EDGE *head;
	int pfst, plast; //首次和最后的出现
	int dis;
	size_t size;
} *PNODE;
typedef struct EDGE{
	NODE *st, *ed;
	EDGE *next;
	int w;
	bool out;
} *PEDGE;

class TA{
private:
	int pool[MAXN<<1], n;
public:
	void Init(int n){
		memset(pool, 0, sizeof(pool));
		this->n = n;
	}
	int GetSum(int pos){
		int sum = 0;
		for(; pos > 0; pos -= lowbit(pos))
			sum += pool[pos];
		return sum;
	}
	void Update(int pos, int val){
		for(; pos <= n; pos += lowbit(pos))
			pool[pos] += val;
	}
};

class TREE{
private:
	NODE node[MAXN];
	EDGE edge[MAXN<<1];
	PNODE anc[MAXN<<1];	//指定（伪）深度对应的节点
	int rmqarr[MAXN<<1][LOGMN];//深度序列+RMQ
	size_t ncnt, ecnt;//点边数
	size_t cnt, sig;//LCA用
	TA ta;//树状数组
	void LCADFS(PNODE cur){
		PEDGE e;
		int now;
		cur->size = 1;
		cur->pfst = ++ cnt;
		now = ++ sig;
		anc[now] = cur;
		rmqarr[cnt][0] = now;
		for(e = cur->head; e; e = e->next){
			if(e->ed != cur->par[0]){
				e->ed->par[0] = cur;
				e->ed->dis = cur->dis + e->w;
				e->out = true;
				LCADFS(e->ed);
				//顺便求出子树尺寸
				cur->size += e->ed->size;
				rmqarr[++ cnt][0] = now;
			}
		}
		cur->plast = cnt;
	}
public:
	//最近公共祖先
	PNODE LCA(PNODE a, PNODE b){
		int aa = a->pfst, bb = b->pfst, m;
		if(aa > bb)
			swap(aa, bb);
		m = log2(bb - aa + 1);
		return anc[min(rmqarr[aa][m], rmqarr[bb - (1<<m) + 1][m])];
	}
	//n步的父亲
	PNODE GetPar(PNODE cur, int n){
		for(int i = 0; n > 0 && cur; i ++, n >>= 1){
			if(n & 1)
				cur = cur->par[i];
		}
		return cur;
	}
	//预处理
	void PreLCA(){
		int j, m;
		size_t i;
		cnt = sig = 0;
		node[1].par[0] = nullptr;
		LCADFS(&node[1]);
		m = log2(cnt);
		for(j = 1; j <= m; j ++){
			for(i = 1;i <= cnt - (1<<(j-1)); i ++){
				rmqarr[i][j] = min(rmqarr[i][j-1], rmqarr[i + (1<<(j-1))][j-1]);
			}
			for(i = 1; i <= ncnt; i++){
				if(node[i].par[j - 1])
					node[i].par[j] = node[i].par[j - 1]->par[j - 1];
			}
		}
		ta.Init(cnt);
	}
	//初始化
	void Init(int n){
		int i;
		ncnt = n;
		ecnt = 0;
		for(i = 1; i <= n; i ++){
			node[i].head = nullptr;
		}
	}
	//加边
	void __addedge(PNODE x, PNODE y, int w){
		edge[ecnt].st = x;
		edge[ecnt].ed = y;
		edge[ecnt].next = x->head;
		x->head = edge + ecnt;
		edge[ecnt].w = w;
		edge[ecnt].out = false;
		ecnt ++;
	}
	void AddEdge(int a, int b, int w){
		__addedge(node + a, node + b, w);
		__addedge(node + b, node + a, w);
	}
	//查询距离
	int __query(PNODE x){
		return x->dis + ta.GetSum(x->pfst);
	}
	int Query(int a, int b){
		PNODE pa = node + a, pb = node + b;
		if(a == b)
			return 0;
		return __query(pa) + __query(pb) - (__query(LCA(pa, pb)) << 1);
	}
	//更新距离
	void Update(int no, int w){
		PEDGE e = edge + (no << 1);
		w = w - e->w;
		e->w += w;
		(e + 1)->w += w;
		if(!e->out)
			e ++;
		ta.Update(e->ed->pfst, w);
		ta.Update(e->ed->plast + 1, - w);
	}
} tree;

int n, q, s;
int main(){
	int i, a, b, w;

	scanf("%d%d%d", &n, &q, &s);
	tree.Init(n);
	for(i = 1; i < n; i ++){
		scanf("%d%d%d", &a, &b, &w);
		tree.AddEdge(a, b, w);
	}
	tree.PreLCA();
	for(i = 0; i < q; i ++){
		scanf("%d%d", &a, &b);
		if(a == 0){
			cout << tree.Query(s, b) << endl;
			s = b;
		}else{
			b --;
			scanf("%d", &w);
			tree.Update(b, w);
		}
	}

	return 0;
}

