#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

#define MAXN 100005
#define LOGMN 30
#define lowbit(x) (x&(-x))

inline int log2(int x){
	int ret = 0;
	while(x > 1){
		x >>= 1;
		ret ++;
	}
	return ret;
}

struct NODE;
struct EDGE;
typedef NODE *PNODE;
typedef EDGE *PEDGE;
struct NODE
{
	PNODE par[LOGMN];
	PEDGE head;
	int pfst;	//首次出现
	int dis;
	size_t size;
};
struct EDGE{
	PNODE st, ed;
	PEDGE next;
};

class TREE{
private:
	NODE node[MAXN];
	EDGE edge[MAXN<<1];
	PNODE anc[MAXN<<1];	//指定（伪）深度对应的节点
	int rmqarr[MAXN<<1][LOGMN];
	size_t ncnt, ecnt;
	size_t cnt, sig;

	void DFS(PNODE cur){
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
				e->ed->dis = cur->dis + 1;
				DFS(e->ed);
				//顺便求出子树尺寸
				cur->size += e->ed->size;
				rmqarr[++ cnt][0] = now;
			}
		}
	}

public:
	PNODE LCA(PNODE a, PNODE b){
		int aa = a->pfst, bb = b->pfst, m;
		if(aa > bb)
			swap(aa, bb);
		m = log2(bb - aa + 1);
		return anc[min(rmqarr[aa][m], rmqarr[bb - (1<<m) + 1][m])];
	}

	PNODE GetPar(PNODE cur, int n){
		for(int i = 0; n > 0 && cur; i ++, n >>= 1){
			if(n & 1)
				cur = cur->par[i];
		}
		return cur;
	}

	void PreLCA(){
		int j, m;
		size_t i;
		cnt = sig = 0;
		node[1].par[0] = nullptr;
		DFS(&node[1]);
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
	}

	void Init(int n){
		int i;
		ncnt = n;
		ecnt = 0;
		for(i = 1; i <= n; i ++){
			node[i].head = nullptr;
		}
	}
	
	void AddEdge(int a, int b){
		auto func = [this](PNODE x, PNODE y)->void{
			edge[ecnt].st = x;
			edge[ecnt].ed = y;
			edge[ecnt].next = x->head;
			x->head = edge + ecnt;
			ecnt ++;
		};
		func(node + a, node + b);
		func(node + b, node + a);
	}

	size_t Query(int a, int b){
		PNODE lca = LCA(node + a, node + b);
		int dis = node[a].dis + node[b].dis - (lca->dis << 1);
		size_t ret = 0;

		if(a == b){
			return node[1].size;
		}
		if(node[a].dis < node[b].dis)
			swap(a, b);
		dis = (dis >> 1) - 1;
		if(node[a].dis == node[b].dis){
			//LCA是中点
			ret  = node[1].size;
			ret -= GetPar(node + a, dis)->size;
			ret -= GetPar(node + b, dis)->size;
		}else if(((node[a].dis - node[b].dis) & 1) == 0){
			//差为偶数步
			ret  = GetPar(node + a, dis + 1)->size;
			ret -= GetPar(node + a, dis)->size;
		}
		return ret;
	}
};
TREE tree;

int n, m;
int main(){
	int i, a, b;

	scanf("%d", &n);
	tree.Init(n);
	for(i = 1; i < n; i ++){
		scanf("%d%d", &a, &b);
		tree.AddEdge(a, b);
	}
	tree.PreLCA();
	scanf("%d", &m);
	for(i = 0; i < m; i ++){
		scanf("%d%d", &a, &b);
		cout << tree.Query(a, b) << endl;
	}

	return 0;
}
