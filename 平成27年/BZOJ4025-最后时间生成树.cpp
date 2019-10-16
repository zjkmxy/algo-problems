//�ο����http://c-sunshine.logdown.com/posts/261889/bzoj4025-bipartite-graph
//WC2015����չ��������

#define nullptr NULL
#include <cstdio>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
const int INF = 0x3fffffff;
struct EDGE{
	int a, b;
	int st, ed;
	bool used;	//�Ƿ�ʹ��
	bool odd;	//�Ƿ��γ�������
}edge[MAXM];
int VioCnt;	//Υ����
bool Ans[MAXN];	//��
int N, M, T;
pair<int, int> opt[MAXM << 1];	//�ߵĽ���ʱ��

int ReadInt(){
	int ret = 0;
	char c;
	while(!isdigit(c = getchar()));
	ret = c - '0';
	while(isdigit(c = getchar())){
		ret = ret * 10 + (c - '0');
	}
	return ret; 
}


class LCT{
private:
	typedef struct NODE{
		NODE *l, *r, *par;  //���ң�����
		bool root;          //�Ƿ�Ϊ���������ú����飩
		int fix, size;

		//������
		bool rev;	//��ת
		int val;	//��ֵ
		NODE* mx;	//��Сֵ��
		NODE(int val = INF):fix(rand()),size(1),val(val){
			par = l = r = nullptr;
			root = true;

			mx = this;
			rev = false;
		};
	}*PNODE;
	PNODE root;
	NODE node[MAXN], edl[MAXM];	//������ͱߵĵ�
	int pcnt;
	typedef pair<PNODE, PNODE> Droot;

	inline int size(PNODE cur){  //ȡ�ô�С
		return cur ? cur->size : 0;
	}
	inline void do_rev(PNODE cur){
		if(cur){
			cur->rev = !cur->rev;
			swap(cur->l, cur->r);
		}
	}
	inline void pushdown(PNODE cur){
		if(cur->rev){
			do_rev(cur->l);
			do_rev(cur->r);
			cur->rev = false;
		}
	}
	inline void popup(PNODE cur){
		cur->size = 1;
		cur->mx = cur;
		if(cur->l){
			cur->l->root = false;
			cur->l->par = cur;
			cur->size += cur->l->size;
			if(cur->l->mx->val < cur->mx->val)
				cur->mx = cur->l->mx;
		}
		if(cur->r){
			cur->r->root = false;
			cur->r->par = cur;
			cur->size += cur->r->size;
			if(cur->r->mx->val < cur->mx->val)
				cur->mx = cur->r->mx;
		}
	}

	void getrank(PNODE A, PNODE& rt, int& rk){
		static PNODE stk[MAXN];
		int sp = 0;
		//��ǿ�Ƹ���
		rt = A;
		while(!rt->root){
			rt = rt->par;
			stk[sp ++] = rt;
		}
		while(sp --){
			pushdown(stk[sp]);
		}
		//�ٴ������
		rt = A;
		rk = size(rt->l) + 1;
		while(!rt->root){
			if(rt == rt->par->r)
				rk += rt->par->size - rt->size;
			rt = rt->par;
		}
	}
	PNODE concat_it(PNODE A, PNODE B){ //����ϲ�
		if(!A) return B;
		if(!B) return A;
		if(A->fix < B->fix){
			pushdown(A);
			A->r = concat_it(A->r, B);
			popup(A);
			return A;
		}else{
			pushdown(B);
			B->l = concat_it(A, B->l);
			popup(B);
			return B;
		}
	}
	inline PNODE concat(PNODE A, PNODE B){
		PNODE ret = concat_it(A, B);
		ret->root = true;
		return ret;
	}
	Droot split_it(PNODE A, int k){
		if(!A) return Droot(nullptr, nullptr);
		Droot ret;
		pushdown(A);
		A->root = true;
		if(size(A->l) >= k){
			ret = split_it(A->l, k);
			A->l = ret.second;
			popup(A);
			ret.second = A;
		}else{
			ret = split_it(A->r, k - size(A->l) - 1);
			A->r = ret.first;
			popup(A);
			ret.first = A;
		}
		return ret;
	}
	inline Droot split(PNODE rt, int k){  //���ѵ㣬��λ
		NODE *tmp = rt->par;
		Droot ret = split_it(rt, k);
		if(ret.first)
			ret.first->par = tmp;
		if(ret.second){
			for(tmp = ret.first; tmp->r; tmp = tmp->r);
			ret.second->par = tmp;
		}
		return ret;
	}
	PNODE access(PNODE A){
		PNODE rt, pchd = nullptr;
		int rk;
		while(A){
			getrank(A, rt, rk);
			A = rt->par;
			pchd = concat(split(rt, rk).first, pchd);
			pchd->par = A;
		}
		return pchd;
	}
private:
	inline NODE* GetRt(PNODE p){
		while(p->par)
			p = p->par;
		return p;
	}
	inline void Link(int j){
		edl[j].par = node + edge[j].a;
		PNODE rt = access(edl + j);
		do_rev(rt);
		rt->par = node + edge[j].b;
		edge[j].used = true;
	}
	inline void DoCut(PNODE u){
		PNODE rt;
		int rk;
		access(u);
		getrank(u, rt, rk);
		rt = split(rt, rk - 1).second;
		rt->par = nullptr;
	}
	inline void Cut(int j){
		ChangeRoot(edl + j);
		DoCut(node + edge[j].a);
		DoCut(node + edge[j].b);
		edge[j].used = false;
	}
	inline void ChangeRoot(PNODE u){
		do_rev(access(u));
	}

public:
	//����ĳ���ߵ���ż��
	inline void ChgEdge(PNODE v, int j){
		if(!((size(v) >> 1) & 1)){
			edge[j].odd = true;
			VioCnt ++;
		}
	}

	//��Ŀ�в������ӱ߻���
	void AddEdge(int j){
		PNODE u = node + edge[j].a, v = node + edge[j].b, z;
		//�Ƿ�ֱ�����ӣ�
		if(GetRt(u) != GetRt(v)){
			Link(j);
			return;
		}
		//����·����С
		ChangeRoot(u);
		v = access(v);
		z = v->mx;
		//�����ȸ�����ż�ԣ��ټӱ�
		if(edge[j].ed > z->val){
			int lab = z - edl;
			ChgEdge(v, lab);
			Cut(lab);
			Link(j);
		}else{
			ChgEdge(v, j);
		}
	}
	//��Ŀ�в�����ɾ��
	void DelEdge(int j){
		if(edge[j].used){
			Cut(j);
		}else if(edge[j].odd){
			edge[j].odd = false;
			VioCnt --;
		}
	}
	//��ʼ��
	void Init(int N, int M){
		pcnt = N + 1;
		for(int i = 0; i < M; i ++){
			edge[i].a = ReadInt();
			edge[i].b = ReadInt();
			edge[i].st = ReadInt();
			edge[i].ed = ReadInt();
			edge[i].used = edge[i].odd = false;
			VioCnt = 0;
			edl[i] = NODE(edge[i].ed);
			opt[i << 1] = make_pair(edge[i].st, i);
			opt[i << 1 | 1] = make_pair(edge[i].ed, i);
		}
		sort(opt, opt + (M << 1));
	}
}tree;

int main(){
	scanf("%d%d%d", &N, &M, &T);
	tree.Init(N, M);
	for(int i = 1, p = 0; i <= T; i ++){
		for(; p < (M << 1) && opt[p].first < i; p ++){
			int t = opt[p].first, w = opt[p].second;
			if(edge[w].st == t){
				tree.AddEdge(w);
			}else{
				tree.DelEdge(w);
			}
			VioCnt = VioCnt;
		}
		Ans[i] = (VioCnt == 0);
	}
	for(int i = 1; i <= T; i ++){
		puts(Ans[i] ? "Yes" : "No");
	}
	return 0;
}
