#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
typedef long long LL;

//���鼯
class UnionSet{
public:
	//�ڵ�
	typedef struct NODE{
		NODE *par;
		int rank;  //��
		int siz;   //�Ӽ���С
	}*PNODE;
	NODE node[MAXN];
	//��¼��ʷ������ջ
	struct OPERAT{
		PNODE sub, par; //���ӣ�����
		int siz;        //���ӵĴ�С
		bool addrk;     //���׵����Ƿ�����
		OPERAT(){};
		OPERAT(PNODE sub, PNODE par, int siz, bool addrk):sub(sub),par(par),siz(siz),addrk(addrk){}
	}stk[MAXN];
	int top;
	//��ʼ��
	void Init(int N){
		for(int i = 0; i <= N; i ++){
			node[i].par = &node[i];
			node[i].rank = 0;
			node[i].siz = 1;
		}
		top = 0;
	}
	//���a�ڲ��鼯��ĸ�
	inline PNODE GetRoot(PNODE a){
		while(a->par != a)
			a = a->par;
		return a;
	}
	//�ϲ�a��b�����غϲ������Ŀɴﶥ�����
	LL Union(PNODE a, PNODE b){
		a = GetRoot(a);
		b = GetRoot(b);
		if(a != b){
			LL ret = (LL)(a->siz) * (LL)(b->siz);
			bool addrk = false;
			if(a->rank > b->rank){
				swap(a, b);
			}else if(a->rank == b->rank){
				b->rank ++;
				addrk = true;
			}
			stk[top ++] = OPERAT(a, b, a->siz, addrk);
			a->par = b;
			b->siz += a->siz;
			a->siz = 0;
			return ret;
		}else
			return 0;
	}
	//Rollbackֱ��base��
	void Regret(int base){
		while(top > base){
			top --;
			stk[top].sub->par = stk[top].sub;
			stk[top].sub->siz = stk[top].siz;
			stk[top].par->siz -= stk[top].siz;
			if(stk[top].addrk)
				stk[top].par->rank --;
		}
	}
}us;
//�ߵļ���
struct EDGE{
	UnionSet::PNODE a, b;  //��������
	int tim;   //����ʱ��
	bool operator<(const EDGE& rhs)const{
		return tim < rhs.tim;
	};
}edge[MAXM];
int N, M, K;
bool Input(){
	int a, b, i;
	if(scanf("%d%d%d", &N, &M, &K) == EOF)
		return false;
	for(i = 0; i < M; i ++){
		scanf("%d%d%d", &a, &b, &edge[i].tim);
		edge[i].a = us.node + a;
		edge[i].b = us.node + b;
	}
	sort(edge, edge + M);
	return true;
}
//��ʱ��������ν��
//��ʱ��߽磬��ʱ��߽磬Ԥ�ȼӺõĴ𰸣�egde����߽磬edge���ұ߽�
void DivAndConq(int l, int r, LL pre, int el, int er){
	//�ݹ��յ�
	if(l == r){
		printf("%lld\n", pre);
		return ;
	}
	const int mid = (l + r) >> 1, base = us.top;
	int i;
	LL ans;
	//��ʱ�䳬��mid�ı߶��ϲ���Ȼ��ݹ鴦������
	for(ans = pre, i = er; i >= el && edge[i].tim > mid; i --)
		ans += us.Union(edge[i].a, edge[i].b);
	DivAndConq(l, mid, ans, el, i);
	us.Regret(base);
	//�����Һ���
	for(ans = pre, i = el; i <= er && edge[i].tim <= mid; i ++)
		ans += us.Union(edge[i].a, edge[i].b);
	DivAndConq(mid + 1, r, ans, i, er);
	us.Regret(base);
}
void Solve(){
	LL ans = 0;
	int ep;
	us.Init(N);
	for(ep = M - 1; ep >= 0 && edge[ep].tim > K; ep --)
		ans += us.Union(edge[ep].a, edge[ep].b);
	DivAndConq(1, K, ans, 0, ep);
}
int main(){
	while(Input())
		Solve();
	return 0;
}