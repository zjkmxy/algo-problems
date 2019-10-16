//������������ࡣ�о��������ߵĺ�
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXT = MAXN * 180;
struct NODE;
struct EDGE;
struct SN;
struct NODE{
	int col;
	int sz;
	EDGE* head;
	NODE* par;
	NODE* son;
	SN* rt; //�߶����ĸ�
}node[MAXN];
struct EDGE{
	NODE *a, *b;
	EDGE *next;
}edge[MAXN<<1];
int N, M, ecnt;
int colorcnt[MAXN], mxclcnt; //ȫ��������ĳ����ɫ�ĸ��������ֵ

//�ɳ־û��߶���
struct SN{
	int cnt;	//�±�Ϊ��ɫ�ڵ������cntΪ������
	SN *l, *r;
	static SN* alloc();
	inline void popup(){
		cnt = l->cnt + r->cnt;
	}
	SN* update(int pos, int val, int L, int R)const{
		SN* rt = alloc();
		*rt = *this;
		if(L == R){
			rt->cnt += val;
			rt->cnt = max(rt->cnt, 0);
		}else{
			int mid = (L + R) >> 1;
			if(pos <= mid)
				rt->l = rt->l->update(pos, val, L, mid);
			else
				rt->r = rt->r->update(pos, val, mid + 1, R);
			rt->popup();
		}
		return rt;
	}
	int query(int lp, int rp, int L, int R)const{
		if(lp <= L && R <= rp){
			return cnt;
		}else{
			int mid = (L + R) >> 1;
			if(rp <= mid){
				return l->query(lp, rp, L, mid);
			}else if(lp > mid){
				return r->query(lp, rp, mid + 1, R);
			}else{
				return l->query(lp, mid, L, mid) + r->query(mid + 1, rp, mid + 1, R);
			}
		}
	}
}pool[MAXT];
typedef SN *PSN;
SN* const nil = pool;
int sncnt = 1;

SN* SN::alloc(){
	SN* ret = &pool[sncnt ++];
	if(sncnt > MAXT)
		throw 0;
	ret->cnt = 0;
	ret->l = ret->r = nil;
	return ret;
}

//ȷ���غ���
void DFS(NODE* rt){
	EDGE* j;

	rt->sz = 1;
	for(j = rt->head; j; j = j->next){
		if(j->b == rt->par)
			continue;
		j->b->par = rt;
		DFS(j->b);
		rt->sz += j->b->sz;
		if(!rt->son || j->b->sz > rt->son->sz)
			rt->son = j->b;
	}
}

int cnt[MAXN];	//DFSʱÿ����ɫ�ĸ���
//���cnt�á���������������
void Clear(NODE* rt){
	EDGE *j;
	cnt[rt->col] --;
	for(j = rt->head; j; j = j->next){
		if(j->b != rt->par)
			Clear(j->b);
	}
}

//һ�߼���cntһ�������߶����ڴ����ȶ���ҵ�
//����ֻ��һ���Խ���ȫ���޸�
//fc���ǡ������ӵ�ĳ����ɫ�ĸ�����
int fc[MAXN];
void Add(NODE* rt){
	EDGE *j;
	fc[rt->col] ++;
	for(j = rt->head; j; j = j->next){
		if(j->b != rt->par)
			Add(j->b);
	}
}
void Deal(NODE* rt, PSN &segt){
	EDGE *j;
	if(fc[rt->col] > 0){
		segt = segt->update(cnt[rt->col], -1, 0, mxclcnt);
		cnt[rt->col] += fc[rt->col];
		segt = segt->update(cnt[rt->col], +1, 0, mxclcnt);
		fc[rt->col] = 0;
	}
	for(j = rt->head; j; j = j->next){
		if(j->b != rt->par)
			Deal(j->b, segt);
	}
}

//�����߶���
void DFS2(NODE* rt){
	EDGE *j;

	rt->rt = nil;
	for(j = rt->head; j; j = j->next){
		if(j->b != rt->par && j->b != rt->son){
			DFS2(j->b);
			Clear(j->b);
		}
	}
	if(rt->son){
		DFS2(rt->son);
		rt->rt = rt->son->rt;
	}
	fc[rt->col] ++;
	for(j = rt->head; j; j = j->next){
		if(j->b != rt->par && j->b != rt->son){
			Add(j->b);
		}
	}
	for(j = rt->head; j; j = j->next){
		if(j->b != rt->par && j->b != rt->son){
			Deal(j->b, rt->rt);
		}
	}
	if(fc[rt->col] > 0){
		rt->rt = rt->rt->update(cnt[rt->col], -1, 0, mxclcnt);
		cnt[rt->col] += fc[rt->col];
		rt->rt = rt->rt->update(cnt[rt->col], +1, 0, mxclcnt);
		fc[rt->col] = 0;
	}
}

void addedge(NODE* a, NODE* b){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = a->head;
	a->head = &edge[ecnt ++];
}

int main(){
	int i, a, b;
	scanf("%d%d", &N, &M);
	for(i = 1; i <= N; i ++){
		scanf("%d", &node[i].col);
		colorcnt[node[i].col] ++;
		mxclcnt = max(mxclcnt, colorcnt[node[i].col]);
	}
	mxclcnt ++;
	for(i = 1; i < N; i ++){
		scanf("%d%d", &a, &b);
		addedge(node + a, node + b);
		addedge(node + b, node + a);
	}
	nil->l = nil->r = nil;
	DFS(node + 1);
	DFS2(node + 1);
	while(M --){
		scanf("%d%d", &i, &b);
		if(b <= mxclcnt)
			a = node[i].rt->query(b, mxclcnt, 0, mxclcnt);
		else
			a = 0;
		printf("%d\n", a);
	}
	return 0;
}