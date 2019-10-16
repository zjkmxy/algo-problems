#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<functional>
#include<vector>
using namespace std;

const int MAXN = 501;

struct EDGE;
struct NODE {
	int id;
	NODE *blos; // us
				/*
				*    B<--->C
				*   /      |
				*  A       |
				*   \      |
				*    D<--->E
				*  <-> 匹配邊  -- 未匹配邊
				*  假設是CE邊走的時候發現了花（A是偶點，CE都是偶）
				*  那麼對B來說，它的blos就是A，bn就是C，bf就是E
				*/
	NODE *bn, *bf; // 成花的匹配點，n是近的，f是遠的
	NODE *par; // tree p
	NODE *mat; // match node 等於自己的是捨棄的點
	int odd; // d
	EDGE *head;
	int lcaFlg;

	NODE() : mat(nullptr), head(nullptr) {}

	// 尋找花的根節點
	NODE* BlsRt() {
		if (blos != this)
			blos = blos->BlsRt();
		return blos;
	}

	// 並查集的合並
	inline void Merge(NODE* b) {
		//b = b->BlsRt();
		//auto tmp = BlsRt();
		//tmp->blos = b;
		//return tmp != b;
		blos = b;
	}

	// 兩個點匹配
	inline void MatchWith(NODE* b) {
		mat = b;
		b->mat = this;
	}
} node[MAXN];
struct EDGE {
	NODE *a, *b;
	EDGE *next;
} edge[MAXN * MAXN];
int N, M, ecnt;
int ans;

struct Queue {
	NODE *q[MAXN];
	NODE **front, **back;

	inline void Clear() {
		front = back = q;
	}

	inline void Push(NODE* x) {
		*back = x;
		back++;
	}

	inline NODE* Pop() {
		front++;
		return *(front - 1);
	}

	inline bool Empty() {
		return back == front;
	}
};

// 暴力尋找cross edge xy的LCA，rt是樹根
NODE* LCA(NODE* x, NODE* y, NODE* rt) {
	while (x != y && x->lcaFlg != 2 && y->lcaFlg != 1) {
		x->lcaFlg = 1;
		y->lcaFlg = 2;
		if (x != rt)
			x = x->par->BlsRt();
		if (y != rt)
			y = y->par->BlsRt();
	}
	// 消除花外有影響的點的flg
	NODE *p = x, *q = y;
	if (q->lcaFlg == 1)
		swap(p, q);
	for (x = p; x != q; x = x->par->BlsRt())
		x->lcaFlg = -1;
	x->lcaFlg = -1;

	return p;
}

// 把n到blos路徑上的點縮到blos的花
void Contract(NODE* blos, NODE* n, NODE* f, Queue& q) {
	for (NODE* x = n->BlsRt(); x != blos; x = x->par->BlsRt()) {
		x->Merge(blos);
		if (x->odd == 1) {
			// 原本的奇數點，現在變得可以是偶數了，重搜
			x->bn = n;
			x->bf = f;
			q.Push(x);
		}
	}
}

// 找到了新的增廣路，從x一直翻轉到rt
// 這個函數是一次走兩步的，所以沒有花的時候會一直是偶點
// 遇到花了就要拆開繞
// 注意順序，一定是先找到完整路徑再翻轉
void FlipBack(NODE* rt, NODE* x) {
	if (rt != x) {
		if (x->odd == 0) {
			// 花外的偶數點
			FlipBack(rt, x->par->par);
			x->par->par->MatchWith(x->par);
		}
		else {
			// 奇數點說明撞到了花上，要反繞
			FlipBack(x->mat, x->bn);
			FlipBack(rt, x->bf);
			x->bn->MatchWith(x->bf);
		}
	}
}

bool BFS(NODE* rt) {
	static Queue q;

	q.Clear();
	for (int i = 0; i < N; i++) {
		node[i].blos = node + i;
		node[i].lcaFlg = -1;
		node[i].odd = -1;
	}
	rt->odd = 0;

	q.Push(rt);
	while (!q.Empty()) {
		NODE* x = q.Pop();
		NODE* xblos = x->BlsRt();
		for (EDGE* j = x->head; j; j = j->next) {
			NODE* y = j->b;
			NODE* yblos = y->BlsRt();
			// 點沒被刪除，且不在同一個花上
			if (y->mat == y || yblos == xblos)
				continue;
			if (y->odd == -1) {
				// 發現新的點，要麼建立樹，要麼增廣
				if (y->mat == nullptr) {
					FlipBack(rt, x);
					x->MatchWith(y);
					return true;
				}
				else {
					y->par = x;
					y->odd = 1;
					y->mat->par = y;
					y->mat->odd = 0;
					q.Push(y->mat);
				}
			}
			else {
				// 已經發現的點，可能是偶數環或者花
				if (yblos->odd == 0) {
					NODE* blos = LCA(xblos, yblos, rt);
					Contract(blos, x, y, q);
					Contract(blos, y, x, q);
				}
			}
		}
	}
	return false;
}


int Match() {
	for (int i = 0; i < N; i++) {
		if (node[i].mat == nullptr) {
			if (BFS(node + i))
				ans++;
			else
				node[i].mat = node + i;
		}
	}
	return ans;
}

void AddEdgeGreedy(NODE* x, NODE* y) {
	EDGE *ret;
	ret = &edge[ecnt++];
	ret->a = x;
	ret->b = y;
	ret->next = x->head;
	x->head = ret;

	ret = &edge[ecnt++];
	ret->a = y;
	ret->b = x;
	ret->next = y->head;
	y->head = ret;

	if (y->mat == nullptr && x->mat == nullptr) {
		x->MatchWith(y);
		ans++;
	}
}

inline char getc() {
	static const int BUFLEN = 1 << 15;
	static char B[BUFLEN], *S = B, *T = B;
	if (S == T) S = B, T = S + fread(B, 1, BUFLEN, stdin);
	return (S == T) ? 0 : *(S++);
}
int ReadInt() {
	char ch; int aa = 0;
	do ch = getc(); while (!isdigit(ch));
	do aa = aa * 10 + ch - '0', ch = getc();
	while (isdigit(ch));
	return aa;
}

int main()
{
	N = ReadInt(), M = ReadInt();

	ans = 0;
	for (int i = 0; i < N; i++) {
		node[i].id = i;
	}

	while (M--) {
		int x, y;
		x = ReadInt(), y = ReadInt();
		x--, y--;
		AddEdgeGreedy(node + x, node + y);
	}

	int res = Match();
	printf("%d\n", res);
	for (int i = 0; i < N; ++i) {
		if (node[i].mat != node + i && node[i].mat != nullptr)
			printf("%d ", node[i].mat->id + 1);
		else
			printf("%d ", 0);
	}
	printf("\n");

	return 0;
}
