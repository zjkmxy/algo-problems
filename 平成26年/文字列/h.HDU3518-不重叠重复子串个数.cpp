#include <cstdio>
#include <algorithm>
#include <list>
using namespace std;

const int ALB = 180;
const int MAXN = 20005;
class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		list<NODE*> chd;
		int val;	//max
		int rcnt;	//right的个数
		int right;	//最左边的right的位置
		int mr;		//最右边的right位置
		NODE(int val, int right):par(nullptr),val(val),rcnt(1),right(right),mr(right){
			fill_n(go, ALB, reinterpret_cast<NODE*>(nullptr));
			chd.clear();
		}
		NODE(){};
	}node[MAXN<<1], *last;
	int ncnt, len;
public:
	int ans;
	void Init(){
		ncnt = 1;
		len = 0;
		last = node;
		ans = 0;
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
			cur->mr = max(cur->mr, (*it)->mr);
			cur->right = min(cur->right, (*it)->right);
		}
	}
	int Chord(){
		int ans = 0;
		for(int i = ncnt - 1; i > 0; i --){
			node[i].par->chd.push_back(&node[i]);
		}
		DFS(node);
		for(int i = ncnt - 1; i > 0; i --){
			if(node[i].rcnt > 1){
				int cur = min(node[i].val, node[i].mr - node[i].right);
				int pv = node[i].par ? node[i].par->val : 0;
				if(cur > pv)
					ans += cur - pv;
			}
		}
		return ans;
	}
}sam;
char str[MAXN];

int main(){
	for(scanf("%s", str); str[0] != '#'; scanf("%s", str)){
		sam.Init();
		for(char *ch = str; *ch; ++ ch)
			sam.Append(*ch - 'a');
		printf("%d\n", sam.Chord());
	}
	return 0;
}
