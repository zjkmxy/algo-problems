#include<algorithm> 
#include<cstdio>
#include<cstring>
using namespace std; 

typedef unsigned long long ll;
const int ALB = 26;
const int MAXN = 90005;
const int INF = 0x3fffffff;

char stra[MAXN], strb[MAXN];
ll bcnt[ALB], ans;

class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//当前状态max的值
		int ch;
		NODE(int val, int ch):par(NULL),val(val),ch(ch){
			fill_n(go, ALB, (NODE*)NULL);
		}
		NODE(){};
		inline int cnt(){
			return par ? val - par->val : val;
		}
	}node[MAXN<<1], *last;
	int ncnt;
public:
	void Init(){
		ncnt = 1;
		last = node;
		node[0] = NODE(0, -1);
	}
	void Append(int x){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1, x);
		for(; p && !p->go[x]; p = p->par)
			p->go[x] = np;
		if(p){
			NODE *q = p->go[x];
			if(p->val + 1 == q->val){
				np->par = q;
			}else{
				NODE *nq = &node[ncnt ++];
				*nq = *q;
				nq->val = p->val + 1;
				q->par = nq;
				np->par = nq;
				for(; p && p->go[x] == q; p = p->par)
					p->go[x] = nq;
			}
		}else{
			np->par = node;
		}
		last = np;
	}

	//b是反插成前缀的，所以直接打表
	inline void DealB(){
		fill_n(bcnt, ALB, 0);
		for(NODE* x = node + 1; x < node + ncnt; x ++)
			bcnt[x->ch] += x->cnt();
	}
	//a是要照后计算的
	inline void DealA(){
		ans = 0;
		for(NODE* x = node; x < node + ncnt; x ++){
			ans += x->cnt();
			for(int ch = 0; ch < ALB; ch ++){
				if(!x->go[ch])
					ans += x->cnt() * bcnt[ch];
			}
		}
	}
}sa, sb;

void domain(){
	char *p;
	scanf("%s%s", &stra, &strb);
	sa.Init();
	for(p = stra; *p; ++ p)
		sa.Append(*p - 'a');
	sb.Init();
	for(p = strb; *p; ++ p);
	for(-- p; p >= strb; -- p)
		sb.Append(*p - 'a');
	sb.DealB();
	sa.DealA();
	printf("%I64u\n", ans + 1);
}

int main(){
	int T;
	scanf("%d", &T);
	while(T --)
		domain();
	return 0;
}