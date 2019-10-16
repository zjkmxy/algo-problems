//HDU5217
/*
首先建立一颗线段树，每个节点存储两个值R, L，
代表如果仅仅使用这个区间的括号，那么匹配结束之后剩下R个右括号和L个左括号，修改操作很容易实现

对于询问操作，我们首先可以在线段树上询问出消除掉匹配的括号之后，每种括号分别剩下多少个
（显然，最后必定是'))..)(..(('这种形式），那么我们就知道了第K个括号是哪种类型的（或者不足K个括号，输出-1）

容易发现，如果我们从左向右访问线段树，那么')'的数目是不会减少的；
同理，从右向左访问时'('的数量不会减少。如果答案是')'，那么我们从左往右访问，否则从右往左访问。
（为了方便说明，我们假设第K个括号是')'，如果不是的话可以类似处理）
如果处理完一个区间，')'的数量不足K个，那我们继续处理下一个区间。
如果处理完当前区间后')'的数量已经达到K个了，那么我们知道答案必定位于这个区间，
计算一下便可以知道答案位于此区间的左半部分还是右半部分，
然后我们就可以一路深入下去，直到区间中只有一个点，
而这一步往下走的深度最多为线段树的层数
总时间复杂度为O(QlogN)

这里使用张昆玮枚举区间法简化计算
2901MS 当时为第一名
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
		int lp, rp; //左右边界上连续的括号个数
		NODE():lp(0),rp(0){}
	}node[MAXN<<1];
	int base;
	void popup(NODE* cur, NODE* lchd, NODE* rchd){  //合并结果
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
	//深入查找当前段的第k个的位置
	int Search(int cur, int k){
		while(cur < base){
			NODE *lchd = node + (cur << 1), *rchd = node + (cur << 1) + 1;
			if(lchd->rp > rchd->lp){
				if(k <= lchd->lp + lchd->rp - rchd->lp){
					cur <<= 1;
				}else{
					//右孩子中实际的第几个是：k - (lchd->lp + lchd->rp - rchd->lp)
					//但是进入右孩子的时候，不知道前面的一部分已经消去了，所以要加上
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
	//枚举区间，找到合适的段然后深入
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
		//两边枚举
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
					//正数第k个是倒数第N-k+1个
					//根据之前的算法，应该是区间内倒数第(sum - k + 1) - rret.rp + rret.lp个
					//换算成正数似乎应该是下面的式子
					return Search(rp ^ 1, node[rp ^ 1].lp + node[rp ^ 1].rp - ((sum - k + 1) - rret.rp + rret.lp) + 1);
				}
				rret = tmp;
			}
		}
		//跨越中间段
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

