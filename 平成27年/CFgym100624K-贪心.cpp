#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>

const int MAXN = 4005;
const int MAXM = 12005;
struct NODE;
struct EDGE;
typedef NODE *PNODE;
typedef EDGE *PEDGE;
enum STATE{UNKNOWN, USED, DISABLED};
struct NODE{
	char name[15];
	PEDGE head;
	int deg;
	int use;
}node[MAXN];
struct EDGE{
	PNODE a, b;
	PEDGE next;
	PEDGE pair;
	STATE st;
}edge[MAXM];
int n, m, k, ecnt;
std::queue<PNODE> oplst;

inline PNODE getByName(char *name){
	int ret = atoi(name + 2);
	if(name[0] == 'B'){
		ret += k + n;
	}
	if(name[1] == 'P'){
		ret += k;
	}
	return node + ret;
}

PEDGE doAddEdge(PNODE a, PNODE b){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = a->head;
	a->head = edge + ecnt;
	a->deg ++;
	return &edge[ecnt ++];
}

inline void addEdge(PNODE a, PNODE b, STATE st){
	PEDGE p, q;
	p = doAddEdge(a, b);
	q = doAddEdge(b, a);
	p->pair = q;
	q->pair = p;
	p->st = q->st = st;
	if(st == USED){
		a->use ++;
		b->use ++;
	}
}

void init(){
	int i;
	ecnt = 0;
	for(i = 1; i <= k; i ++){
		node[i].deg = 0;
		node[i].use = 0;
		node[i].head = nullptr;
		sprintf(node[i].name, "AS%d", i);
	}
	for(i = k + 1; i <= k + n; i ++){
		node[i].deg = 0;
		node[i].use = 0;
		node[i].head = nullptr;
		sprintf(node[i].name, "AP%d", i - k);
	}
	for(i = k + n + 1; i <= k + n + k; i ++){
		node[i].deg = 0;
		node[i].use = 0;
		node[i].head = nullptr;
		sprintf(node[i].name, "BS%d", i - k - n);
	}
	for(i = k + n + k + 1; i <= k + n + k + m; i ++){
		node[i].deg = 0;
		node[i].use = 0;
		node[i].head = nullptr;
		sprintf(node[i].name, "BP%d", i - k - k - n);
	}
	while(!oplst.empty()){
		oplst.pop();
	}
}

void input(){
	int i;
	char tmp1[12], tmp2[12];

	scanf("%d%d%d", &k, &n, &m);
	init();
	for(i = 0; i < n + k - 1; i ++){
		scanf("%s%s", tmp1, tmp2);
		addEdge(getByName(tmp1), getByName(tmp2), UNKNOWN);
	}
	for(i = 0; i < m + k - 1; i ++){
		scanf("%s%s", tmp1, tmp2);
		addEdge(getByName(tmp1), getByName(tmp2), UNKNOWN);
	}
	for(i = 0; i < k; i ++){
		scanf("%s%s", tmp1, tmp2);
		addEdge(getByName(tmp1), getByName(tmp2), USED);
	}
	for(i = 1; i <= k + k + n + m; i ++){
		if(node[i].deg == 2){
			oplst.push(node + i);
		}
	}
}

bool deal(){
	PNODE cur, ed;
	PEDGE e, e2;
	int cnt;

	while(!oplst.empty()){
		cur = oplst.front();
		oplst.pop();
		for(e = cur->head; e; e = e->next){
			if(e->st == UNKNOWN){
				e->st = e->pair->st = USED;
				cur->use ++;
				ed = e->b;
				ed->use ++;
				//节点完全占用
				if(ed->use == 2){
					for(e2 = ed->head; e2; e2 = e2->next){
						if(e2->st == UNKNOWN){
							e2->st = e2->pair->st = DISABLED;
							ed->deg --;
							e2->b->deg --;
							if(e2->b->deg <= 2){
								oplst.push(e2->b);
							}
						}//unknown
					}//e2
				}//use
			}//unknown
		}
		if(cur->use != 2){
			return false;
		}
	}

	for(cur = node + 1; cur <= node + n + m + k + k; cur ++){
		if(cur->use != 2){
			return false;
		}
	}

	cur = node + 1;
	ed = nullptr;
	cnt = 0;
	do{
		cnt ++;
		for(e = cur->head; e; e = e->next){
			if(e->st == USED && e->b != ed){
				ed = cur;
				cur = e->b;
				break;
			}
		}
	}while(cur != node + 1);

	return (cnt == n + m + k + k);
}

void print(){
	PNODE cur = node + 1, par = nullptr;
	PEDGE e;
	do{
		printf(" %s", cur->name);
		for(e = cur->head; e; e = e->next){
			if(e->st == USED && e->b != par){
				par = cur;
				cur = e->b;
				break;
			}
		}
	}while(cur != node + 1);
}

int main(){
	int t;
	scanf("%d", &t);
	for(int i = 1; i <= t; i ++){
		input();
		if(deal()){
			printf("YES");
			print();
			printf("\n");
		}else{
			printf("NO\n");
		}
	}
	return 0;
}
