//问有多少个不同的子串
//每个节点的val减去父亲的val，最后加成总和就是答案
//动态确保每一个节点不重不漏地加过一遍，打表就好
#include <cstdio>
#include <algorithm>
using namespace std;

const int ALB = 180;
const int MAXN = 5005;
typedef unsigned long ul;

class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//max
		int right;
		NODE(int val, int right):par(nullptr),val(val),right(right){
			fill_n(go, ALB, reinterpret_cast<NODE*>(nullptr));
		}
		NODE(){};
		//计算有多少个不同的子串
		inline int calc(){
			if(par)
				return val - par->val;
			else
				return val;
		}
	}node[MAXN<<1], *last;
	int ncnt, len;
public:
	int ans;
	void Init(){
		ncnt = 1;
		len = 0;
		ans = 0;
		last = node;
		node[0] = NODE(0, 0);
	}
	void Append(int x){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1, ++ len);
		for(; p && !p->go[x]; p = p->par)
			p->go[x] = np;
		if(p){
			NODE *q = p->go[x];
			if(p->val + 1 == q->val){
				np->par = q;
				ans += np->calc();
			}else{
				ans -= p->calc() + q->calc();
				NODE *nq = &node[ncnt ++];
				*nq = *q;
				nq->val = p->val + 1;
				q->par = nq;
				np->par = nq;
				ans += p->calc() + q->calc() + np->calc() + nq->calc();
				for(; p && p->go[x] == q; p = p->par)
					p->go[x] = nq;
			}
		}else{
			np->par = node;
			ans += np->calc();
		}
		last = np;
	}
}sam;
int len;
char str[MAXN];
int dp[MAXN][MAXN];

void domain(){
	int Q, i, j;
	scanf("%s", str);
	len = strlen(str);
	for(i = 0; i < len; i ++){
		sam.Init();
		for(j = i; j < len; j ++){
			sam.Append(str[j] - 'a');
			dp[i+1][j+1] = sam.ans;
		}
	}
	scanf("%d", &Q);
	while(Q --){
		scanf("%d%d", &i, &j);
		printf("%d\n", dp[i][j]);
	}
}

int main(){
	int T;
	scanf("%d", &T);
	while(T --)
		domain();
	return 0;
}
