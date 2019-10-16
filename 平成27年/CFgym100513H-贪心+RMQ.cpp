//֮ǰ�����ԭ�����ڣ�ÿ�ζ�һ��������ֱ�ߣ�����ѡ�񿿽��󶥵����Сֵ����һ�ε��ʷֵ�
//����������6����  ->  1 2 3 5 4 3
//ֵ��ע����ǣ�Splay�ڷ�ת֮���޷���ȫ��ά����ֵ���ʱ��ʱ�������Һ��ӡ�������
//���ֻ��ʹ��RMQ�ݹ鷨
#include <cstdio>
#include <algorithm>
#include <set>
using namespace std;

const int MAXN = 500005;
const int LOGMN = 21;
const int INF = 0x3fffffff;
int N, arr[MAXN], rel;

//�����ر�ŵ�Ӱ�죬�ָ�ԭ���
inline int UnRel(int lab){
	lab += (rel - 1);
	return lab <= N ? lab : lab - N;
}
//�ر��
inline int Rel(int lab){
	lab -= rel - 1;
	return lab <= 0 ? lab + N : lab;
}

//��׼RMQ�ڴ汬ը���������߶���RMQ����
class RMQ{
	struct DATA{
		int lid, lval;	//������С
		int rid, rval;	//������С
		DATA *lchd, *rchd;
	}node[MAXN<<3];
	int N, ncnt;
	inline void popup(const DATA* l, const DATA* r, DATA* ret){
		if(l->lval <= r->lval){
			ret->lid = l->lid;
			ret->lval = l->lval;
		}else{
			ret->lid = r->lid;
			ret->lval = r->lval;
		}
		if(l->rval < r->rval){
			ret->rid = l->rid;
			ret->rval = l->rval;
		}else{
			ret->rid = r->rid;
			ret->rval = r->rval;
		}
	}
	DATA* build(int L, int R){
		DATA* ret = &node[ncnt ++];
		if(L == R){
			ret->lid = ret->rid = L;
			ret->lval = ret->rval = arr[UnRel(L)];
			ret->lchd = ret->rchd = nullptr;
		}else{
			int mid = (L + R) >> 1;
			ret->lchd = build(L, mid);
			ret->rchd = build(mid + 1, R);
			popup(ret->lchd, ret->rchd, ret);
		}
		return ret;
	}
	DATA query(DATA* rt, int l, int r, int L, int R){
		if(l == L && r == R){
			return *rt;
		}else{
			int mid = (L + R) >> 1;
			if(r <= mid){
				return query(rt->lchd, l, r, L, mid);
			}else if(l > mid){
				return query(rt->rchd, l, r, mid + 1, R);
			}else{
				DATA ld = query(rt->lchd, l, mid, L, mid);
				DATA rd = query(rt->rchd, mid + 1, r, mid + 1, R);
				DATA ret;
				popup(&ld, &rd, &ret);
				return ret;
			}
		}
	}
public:
	inline void Build(int N){
		this->N = N;
		ncnt = 0;
		build(1, N);
	}
	inline int Query(int l, int r, bool bLeft){
		if(l > r)
			return 0;
		DATA ret = query(node, l, r, 1, N);
		if(bLeft)
			return ret.lid;
		else
			return ret.rid;
	}
}rmq;

struct LINE{
	int a, b, c;	//��ԭ���
	bool operator<(const LINE& rhs)const{
		if(arr[a] != arr[rhs.a])
			return arr[a] < arr[rhs.a];
		else if(arr[b] != arr[rhs.b])
			return arr[b] < arr[rhs.b];
		else
			return arr[c] < arr[rhs.c];
	}
}lines[MAXN];
int nln;

//�������
void SortPrint(int a, int b, int c){
	if(arr[b] > arr[c])
		swap(b, c);
	if(arr[a] > arr[b])
		swap(a, b);
	if(arr[b] > arr[c])
		swap(b, c);
	lines[nln].a = a;
	lines[nln].b = b;
	lines[nln].c = c;
	nln ++;
}

//���м������ʷ֣�����Ϊ�±�ţ�
void NextTurn(int l, int r){
	int x;
	if(r <= l + 1)
		return ;
	x = rmq.Query(l + 1, r - 1, arr[UnRel(l)] > arr[UnRel(r)]);
	SortPrint(UnRel(l), UnRel(x), r > N ? rel : UnRel(r));
	NextTurn(l, x);
	NextTurn(x, r);
}

//ȷ���ر��λ�ã�Ȼ����е�һ���ʷ�
void FirstTurn(){
	int i, a = 0, b = 0, c = 0;
	int t1, t2;

	rel = 1;
	for(i = 2; i <= N; i ++){
		if(arr[i] < arr[rel])
			rel = i;
	}
	a = rel;
	for(i = 1; i <= N; i ++){
		if(i == rel)
			continue;
		if(b == 0 || arr[i] < arr[b])
			b = i;
	}
	rmq.Build(N + 1);
	t1 = rmq.Query(2, Rel(b) - 1, false);
	t2 = rmq.Query(Rel(b) + 1, N, true);
	if(t1 == 0 || (t2 != 0 && arr[UnRel(t1)] > arr[UnRel(t2)])){
		SortPrint(a, b, UnRel(t2));
		NextTurn(1, Rel(b));
		NextTurn(Rel(b), t2);
		NextTurn(t2, N + 1);
	}else{
		SortPrint(a, b, UnRel(t1));
		NextTurn(1, t1);
		NextTurn(t1, Rel(b));
		NextTurn(Rel(b), N + 1);
	}
}

int main(){
	int i;
	scanf("%d", &N);
	for(i = 1; i <= N; i ++)
		scanf("%d", &arr[i]);
	arr[N + 1] = arr[1];
	nln = 0;

	FirstTurn();

	sort(lines, lines + nln);
	for(i = 0; i < nln; i ++){
		printf("%d %d %d\n", lines[i].a, lines[i].b, lines[i].c);
	}

	return 0;
}