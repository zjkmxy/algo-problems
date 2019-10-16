/*
用set维护安装的软件包这一联通块所有的叶子的dfn
插入的时候因为直插入一个叶子，和前驱后继做LCA即可得知插入长度
删除的时候暴力即可，均摊和插入相同
结果是O(NlogN)的
*/
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;

inline int log2(int x){
	int ret = 0;
	for(; x > 1; x >>= 1, ++ ret);
	return ret;
}

const int MAXN = 100005;
struct EDGE;
struct NODE{
	int id;
	NODE *par;
	EDGE *head;
	int pfst, plast;
	int depth;
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXN];
NODE *anc[MAXN<<1];
int rmqarr[MAXN<<1][30], cnt;
int N, ecnt, Q;
set<int> leaf;

void DFS(NODE *cur){
	EDGE *j;
	cur->pfst = ++ cnt;
	anc[cnt] = cur;
	rmqarr[cnt][0] = cur->pfst;
	for(j = cur->head; j; j = j->next){
		j->b->depth = cur->depth + 1;
		j->b->par = cur;
		DFS(j->b);
		rmqarr[++ cnt][0] = cur->pfst;
		anc[cnt] = cur;
	}
	cur->plast = cnt;
}

NODE* LCA(NODE* a, NODE* b){
	int aa = a->pfst, bb = b->pfst, m;
	if(aa > bb)
		swap(aa, bb);
	m = log2(bb - aa + 1);
	return anc[min(rmqarr[aa][m], rmqarr[bb - (1<<m) + 1][m])];
}

void PreLCA(){
	int m, i, j;
	DFS(node);
	m = log2(cnt);
	for(j = 1; j <= m; j ++){
		for(i = 1;i <= cnt - (1<<(j-1)); i ++){
			rmqarr[i][j] = min(rmqarr[i][j-1], rmqarr[i + (1<<(j-1))][j-1]);
		}
	}
}

void addedge(NODE* a, NODE* b){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = a->head;
	a->head = &edge[ecnt ++];
}

int install(int p){
	int ans = 0, tmp = 0x3fffffff;

	p = node[p].pfst;
	//p是叶子
	if(leaf.find(p) != leaf.end())
		return 0;
	//树是空的
	if(leaf.size() == 0){
		leaf.insert(node[0].pfst);
		ans ++;
	}
	auto it = leaf.lower_bound(p);
	if(it != leaf.end()){
		auto lca = LCA(anc[*it], anc[p]);
		if(lca->pfst == p)
			return ans;
		tmp = min(tmp, anc[p]->depth - lca->depth);
	}
	if(it != leaf.begin()){
		-- it;
		auto lca = LCA(anc[*it], anc[p]);
		if(lca->pfst == *it){ //根本身存在
			leaf.erase(it);
		}
		tmp = min(tmp, anc[p]->depth - lca->depth);
	}
	leaf.insert(p);
	return ans + tmp;
}

int uninstall(int p){
	int ans = 0, tmp;
	bool flg = false;
	NODE *cur;

	p = node[p].pfst;
	while(true){
		auto it = leaf.lower_bound(p);
		if(it == leaf.end() || *it > anc[p]->plast)
			break;
		flg = true;
		cur = anc[*it];
		tmp = cur->depth - anc[p]->depth;
		auto it2 = it;
		++ it2;
		if(it2 != leaf.end() && *it2 <= anc[p]->plast){
			tmp = min(tmp, cur->depth - LCA(anc[*it2], cur)->depth);
		}
		ans += tmp;
		leaf.erase(cur->pfst);
	}
	//flg表示p是否安装
	if(flg){
		ans ++;	//p节点本身
		if(anc[p]->par){
			auto it = leaf.lower_bound(anc[p]->par->pfst);
			if(it == leaf.end() || *it > anc[p]->par->plast)
				leaf.insert(anc[p]->par->pfst);
		}
	}
	return ans;
}

int main(){
	int a, i;
	char str[20];
	ecnt = 0;
	scanf("%d", &N);
	for(i = 1; i < N; i ++){
		scanf("%d", &a);
		node[i].id = i;
		addedge(node + a, node + i);
	}
	PreLCA();
	scanf("%d", &Q);
	while(Q --){
		scanf("%s%d", str, &a);
		if(str[0] == 'i'){
			printf("%d\n", install(a));
		}else{
			printf("%d\n", uninstall(a));
		}
	}
	return 0;
}