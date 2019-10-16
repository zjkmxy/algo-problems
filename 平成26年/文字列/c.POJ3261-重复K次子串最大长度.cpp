#ifndef _DEBUG
#define nullptr NULL
#endif

#include <cstdio>
#include <algorithm>
#include <map>
#include <list>
using namespace std;

const int ALB = 180;
const int MAXN = 20005;
class SAM{
	struct NODE{
		NODE *par;
		map<int, NODE*> go;
		list<NODE*> chd;
		int val;	//max
		int rcnt;	//rightµÄ¸öÊý
		NODE(int val):par(nullptr),val(val),rcnt(1){
			go.clear();
		}
		NODE(){};
	}node[MAXN<<1], *last;
	int ncnt, len;
public:
	void Init(){
		ncnt = 1;
		len = 0;
		last = node;
		node[0] = NODE(0);
	}
	void Append(int x){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1);
		while(p){
			map<int, NODE*>::iterator it = p->go.find(x);
			if(it == p->go.end())
				p->go[x] = np;
			else
				break;
			p = p->par;
		}
		if(p){
			NODE *q = p->go[x];
			if(p->val + 1 == q->val){
				np->par = q;
			}else{
				NODE *nq = &node[ncnt ++];
				*nq = *q;
				nq->rcnt = 0;
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

	void DFS(NODE* cur){
		list<NODE*>::iterator it;
		for(it = cur->chd.begin(); it != cur->chd.end(); it ++){
			DFS(*it);
			cur->rcnt += (*it)->rcnt;
		}
	}
	int Chord(int K){
		int ans = 0;
		for(int i = ncnt - 1; i > 0; i --){
			node[i].par->chd.push_back(&node[i]);
		}
		DFS(node);
		for(int i = ncnt - 1; i > 0; i --){
			if(node[i].rcnt >= K)
				ans = max(ans, node[i].val);
		}
		return ans;
	}
}sam;
int N, K;

int main(){
	sam.Init();
	scanf("%d%d", &N, &K);
	while(N --){
		int a;
		scanf("%d", &a);
		sam.Append(a);
	}
	printf("%d\n", sam.Chord(K));
	return 0;
}
