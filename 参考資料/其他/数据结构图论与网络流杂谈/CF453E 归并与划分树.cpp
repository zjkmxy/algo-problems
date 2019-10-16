//������ÿ�״����Ҳ������ˮ��
#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 100005;
const int LOGN = 18;

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

struct Pony{
	int s, m, r;
	LL sumr, summ;	//��������һ�ε�r�ͣ���������һ�ε�m��
	int time;	//�仯����ֵ��ʱ��
	inline bool operator<(const Pony& rhs)const{
		return time < rhs.time;
	};
	inline void read(){
		s = ReadInt(), m = ReadInt(), r = ReadInt();
		time = (r == 0) ? 0x3fffffff : m / r + 1;
	};
}pool[MAXN*LOGN];
inline bool operator<(const int& lhs, const Pony& rhs){
	return lhs < rhs.time;
};
int ppos;
inline Pony* alloc(const int cnt){
	Pony* ret = pool + ppos;
	ppos += cnt;
	return ret;
}

class MergeTree{
public:
	struct NODE{
		Pony* arr;	//�鲢�õ�Pony
		int time;	//�������ʱ��
		int l, r;
		NODE *lchd, *rchd;

		//���ϰ�����ʱ��鲢
		inline void popup(int mid){
			int i, len = r - l + 1;
			merge(lchd->arr, lchd->arr + mid - l + 1, rchd->arr, rchd->arr + r - mid, arr);
			arr[0].summ = arr[0].m;
			for(i = 1; i < len; i ++)
				arr[i].summ = arr[i].m + arr[i - 1].summ;
			arr[len - 1].sumr = arr[len - 1].r;
			for(i = len - 2; i >= 0; i --)
				arr[i].sumr = arr[i].r + arr[i + 1].sumr;
		}

		//���Ʊ�ǣ�����ʱ�䣩
		inline void pushdown(){
			if(time > 0){
				lchd->time = time;
				rchd->time = time;
				time = 0;
			}
		}

		LL query(int L, int R, int tim){
			LL ret = 0;
			if(l == r){
				//�������
				if(arr->r == 0){
					ret = arr->s;
				}else{
					if((arr->m -arr->s) / arr->r < tim - time){
						ret = arr->m;
					}else{
						ret = arr->s + arr->r * (tim - time);
					}
				}
				arr->s = 0;
				time = tim;
			}else{
				//ע�⣺�������������ʱ�䲻һ�£�ǿ�����µݹ�
				if(L <= l && r <= R && time > 0){
					int pos = upper_bound(arr, arr + (r - l + 1), tim - time) - arr;
					//����ħ�������
					if(pos > 0)
						ret += arr[pos - 1].summ;
					//����ħ��δ����
					if(pos < (r - l + 1))
						ret += arr[pos].sumr * (tim - time);
				}else{
					int mid = (l + r) >> 1;
					pushdown();
					if(L <= mid)
						ret += lchd->query(L, R, tim);
					if(R > mid)
						ret += rchd->query(L, R, tim);
				}
				time = (L <= l && r <= R) ? tim : 0;
			}
			return ret;
		}
	}node[MAXN<<1];
	int ncnt;
	NODE *build(int l, int r){
		NODE* ret = &node[ncnt ++];
		ret->l = l, ret->r = r;
		ret->arr = alloc(r - l + 1);
		ret->time = 0;
		if(l < r){
			int mid = (l + r) >> 1;
			ret->lchd = build(l, mid);
			ret->rchd = build(mid + 1, r);	
			ret->popup(mid);
		}else{
			ret->lchd = ret->rchd = nullptr;
			ret->arr->read();
		}
		return ret;
	}
public:
	inline void Build(int N){
		build(1, N);
	}
	inline LL Query(int L, int R, int time){
		return node->query(L, R, time);
	}
}tree;

int N, Q;
int main(){
	int t, l, r;

	N = ReadInt();
	tree.Build(N);
	Q = ReadInt();
	while(Q --){
		t = ReadInt(), l = ReadInt(), r = ReadInt();
		printf("%I64d\n", tree.Query(l, r, t));
	}
	return 0;
}