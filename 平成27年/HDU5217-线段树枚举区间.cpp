//HDU5217
/*
���Ƚ���һ���߶�����ÿ���ڵ�洢����ֵR, L��
�����������ʹ�������������ţ���ôƥ�����֮��ʣ��R�������ź�L�������ţ��޸Ĳ���������ʵ��

����ѯ�ʲ������������ȿ������߶�����ѯ�ʳ�������ƥ�������֮��ÿ�����ŷֱ�ʣ�¶��ٸ�
����Ȼ�����ض���'))..)(..(('������ʽ������ô���Ǿ�֪���˵�K���������������͵ģ����߲���K�����ţ����-1��

���׷��֣�������Ǵ������ҷ����߶�������ô')'����Ŀ�ǲ�����ٵģ�
ͬ�������������ʱ'('������������١��������')'����ô���Ǵ������ҷ��ʣ��������������ʡ�
��Ϊ�˷���˵�������Ǽ����K��������')'��������ǵĻ��������ƴ���
���������һ�����䣬')'����������K���������Ǽ���������һ�����䡣
��������굱ǰ�����')'�������Ѿ��ﵽK���ˣ���ô����֪���𰸱ض�λ��������䣬
����һ�±����֪����λ�ڴ��������벿�ֻ����Ұ벿�֣�
Ȼ�����ǾͿ���һ·������ȥ��ֱ��������ֻ��һ���㣬
����һ�������ߵ�������Ϊ�߶����Ĳ���
��ʱ�临�Ӷ�ΪO(QlogN)

����ʹ��������ö�����䷨�򻯼���
2901MS ��ʱΪ��һ��
*/
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 262144;
char str[MAXN];
int N, Q;
class TREE{
private:
	struct NODE{
		int lp, rp; //���ұ߽������������Ÿ���
		NODE():lp(0),rp(0){}
	}node[MAXN<<1];
	int base;
	void popup(NODE* cur, NODE* lchd, NODE* rchd){  //�ϲ����
		if(lchd->rp > rchd->lp){
			cur->lp = lchd->lp;
			cur->rp = rchd->rp + lchd->rp - rchd->lp;
		}else{
			cur->rp = rchd->rp;
			cur->lp = lchd->lp + rchd->lp - lchd->rp;
		}
	}
public:
	void Init(int N){
		int i, cnt;
		for(i = 0, N ++; N > 0; i ++)
			N >>= 1;
		for(i ++, cnt = 1; i > 0; i --)
			cnt <<= 1;
		base = cnt >> 1;
		for(i = cnt - 1; i >= base; i --){
			node[i].lp = (str[i - base] == ')') ? 1 : 0;
			node[i].rp = (str[i - base] == '(') ? 1 : 0;
		}
		for(; i > 0; i --){
			popup(node + i, node + (i << 1), node + (i << 1) + 1);
		}
	}
	void Query(int l, int r, NODE& ret){
		NODE lret, rret, tmp;
		for(l += base - 1, r += base + 1; l ^ r ^ 1; l >>= 1, r >>= 1){
			if(!(l & 1)){
				popup(&tmp, &lret, &node[l ^ 1]);
				lret = tmp;
			}
			if(r & 1){
				popup(&tmp, &node[r ^ 1], &rret);
				rret = tmp;
			}
		}
		popup(&ret, &lret, &rret);
	}
	void Update(int pos){
		pos += base;
		swap(node[pos].lp, node[pos].rp);
		for(pos >>= 1; pos; pos >>= 1)
			popup(node + pos, node + (pos << 1), node + (pos << 1) + 1);
	}
	//������ҵ�ǰ�εĵ�k����λ��
	int Search(int cur, int k){
		while(cur < base){
			NODE *lchd = node + (cur << 1), *rchd = node + (cur << 1) + 1;
			if(lchd->rp > rchd->lp){
				if(k <= lchd->lp + lchd->rp - rchd->lp){
					cur <<= 1;
				}else{
					//�Һ�����ʵ�ʵĵڼ����ǣ�k - (lchd->lp + lchd->rp - rchd->lp)
					//���ǽ����Һ��ӵ�ʱ�򣬲�֪��ǰ���һ�����Ѿ���ȥ�ˣ�����Ҫ����
					cur = (cur << 1) + 1;
					k = k - (lchd->lp + lchd->rp - rchd->lp) + rchd->lp;
				}
			}else{
				if(k <= lchd->lp){
					cur <<= 1;
				}else{
					cur = (cur << 1) + 1;
					k = k - lchd->lp + lchd->rp;
				}
			}
		}
		return cur - base;
	}
public:
	//ö�����䣬�ҵ����ʵĶ�Ȼ������
	int DoQuery(int l, int r, int k){
		NODE ret, lret, rret, tmp;
		int lp, rp, sum, p;
		if(l == r){
			return k == 1 ? l : -1;
		}
		Query(l, r, ret);
		sum = ret.lp + ret.rp;
		if(k > sum)
			return -1;
		//����ö��
		for(p = 0, lp = l + base - 1, rp = r + base + 1; lp ^ rp ^ 1; lp >>= 1, rp >>= 1, p ++){
			if(!(lp & 1)){
				popup(&tmp, &lret, &node[lp ^ 1]);
				if(tmp.lp >= k){
					return Search(lp ^ 1, k - lret.lp + lret.rp);
				}
				lret = tmp;
			}
			if(rp & 1){
				popup(&tmp, &node[rp ^ 1], &rret);
				if(tmp.rp > sum - k){
					//������k���ǵ�����N-k+1��
					//����֮ǰ���㷨��Ӧ���������ڵ�����(sum - k + 1) - rret.rp + rret.lp��
					//����������ƺ�Ӧ���������ʽ��
					return Search(rp ^ 1, node[rp ^ 1].lp + node[rp ^ 1].rp - ((sum - k + 1) - rret.rp + rret.lp) + 1);
				}
				rret = tmp;
			}
		}
		//��Խ�м��
		l += base - 1;
		r += base + 1;
		for(p --, lp = (l >> p), rp = (r >> p); p >= 0; p--, lp = (l >> p), rp = (r >> p)){
			if(!(lp & 1)){
				popup(&tmp, &node[lp ^ 1], &rret);
				if(tmp.rp > sum - k){
					return Search(lp ^ 1, node[lp ^ 1].lp + node[lp ^ 1].rp - ((sum - k + 1) - rret.rp + rret.lp) + 1);
				}
				rret = tmp;
			}
			if(rp & 1){
				popup(&tmp, &lret, &node[rp ^ 1]);
				if(tmp.lp >= k){
					return Search(rp ^ 1, k - lret.lp + lret.rp);
				}
				lret = tmp;
			}
		}
		return -1;
	}
}tree;

void domain(){
	int op, l, r, k;
	scanf("%d%d", &N, &Q);
	scanf("%s", str + 1);
	tree.Init(N);
	while(Q --){
		scanf("%d%d", &op, &l);
		if(op == 1){
			tree.Update(l);
		}else{
			scanf("%d%d", &r, &k);
			printf("%d\n", tree.DoQuery(l, r, k));
		}
	}
}

int main(){
	int t;
	scanf("%d", &t);
	while(t --)
		domain();
	return 0;
}

