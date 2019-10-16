/**************************************************************
Problem: 3065
User: zjkmxy
Language: C++
Result: Accepted
Time:25792 ms
Memory:156416 kb
****************************************************************/

#include <cstdio>
#include <algorithm>
#include <cctype>
using namespace std;

const int MAXN = 70005;
const int MAXV = 70000;
const int MAXT = MAXN * 140;
const double ALPHA = 0.80;

//ֵ���߶���
class Segtree{
public:

	//�߶����ڵ�أ����Զ�̬����
	static struct SN{
		SN *lchd, *rchd;
		int cnt;    //ֵ��������Ԫ�ظ���
		static SN* create();
		void destroy();

		inline void popup(){
			cnt = lchd->cnt + rchd->cnt;
		}
	}pool[MAXT];
	typedef SN *PSN;
	static int tcnt;
	static PSN recyc[MAXT]; //�Ѿ����յĽڵ�
	static int nrec;    //���սڵ����
	static PSN null;    //�ڱ�

	static void InitAll(){
		tcnt = 1;
		nrec = 0;
		null = &pool[0];
		null->cnt = 0;
		null->lchd = null;
		null->rchd = null;
	}

	//�߶������ڲ�����������
	PSN root;
	//���ﲻ�����������������������rt������null��������麢��
	void insert(PSN& rt, int val, int cnt, int L, int R){
		if(rt == null)
			rt = SN::create();
		if(L == R){
			rt->cnt += cnt;
		}else{
			int mid = (L + R) >> 1;
			if(val <= mid)
				insert(rt->lchd, val, cnt, L, mid);
			else
				insert(rt->rchd, val, cnt, mid + 1, R);
			rt->popup();
		}
		if(rt->cnt == 0){
			rt->destroy();
			rt = null;
		}
	};

	Segtree():root(null){};
	//����ӿڣ�����cnt��ֵΪval����
	inline void Insert(int val, int cnt){
		insert(root, val, cnt, 0, MAXV);
	}
	//����ӿڣ����������
	inline void Clear(){
		root->destroy();
		root = null;
	}
};

inline Segtree::SN* Segtree::SN::create(){
	SN* ret;
	if(nrec > 0){
		-- nrec;
		ret = recyc[nrec];
	}else
		ret = &pool[tcnt ++];
	ret->cnt = 0;
	ret->lchd = ret->rchd = null;
	return ret;
}

inline void Segtree::SN::destroy(){
	recyc[nrec ++] = this;
	if(lchd != null){
		lchd->destroy();
		lchd = null;
	}
	if(rchd != null){
		rchd->destroy();
		rchd = null;
	}
}
Segtree::SN Segtree::pool[MAXT];
Segtree::PSN Segtree::null = pool;
int Segtree::tcnt;
Segtree::PSN Segtree::recyc[MAXT];
int Segtree::nrec;

//��������
class SCGT{
public:
	struct NODE{
		int val, cnt;
		NODE *lchd, *rchd;
		Segtree segt;
	}node[MAXN];
	int ncnt;
	typedef NODE *PNODE;
	PNODE root, null;
	PNODE dfn[MAXN];    //�����������ؽ��ڵ���
	Segtree::PSN inv[MAXN]; //query��������
	int ninv;
	int exv[MAXN];  //query������ֵ
	int nexv;

	//����arr�����ֵ��ʼ����
	void Init(int arr[], int N){
		Segtree::InitAll();
		null = node;
		null->lchd = null->rchd = null;
		null->val = -1;
		null->cnt = 0;
		for(int i = 1; i <= N; i ++){
			dfn[i] = &node[i];
			dfn[i]->val = arr[i];
		}
		root = Build(1, N);
		ncnt = N + 1;
	}
	//����dfn��L��R����������������
	PNODE Build(int L, int R){
		PNODE ret = null;
		if(L > R)
			return ret;
		int mid = (L + R) >> 1;
		ret = dfn[mid];
		ret->lchd = Build(L, mid - 1);
		ret->rchd = Build(mid + 1, R);
		for(int i = L; i <= R; i ++)
			ret->segt.Insert(dfn[i]->val, 1);
		ret->cnt = (R - L + 1);
		return ret;
	};
	//ɾ��rtΪ��������������ɾ���������ڵ������������lst��
	int Remove(PNODE& rt, PNODE lst[]){
		int cnt = 0;
		if(rt == null)
			return 0;
		rt->segt.Clear();
		cnt += Remove(rt->lchd, lst);
		lst[cnt ++] = rt;
		cnt += Remove(rt->rchd, lst + cnt);
		rt = null;
		return cnt;
	}
	//�ع�rtΪ��������
	inline void Rebuild(PNODE& rt){
		int cnt = Remove(rt, dfn);
		rt = Build(0, cnt - 1);
	}
	//ȡ��[l..r]�����䣬�ѹ�����ŵ�����
	void GetInt(PNODE rt, int l, int r){
		int lcnt = rt->lchd->cnt, cnt = rt->cnt;
		if(l == 1 && r == cnt){
			inv[ninv ++] = rt->segt.root;
			return ;
		}
		if(l <= lcnt + 1 && r >= lcnt + 1)
			exv[nexv ++] = rt->val;
		if(l <= lcnt)
			GetInt(rt->lchd, l, min(r, lcnt));
		if(r > lcnt + 1)
			GetInt(rt->rchd, max(l - lcnt - 1, 1), r - lcnt - 1);
	}
	//��ѯL��R֮��ĵ�K��
	int Query(int L, int R, int K){
		int i, l = 0, r = MAXV, mid, lcnt;
		ninv = nexv = 0;
		GetInt(root, L, R);
		while(l != r){
			mid = (l + r) >> 1;
			lcnt = 0;
			for(i = 0; i < ninv; i ++)
				lcnt += inv[i]->lchd->cnt;
			for(i = 0; i < nexv; i ++)
				lcnt += (exv[i] >= l && exv[i] <= mid) ? 1 : 0;
			if(K <= lcnt){
				for(i = 0; i < ninv; i ++)
					inv[i] = inv[i]->lchd;
				r = mid;
			}else{
				for(i = 0; i < ninv; i ++)
					inv[i] = inv[i]->rchd;
				l = mid + 1;
				K -= lcnt;
			}
		}
		return l;
	}
	//�޸�rtΪ���������еĵ�pos��ֵ������ԭ�ȵ�ֵ
	int Change(PNODE& rt, int pos, int val){
		int ori, lcnt = rt->lchd->cnt;
		rt->segt.Insert(val, 1);
		if(pos == lcnt + 1){
			ori = rt->val;
			rt->val = val;
		}else if(pos <= lcnt){
			ori = Change(rt->lchd, pos, val);
		}else{
			ori = Change(rt->rchd, pos - lcnt - 1, val);
		}
		rt->segt.Insert(ori, -1);
		return ori;
	}

	//��pos��ֵ�������val������������ڵ�
	PNODE Insert(PNODE &rt, int pos, int val){
		if(rt == null){
			rt = &node[ncnt ++];
			rt->val = val;
			rt->lchd = rt->rchd = null;
			rt->segt.Insert(val, 1);
			rt->cnt = 1;
			return null;
		}
		rt->segt.Insert(val, 1);
		rt->cnt ++;
		int lcnt = rt->lchd->cnt;
		PNODE scape;
		if(pos <= lcnt)
			scape = Insert(rt->lchd, pos, val);
		else
			scape = Insert(rt->rchd, pos - lcnt - 1, val);
		//����������˵������һ��������ڵ�
		if(rt->cnt * ALPHA > max(rt->lchd->cnt, rt->rchd->cnt)){
			if(scape != null){
				if(rt->lchd == scape)
					Rebuild(rt->lchd);
				else
					Rebuild(rt->rchd);
			}
			return null;
		}else
			return rt;
	}
}tree;

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

char ReadOp(){
	char ret = 0;
	do{
		ret = getchar();
	}while(!isalpha(ret));
	return ret;
}

int arr[MAXN], N, Q, lastans;
int main(){
	int i, x, y, k;
	char op[3];

	N = ReadInt();
	for(i = 1; i <= N; i ++){
		arr[i] = ReadInt();
	}
	tree.Init(arr, N);
	Q = ReadInt();
	while(Q --){
		op[0] = ReadOp();
		x = ReadInt();
		y = ReadInt();
		x ^= lastans;
		y ^= lastans;

		if(op[0] == 'I'){
			x --;
			tree.Insert(tree.root, x, y);
		}else if(op[0] == 'M'){
			tree.Change(tree.root, x, y);
		}else{
			k = ReadInt();
			k ^= lastans;
			lastans = tree.Query(x, y, k);
			printf("%d\n", lastans);
		}
	}
	return 0;
}