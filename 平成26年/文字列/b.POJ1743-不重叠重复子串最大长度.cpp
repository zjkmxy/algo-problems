//POJ������ˮ����
//SAM����ɨ�裬ά��������С��right
#ifndef _DEBUG
#define nullptr NULL
#endif

#include <cstdio>
#include <algorithm>
using namespace std;

const int ALB = 180;
const int MAXN = 20005;
class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//max
		int rcnt;	//right�ĸ���
		int right;	//����ߵ�right��λ��
		int mr;		//���ұߵ�rightλ��
		NODE(int val, int right):par(nullptr),val(val),rcnt(1),right(right),mr(right){
			fill_n(go, ALB, reinterpret_cast<NODE*>(nullptr));
		}
		NODE(){};
	}node[MAXN<<1], *last;
	int ncnt, len;
public:
	void Init(){
		ncnt = 1;
		len = 0;
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

	int Chord(){
		int ans = 0;
		//ע�����ﲢ������ȷ����rcnt����c��
		//�еĺ�����par���֮ǰ
		for(int i = ncnt - 1; i > 0; i --){
			if(node[i].par){
				node[i].par->rcnt += node[i].rcnt;
				int cur = min(node[i].par->val, node[i].mr - node[i].par->right);
				ans = max(cur, ans);
				node[i].par->mr = max(node[i].par->mr, node[i].mr);
				node[i].par->right = min(node[i].par->right, node[i].right);
			}
		}
		return ans;
	}
}sam;
int N;
int arr[MAXN];

int main(){
	for(scanf("%d", &N); N; scanf("%d", &N)){
		sam.Init();
		for(int i = 0; i < N; i ++){
			scanf("%d", &arr[i]);
		}
		N --;
		for(int i = 0; i < N; i ++){
			sam.Append(arr[i + 1] - arr[i] + 90);
		}
		sam.Append(0);
		int ans = sam.Chord();
		if(ans < 4)
			ans = 0;
		else
			ans ++;
		printf("%d\n", ans);
	}
	return 0;
}
