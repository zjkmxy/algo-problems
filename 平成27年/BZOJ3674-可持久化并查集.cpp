#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 20005;
typedef long long LL;
class Tree{
private:
	typedef struct NODE{
		NODE *l, *r;
		int par;  //���鼯
		int deep;
		int siz;
	} *PNODE;
	NODE node[MAXN*80];
	PNODE pending, root[MAXN];
	int cnt, n, rt;
	inline PNODE build(){
		PNODE ret = &node[cnt++];
		ret->par = -1;
		ret->deep = 0;
		ret->siz = 1;
		ret->l = ret->r = pending;
		return ret;
	}
	NODE* forcenode(PNODE cur, int pos, int L, int R){
		int mid;
		while(L != R){
			mid = (L + R) >> 1;
			if(pos <= mid){
				if(cur->l == pending)
					cur->l = build();
				cur = cur->l;
				R = mid;
			}else{
				if(cur->r == pending)
					cur->r = build();
				cur = cur->r;
				L = mid + 1;
			}
		}
		return cur;
	}
	NODE* update(PNODE cur, int pos, int par, int deep, int siz, int L, int R){
		PNODE ret = &node[cnt++];
		(*ret) = (*cur);
		if(L == R){
			ret->deep = deep;
			ret->par = par;
			ret->siz = siz;
		}else{
			int mid = (L + R) >> 1;
			if(pos <= mid)
				ret->l = update(ret->l, pos, par, deep, siz, L, mid);
			else
				ret->r = update(ret->r, pos, par, deep, siz, mid+1, R);
		}
		return ret;
	}
public:
	void Init(int N){
		cnt = 1;
		this->n = N;
		pending = &node[0];
		pending->l = pending->r = pending;
		pending->par = -1;
		pending->siz = 1;
		pending->deep = 0;
		root[0] = build();
		rt = 0;
	}
	//deep��sizΪ���
	int GetRoot(int id, int &deep, int &siz){
		PNODE cur = forcenode(root[rt], id, 1, n);
		if(cur->par >= 0){
			int tmp = GetRoot(cur->par, deep, siz);
			update(root[rt], id, tmp, cur->deep, cur->siz, 1, n);
			return tmp;
		}else{
			deep = cur->deep;
			siz = cur->siz;
			return id;
		}
	}
	//�ϲ������سɹ�ʧ��
	bool Merge(int a, int b){
		int dpa, dpb, sza, szb;
		a = GetRoot(a, dpa, sza);
		b = GetRoot(b, dpb, szb);
		if(dpa > dpb){
			swap(a, b);
			swap(dpa, dpb);
			swap(sza, szb);
		}
		////////ʧ����Ҳ��������
		rt ++;
		root[rt] = root[rt - 1];
		////////ʧ����Ҳ��������
		if(a != b){
			root[rt] = update(root[rt], a, b, dpa, 0, 1, n);
			if(dpa == dpb)
				dpb ++;
			root[rt] = update(root[rt], b, -1, dpb, sza + szb, 1, n);
			return true;
		}else{
			return false;
		}
	}
	//��ѯ�������Ƿ�ͬһ��
	bool Query(int a, int b){
		int dpa, dpb, sza, szb;
		a = GetRoot(a, dpa, sza);
		b = GetRoot(b, dpb, szb);
		////////��ѯ��������
		rt ++;
		root[rt] = root[rt - 1];
		////////
		return (a == b);
	}
	//Rollback���˻ص�k��֮��
	void Regret(int k){
		rt ++;
		root[rt] = root[k];
	}
}tree;
int N, M;
int main(){
	int f, a, b;
	scanf("%d%d", &N, &M);
	tree.Init(N);
	while(M --){
		scanf("%d", &f);
		if(f == 1){
			//�ϲ�
			scanf("%d%d", &a, &b);
			tree.Merge(a, b);
		}else if(f == 2){
			//�ع�
			scanf("%d", &a);
			tree.Regret(a);
		}else if(f == 3){
			//��ѯ
			scanf("%d%d", &a, &b);
			if(tree.Query(a, b)){
				printf("1\n");
			}else{
				printf("0\n");
			}
		}else{
			//��ѯ��С
			scanf("%d", &a);
			tree.GetRoot(a, a, b);
			printf("%d\n", b);
		}
	}
	return 0;
}