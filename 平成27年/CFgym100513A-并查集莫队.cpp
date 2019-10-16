//���鼯��ֻ�ܻ��˲��ܼ��ģ�����Ī�ӵ�ʱ��Ҫ���⴦��
//1.���ڵĲ�ѯ����������
//2.����ѯ���ұ߽������ұ߽翪ʼֻ�Ӳ�����ÿ�ε�һ����ѯ��ʱ���ȰѲ�ѯ�����ڵĲ��ּ��ϣ�Ȼ���˵���ÿ����ѯ������Ĳ����ǵ����ġ�
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAXN = 100005;
class UnionSet{
public:
	//�ڵ�
	typedef struct NODE{
		NODE *par;
		int rank;		//��
		bool zusammen;	//�͸����Ƿ�һ��
	}*PNODE;
	NODE node[MAXN];
	//��¼��ʷ������ջ
	struct OPERAT{
		PNODE sub, par; //���ӣ�����
		bool addrk;     //���׵����Ƿ�����
		OPERAT(){};
		OPERAT(PNODE sub, PNODE par, bool addrk):sub(sub),par(par),addrk(addrk){}
	}stk[MAXN];
	int top;
	//��ʼ��
	void Init(int N){
		for(int i = 0; i <= N; i ++){
			node[i].par = &node[i];
			node[i].rank = 0;
			node[i].zusammen = true;
		}
		top = 0;
	}
	//���a�ڲ��鼯��ĸ����Լ�����Ƿ�Ϊ����(_In_out_)
	inline PNODE GetRoot(PNODE a, bool &Odd){
		for(; a->par != a; Odd = (Odd == a->zusammen), a = a->par);
		return a;
	}
	//�ϲ�a��b�����ش˴β����Ƿ�ȫ
	bool Union(PNODE a, PNODE b){
		bool ret = true;
		a = GetRoot(a, ret);
		b = GetRoot(b, ret);
		if(a != b){
			bool addrk = false;
			if(a->rank > b->rank){
				swap(a, b);
			}else if(a->rank == b->rank){
				b->rank ++;
				addrk = true;
			}
			stk[top ++] = OPERAT(a, b, addrk);
			a->par = b;
			a->zusammen = !ret;
			return true;
		}else
			return !ret;
	}
	//Rollbackֱ��base��
	void Regret(int base){
		while(top > base){
			top --;
			stk[top].sub->par = stk[top].sub;
			stk[top].sub->zusammen = true;
			if(stk[top].addrk)
				stk[top].par->rank --;
		}
	}
}us;

struct EDGE{
	int a, b;
}edge[MAXN];
struct QUERY{
	int id, grp;
	int l, r;
	bool operator<(const QUERY& rhs)const{
		return grp < rhs.grp || (grp == rhs.grp && r < rhs.r);
	}
}query[MAXN];
int qcnt, grpsz;
//��Ϊ��Щ���ӵ�Query���棬����Answer����
bool ans[MAXN];
int N, M, Q;

//�������
bool BF(int l, int r){
	int i;
	bool ret = true;
	us.Init(N);
	for(i = l; i <= r; i ++){
		ret = ret && us.Union(us.node + edge[i].a, us.node + edge[i].b);
	}
	return ret;
}

//�������
void GroupSolve(){
	int grp, i, r, l, base;
	bool ret;
	for(grp = 0; grp < qcnt; grp = i){
		us.Init(N);
		r = grpsz * (query[grp].grp + 1);
		ret = true;
		for(i = grp; i < qcnt && query[i].grp == query[grp].grp; i ++){
			while(r < query[i].r){
				++ r;
				ret = ret && us.Union(us.node + edge[r].a, us.node + edge[r].b);
			}
			if(!ret){
				ans[query[i].id] = false;
			}else{
				base = us.top;
				for(l = grpsz * (query[i].grp + 1); l >= query[i].l; l --){
					if(!us.Union(us.node + edge[l].a, us.node + edge[l].b))
						break;
				}
				ans[query[i].id] = l < query[i].l;
				us.Regret(base);
			}
		}
	}
}

int main(){
	int i;
	scanf("%d%d%d", &N, &M, &Q);
	for(i = 1; i <= M; i ++){
		scanf("%d%d", &edge[i].a, &edge[i].b);
	}
	grpsz = floor(sqrt(M));
	qcnt = 0;
	for(i = 1; i <= Q; i ++){
		scanf("%d%d", &query[qcnt].l, &query[qcnt].r);
		query[qcnt].grp = query[qcnt].l / grpsz;
		if(query[qcnt].r - query[qcnt].grp * grpsz <= grpsz){
			ans[i] = BF(query[qcnt].l, query[qcnt].r);
		}else{
			query[qcnt].id = i;
			qcnt ++;
		}
	}
	sort(query, query + qcnt);
	GroupSolve();
	for(i = 1; i <= Q; i ++){
		if(ans[i]){
			puts("Possible");
		}else{
			puts("Impossible");
		}
	}
	return 0;
}
