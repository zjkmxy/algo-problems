#include <iostream>
#include <cstdio>

struct Node {
	int key, size;
	Node* chd[2];
	Node() :key(0), size(0) {
		chd[0] = chd[1] = this;
	}
	Node(int val, Node* lchd, Node* rchd) : key(val)
	{
		chd[0] = lchd;
		chd[1] = rchd;
		size = 1 + lchd->size + rchd->size;
	}
	Node* pop() {
		size = 1 + chd[0]->size + chd[1]->size;
		return this;
	}
};

struct Splay {
	Node *null; // 空白節點，左一條線和右一條線的孩子都是反轉的。
	Node *root; // 開始的時候root是無限節點。無限可能成爲根，所以這個不能有
	Splay() {
		null = new Node();
		root = (new Node(*null))->pop();
		root->key = (1LL << 31) - 1;
	}
	void zig(bool w) {
		Node *t = root->chd[w];
		root->chd[w] = null->chd[w];
		null->chd[w] = root;
		root = t;
	}
	void zigzig(bool w) {
		Node *t = root->chd[w]->chd[w];
		root->chd[w]->chd[w] = null->chd[w];
		null->chd[w] = root->chd[w];
		root->chd[w] = null->chd[w]->chd[!w];
		null->chd[w]->chd[!w] = root->pop();
		root = t;
	}
	void finish(bool w) {
		Node *t = null->chd[w], *p = root->chd[!w];
		while (t != null) {
			t = null->chd[w]->chd[w];
			null->chd[w]->chd[w] = p;
			p = null->chd[w]->pop();
			null->chd[w] = t;
		}
		root->chd[!w] = p;
	}
	void select(int k) {
		int t;
		for (;;) {
			bool w = k > (t = root->chd[0]->size);
			if (k == t || root->chd[w] == null)
				break;
			if (w)
				k -= t + 1;
			bool ww = k > (t = root->chd[w]->chd[0]->size);
			if (k == t || root->chd[w]->chd[ww] == null) {
				zig(w);
				break;
			}
			if (ww)
				k -= t + 1;
			if (w != ww) {
				zig(w);
				zig(ww);
			}
			else {
				zigzig(w);
			}
		}
		finish(0);
		finish(1);
		root->pop();
	}
	void search(int x) {
		for (;;) {
			bool w = x > root->key;
			if (root->chd[w] == null)
				break;
			bool ww = x > root->chd[w]->key;
			if (root->chd[w]->chd[ww] == null) {
				zig(w);
				break;
			}
			if (w != ww) {
				zig(w);
				zig(ww);
			}
			else {
				zigzig(w);
			}
		}
		finish(0);
		finish(1);
		root->pop();
		if (x > root->key)
			select(root->chd[0]->size + 1);
	}
	void ins(int x) {
		search(x);
		Node *oldroot = root;
		root = new Node(x, oldroot->chd[0], oldroot);
		oldroot->chd[0] = null;
		oldroot->pop();
		root->pop();
	}
	void del(int x) {
		search(x);
		Node *oldroot = root;
		root = root->chd[1];
		select(0);
		root->chd[0] = oldroot->chd[0];
		root->pop();
		delete oldroot;
	}
	int sel(int k) {
		select(k - 1);
		return root->key;
	}
	int ran(int x) {
		search(x);
		return root->chd[0]->size + 1;
	}
};

Splay splay;

int main()
{
	int n, opt, x;
	scanf("%d", &n);
	while (n--) {
		scanf("%d%d", &opt, &x);
		if (opt == 1) {
			splay.ins(x);
		}
		else if (opt == 2) {
			splay.del(x);
		}
		else if (opt == 3) {
			printf("%d\n", splay.ran(x));
		}
		else if (opt == 4) {
			printf("%d\n", splay.sel(x));
		}
		else if (opt == 5) {
			splay.search(x);
			Node* ptr = splay.root->chd[0];
			while (ptr->chd[1] != splay.null) {
				ptr = ptr->chd[1];
			}
			printf("%d\n",ptr->key);
		}
		else if (opt == 6) {
			splay.search(x + 1);
			printf("%d\n", splay.root->key);
		}
	}

	return 0;
}
