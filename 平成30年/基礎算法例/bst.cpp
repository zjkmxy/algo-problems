// 二分探索木(BST)
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<functional>
using namespace std;

typedef int Data;

// 平凡な二分探索木
class BST {
public:
	struct Node {
		// ch[0]左の子、ch[1]右の子、par親
		Node *ch[2], *par;

		//値
		Data val;

		//部分木のサイズ
		size_t siz;

		Node() = default;

		Node(Data val, Node* par, Node& nil) :val(val), par(par), siz(1) {
			ch[0] = ch[1] = &nil;
		}

		//方向 == 自分は何番目の子？
		int dir() {
			return this == par->ch[0] ? 0 : 1;
		}
	};

	// nil == 空ノード
	// nil.siz == 0
	// nil.ch[0] == 根
	// nilの他のメンバーは使わない
	Node nil;

	inline Node*& Root() {
		return nil.ch[0];
	}

	BST() {
		nil.par = &nil;
		nil.siz = 0;
		Root() = &nil;
	}

	virtual ~BST() {
		function<void(Node*)> free;
		// 後順走査でノードを削除
		free = [&](Node* rt)->void {
			if (rt != &nil) {
				free(rt->ch[0]);
				free(rt->ch[1]);
				delete rt;
			}
		};
		free(Root());
	}

	// 値＝valのノードの中の一つを取得
	virtual Node* Locate(Data val) {
		Node *p = Root();
		// 子節点left,rightを配列ch[0],ch[1]で書くと相当に書きやすい
		while (p != &nil && p->val != val)
			p = p->ch[val > p->val ? 1 : 0];
		return p;
	}

	// valを挿入
	virtual Node* Insert(Data val) {
		Node **ptr = &Root(), *par = &nil;
		//Locateとほぼ同じが、sizの操作や挿入位置の保存を注意せん
		while (*ptr != &nil) {
			par = *ptr;
			ptr = &(*ptr)->ch[val > (*ptr)->val ? 1 : 0];
			par->siz++;
		}
		*ptr = new Node(val, par, nil);
		return *ptr;
	}

	// K番目のノードを取得
	virtual Node* GetKth(size_t k) {
		Node *p = Root();
		while (p != &nil && p->ch[0]->siz != k - 1) {
			if (p->ch[0]->siz < k - 1) {
				k -= p->ch[0]->siz + 1;
				p = p->ch[1];
			}
			else {
				p = p->ch[0];
			}
		}
		return p;
	}

	// ノードpを削除
	// 簡潔に書く為工夫必要
	virtual void Delete(Node* p) {
		Node* rt = p;
		// dirはより小さい子
		int dir = p->ch[0]->siz >= p->ch[1]->siz ? 1 : 0;
		// もしその子がないなら、そのまま削除できる
		if (p->ch[dir] != &nil) {
			// さもなくば、置き換えられる子孫を探す
			// その子孫はdirの逆方向に一歩出してから、dirへ真っ直ぐ
			// つまり前駆か後継か
			p = p->ch[dir ^ 1];
			while (p->ch[dir] != &nil)
				p = p->ch[dir];
			// そして両者の値を交換して、交代先(p)を削除しようと
			swap(rt->val, p->val);
		}

		// Root == nil->ch[0]なので、判断文要らない
		rt = p->par;
		// 親子関係調整
		rt->ch[p->dir()] = p->ch[dir ^ 1];
		p->ch[dir ^ 1]->par = rt;
		// サイズ調整
		for (; rt != &nil; rt = rt->par)
			rt->siz--;

		delete p;
	}

protected:
	// 木の旋転: dirの逆方向に回転する
	// dir == 0なら左の子を親にする、つまり右回旋。逆もしか
	Node * Rotate(Node* rt, int dir) {
		// 新たな親
		Node* y = rt->ch[dir];
		// 子の関係
		rt->ch[dir] = y->ch[dir ^ 1];
		y->ch[dir ^ 1] = rt;
		rt->par->ch[rt->dir()] = y;

		// 親の関係
		rt->ch[dir]->par = rt;
		y->par = rt->par;
		rt->par = y;

		// サイズ調整
		y->siz = rt->siz;
		rt->siz = rt->ch[0]->siz + rt->ch[1]->siz + 1;
		return y;
	}
};

// 平衡二分探索木の二つの例
// テキストではAVL木を紹介したが、AVL木は実装しにくくて、効率も悪いので、
// 実装例を省略した。ここでAVLと似てるサイズ平衡木と
// 使いやすいスプレー木との例を与える。


// 節点サイズ平衡木(Size-Balanced Tree)
// バランス条件：全てのノードのサイズは、そのノードの甥姪のサイズ以上
// deleteした後何もしなくて、次のinsertを待つだけでもいいので、AVLより書きやすい
class SBT : public BST {
protected:
	// rtの(dir^1)の子と、rtのdirの方向の二つの孫との関係を調整
	Node * Maintain(Node* rt, int dir) {
		if (rt == &nil || rt->ch[dir] == &nil)
			return rt;
		if (rt->ch[dir]->ch[dir]->siz <= rt->ch[dir ^ 1]->siz) {
			if (rt->ch[dir]->ch[dir ^ 1]->siz > rt->ch[dir ^ 1]->siz) {
				// Ｚ文字の時、まず孫の親を回転して一文字にする
				rt->ch[dir] = Rotate(rt->ch[dir], dir ^ 1);
			}
			else
				return rt;
		}
		// 一文字の時、rtを回転
		rt = Rotate(rt, dir);

		// 再帰的の呼出を四回したかのように見えるが、計算量はO(logN)
		rt->ch[0] = Maintain(rt->ch[0], 0);
		rt->ch[1] = Maintain(rt->ch[1], 1);
		rt = Maintain(rt, 0);
		rt = Maintain(rt, 1);
		return rt;
	}

public:
	virtual Node* Insert(Data val) override {
		Node *p, *ret = BST::Insert(val);
		p = ret;
		// Insertした後、根まで一直線にメンテナンス
		while (p->par != &nil) {
			int dir = p->dir();
			p = p->par;
			p = Maintain(p, dir);
		}
		return ret;
	}
};

// スプレー木(SPlay tree)
// 毎回のアクセスした後、そのノードを根にすればいい
// かなり書きやすい且つ早い、なんでもできる万能データ構造
// 唯一の欠点は計算量のO(logN)は償却値
class Splay : public BST {
protected:
	Node * MakeRoot(Node* p) {
		while (p->par != &nil) {
			int d = p->dir();
			// 大事なのは、一文字の時まず祖父を回転すること
			if (p->par->par != &nil && p->par->dir() == d)
				Rotate(p->par->par, d);
			Rotate(p->par, d);
		}
		return p;
	}

public:
	virtual Node* Insert(Data val) override {
		return MakeRoot(BST::Insert(val));
	}

	virtual Node* Locate(Data val) override {
		return MakeRoot(BST::Locate(val));
	}

	virtual Node* GetKth(size_t k) override {
		return MakeRoot(BST::GetKth(k));
	}
};

// 前順走査
void Preorder(BST::Node* rt, BST::Node *nil) {
	if (rt != nil) {
		printf("%d ", rt->val);
		Preorder(rt->ch[0], nil);
		Preorder(rt->ch[1], nil);
	}
}

// 中順走査
void Midorder(BST::Node* rt, BST::Node *nil) {
	if (rt != nil) {
		Midorder(rt->ch[0], nil);
		printf("%d ", rt->val);
		Midorder(rt->ch[1], nil);
	}
}

void PrintTree(BST* t) {
	Preorder(t->Root(), &t->nil);
	printf("\n");
	Midorder(t->Root(), &t->nil);
	printf("\n");
}

int main() {
	BST *t;
	const int N = 10;
	int data[N] = { 1, 5, 3, 2, 4, 9, 8, 6, 7, 10 };

	// 普通のBSTの挿入と削除
	t = new BST();
	for (int i = 0; i < N; i++) {
		t->Insert(data[i]);
	}
	PrintTree(t);
	t->Delete(t->Locate(5));
	PrintTree(t);
	delete t;

	// SBTの挿入　BSTよりバランスがよい
	t = new SBT();
	for (int i = 0; i < N; i++) {
		t->Insert(data[i]);
	}
	PrintTree(t);
	delete t;

	// Splayの挿入
	// 1から10の順で挿入すればSplayは一つのchainになるが
	// 後の操作によってどんどんバランスと取る
	t = new Splay();
	for (int i = 0; i < N; i++) {
		t->Insert(i + 1);
	}
	PrintTree(t);
	t->Delete(t->GetKth(1));
	PrintTree(t);
	t->Locate(3);
	PrintTree(t);
	delete t;

	return 0;
}