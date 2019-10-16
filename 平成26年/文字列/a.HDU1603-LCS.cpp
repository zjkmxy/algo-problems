#include <cstdio>
#include <algorithm>
using namespace std;

const int ALB = 26;
const int MAXN = 100005;
class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//当前状态max的值
		NODE(int val):par(nullptr),val(val){
			fill_n(go, ALB, nullptr);
		}
		NODE(){};
	}node[MAXN<<1], *last;
	int ncnt;
public:
	void Init(){
		ncnt = 1;
		last = node;
		node[0] = NODE(0);
	}
	void Append(int x){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1);
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

	int LCS(char *p){
		int ans(0), len(0);
		NODE *cur = node;
		for(; *p; ++ p){
			int ch = *p - 'a';
			if(cur->go[ch]){
				cur = cur->go[ch];
				++ len;
			}else{
				while(cur && !cur->go[ch])
					cur = cur->par;
				if(!cur){
					cur = node;
					len = 0;
				}else{
					len = cur->val + 1;
					cur = cur->go[ch];
				}
			}
			ans = max(ans, len);
		}
		return ans;
	}
}sam;

char stra[MAXN], strb[MAXN];

int main(){;
	while(scanf("%s%s", stra, strb) != EOF){
		sam.Init();
		for(char *p = stra; *p; p ++)
			sam.Append(*p - 'a');
		printf("%d\n", sam.LCS(strb));
	}
	return 0;
}
