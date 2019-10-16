/*
��ʼ��һ��ȫ0��NԪ�����飬Q�β������£�
1 l r x : ��[l,r]�����ڵ�����Ԫ�ؼ�x����������ӵó���100�˾ͱ��100.x������100
2 l r x : ��[l,r]�����ڵ�����Ԫ�ؼ�x������������õ���0�˾ͱ��0.
3 l r   : ��[l,r]����������Ԫ�صĺ�

˼·��
��״������ֵ���������߶���
*/
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXC = 100;
const int MAXN = 100001;
const int MAXB = 320;
const int SIZE = 320;

//������⣺������
namespace BF{
	int arr[MAXN], N, Q;
	void add(int l, int r, int val){
		for(int i = l; i <= r; i ++){
			arr[i] += val;
			arr[i] = min(max(arr[i], 0), 100);
		}
	}
	int sum(int l, int r){
		int ret = 0;
		for(int i = l; i <= r; i ++){
			ret += arr[i];
		}
		return ret;
	}
};

//ֵ��������߶��������ܼ�ע��
class Segtree{
	struct NODE{
		int cnt, sum;	//��������
		bool rst;		//�����־
		NODE *lchd, *rchd;
	}node[MAXC * 2 + 5];
	int ncnt;
	inline void popup(NODE* cur){
		cur->sum = cur->lchd->sum + cur->rchd->sum;
		cur->cnt = cur->lchd->cnt + cur->rchd->cnt;
	}
	inline void pushdown(NODE* cur){
		if(cur->rst){
			cur->lchd->cnt = 0;
			cur->lchd->sum = 0;
			cur->lchd->rst = true;
			cur->rchd->cnt = 0;
			cur->rchd->sum = 0;
			cur->rchd->rst = true;
			cur->rst = false;
		}
	}
	NODE* build(int L, int R){
		NODE *ret = &node[ncnt ++];
		ret->sum = ret->cnt = 0;
		ret->rst = false;
		if(L < R){
			int mid = (L + R) >> 1;
			ret->lchd = build(L, mid);
			ret->rchd = build(mid + 1, R);
		}else{
			ret->lchd = ret->rchd = NULL;
		}
		return ret;
	}
	//����val��ֵΪpos����
	void update(NODE* cur, int pos, int val,int L, int R){
		if(L == R){
			cur->cnt += val;
			cur->sum += val * L;
			return ;
		}
		int mid = (L + R) >> 1;
		pushdown(cur);
		if(pos <= mid){
			update(cur->lchd, pos, val, L, mid);
		}else{
			update(cur->rchd, pos, val, mid + 1, R);
		}
		popup(cur);
	}
	//��ֵ��[l, r]�ڵ�������ɾȥ������ɾ�������ָ���
	int erase(NODE* cur, int l, int r, int L, int R){
		int ret = 0;
		if(l == L && r == R){
			ret = cur->cnt;
			cur->cnt = cur->sum = 0;
			cur->rst = true;
			return ret;
		}
		int mid = (L + R) >> 1;
		pushdown(cur);
		if(r <= mid){
			ret += erase(cur->lchd, l, r, L, mid);
		}else if(l > mid){
			ret += erase(cur->rchd, l, r, mid + 1, R);
		}else{
			ret += erase(cur->lchd, l, mid, L, mid);
			ret += erase(cur->rchd, mid + 1, r, mid + 1, R);
		}
		popup(cur);
		return ret;
	}
public:
	//����
	inline void Build(){
		ncnt = 0;
		build(0, MAXC);
	}
	//����cnt��ֵΪval����
	inline void Insert(int val, int cnt){
		update(node, val, cnt, 0, MAXC);
	}
	//��ֵ��[l, r]�ڵ�������ɾȥ������ɾ�������ָ���
	inline int Erase(int l, int r){
		return erase(node, l, r, 0, MAXC);
	}
	//�����������ֵĺ�
	inline int GetSum(){
		return node->sum;
	}
};

//���ݷ����
class DAT{
	int arr[SIZE];	//����ֵ
	int len;		//���С
	int add;		//����Ӽ�ֵ�ı��
	Segtree sumt;	//����һ���ڲ�ͬ��ֵ�ϵĸ������߶���

	//���Ͻṹ�����������Ĳ���������û�취force
	//Ϊ��force�����ĳɹ���ɣ��ؼ�������Ԫ��
	int sup;	//���arr[i] > sup���������sup
	int inf;	//���arr[i] < inf���������inf
	int cover;	//��ȫ����arr[i]���cover��Ĭ��-1

	//ǿ����ֵadd���������߶���
	inline void force(){
		int i;

		//����force���
		if(cover >= 0){
			fill_n(arr, len, cover);
		}else{
			for(i = 0; i < len; i ++){
				if(arr[i] > sup)
					arr[i] = sup;
				if(arr[i] < inf)
					arr[i] = inf;
			}
		}
		sup = 100;
		inf = 0;
		cover = -1;

		//����add���
		for(i = 0; i < len; i ++){
			arr[i] = max(min(arr[i] + add, 100), 0);
		}
		add = 0;
	}
public:
	//��ʼ��
	void Init(int len){
		//��ʼ����ѯ����
		this->len = len;
		add = 0;
		fill_n(arr, len, 0);
		sumt.Build();
		sumt.Insert(0, len);

		//��ʼ��force���
		sup = 100;
		inf = 0;
		cover = -1;
	}
	//�ڲ��Ӽ�����ע��l,r���ڲ����
	void AddInner(int l, int r, int val){
		int i;

		sumt.Erase(0, MAXC);
		force();
		for(i = l; i <= r; i ++){
			arr[i] = max(min(arr[i] + val, 100), 0);
		}
		for(i = 0; i < len; i ++){
			sumt.Insert(arr[i], 1);
		}
	}
	//����Ӽ���
	void AddWhole(int val){
		int del;	//���ƶ�������

		//�Ƚ��в���
		add += val;
		add = max(min(add, 100), - 100);
		if(add > 0){
			del = sumt.Erase(MAXC - add, MAXC);
			sumt.Insert(MAXC - add, del);
		}else if(add < 0){
			del = sumt.Erase(0, - add);
			sumt.Insert(- add, del);
		}

		//�ٴ���force���
		if(cover >= 0){
			if(add > 0){
				cover = min(cover, MAXC - add);
			}else if(add < 0){
				cover = max(cover, - add);
			}
		}else{
			if(add > 0){
				sup = min(sup, MAXC - add);
				if(sup < inf)
					cover = sup;
			}else if(add < 0){
				inf = max(inf, - add);
				if(inf > sup)
					cover = inf;
			}
		}
	}
	//�������
	inline int GetSum(){
		return sumt.GetSum() + add * len;
	}
	//�������
	inline int PartSum(int l, int r){
		int i, ans = 0;
		sumt.Erase(0, MAXC);
		force();
		for(i = l; i <= r; i ++){
			ans += arr[i];
		}
		for(i = 0; i < len; i ++){
			sumt.Insert(arr[i], 1);
		}
		return ans;
	}
};

//��״����
class BlockList{
	DAT dat[MAXB];
	int siz;	//�����С
	int bcnt;	//����
	int N;		//[1..N]
public:
	void Init(int N){
		this->N = N;
		siz = static_cast<int>(floor(sqrt(N)));
		bcnt = N / siz + 1;
		for(int i = 0; i < bcnt; i ++)
			dat[i].Init(siz);
	}
	void Add(int l, int r, int val){
		int bl = l / siz, br = r / siz;
		int pl = l % siz, pr = r % siz;
		if(bl == br){
			dat[bl].AddInner(pl, pr, val);
		}else{
			for(int i = bl + 1; i < br; i ++)
				dat[i].AddWhole(val);
			dat[bl].AddInner(pl, siz - 1, val);
			dat[br].AddInner(0, pr, val);
		}
	}
	int GetSum(int l, int r){
		int bl = l / siz, br = r / siz;
		int pl = l % siz, pr = r % siz;
		if(bl == br){
			return dat[bl].PartSum(pl, pr);
		}else{
			int ans = 0;
			for(int i = bl + 1; i < br; i ++){
				ans += dat[i].GetSum();
			}
			ans += dat[bl].PartSum(pl, siz - 1);
			ans += dat[br].PartSum(0, pr);
			return ans;
		}
	}
}bl;

int main(){
	int N, Q, c, x, y, z;
	while(scanf("%d%d", &N, &Q) != EOF){
		bl.Init(N);
		while(Q --){
			scanf("%d%d%d", &c, &x, &y);
			if(c == 1){
				scanf("%d", &z);
				bl.Add(x, y, z);
			}else if(c == 2){
				scanf("%d", &z);
				bl.Add(x, y, - z);
			}else{
				printf("%d\n", bl.GetSum(x, y));
			}
		}
	}
	return 0;
}