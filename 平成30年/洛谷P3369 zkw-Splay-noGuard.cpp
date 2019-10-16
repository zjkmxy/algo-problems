#include <iostream>
#include <cstdio>
#include <functional>
#include <optional>
using std::function;
using std::optional;
using std::nullopt;

enum Direct : int {
	LEFT = 0,
	RIGHT = 1
};

struct Node {
	int key, size;
	Node* chd[2];
	Node() :key(0), size(0) {
		chd[LEFT] = chd[RIGHT] = this;
	}
	Node(int val, Node* lchd, Node* rchd) : key(val) {
		chd[LEFT] = lchd;
		chd[RIGHT] = rchd;
		size = 1 + lchd->size + rchd->size;
	}
	Node* pop() {
		size = 1 + chd[LEFT]->size + chd[RIGHT]->size;
		return this;
	}
};

struct Splay {
	Node *null;
	Node *root;
	Splay() {
		null = new Node();
		root = null;
	}
	void zig(Direct dir) {
		Node *new_root = root->chd[dir];
		root->chd[dir] = null->chd[dir];
		null->chd[dir] = root;
		root = new_root;
	}
	void zigzig(Direct dir) {
		Node *middle = root->chd[dir];
		Node *new_root = middle->chd[dir];
		middle->chd[dir] = null->chd[dir];
		null->chd[dir] = middle;
		root->chd[dir] = middle->chd[1 ^ dir];
		middle->chd[1 ^ dir] = root->pop();
		root = new_root;
	}
	void finish(Direct dir) {
		// The front node of the reversed temporary tree
		Node *head = null->chd[dir];
		// The new child of head after putting head back
		Node *child = root->chd[1 ^ dir];
		while (head != null) {
			Node *next_head = head->chd[dir];
			head->chd[dir] = child;
			child = head->pop();
			null->chd[dir] = next_head;
			head = next_head;
		}
		root->chd[1 ^ dir] = child;
	}
	void splay(function<optional<Direct>(Node*)> pred) {
		while (true) {
			auto dir1 = pred(root);
			if (!dir1.has_value() || root->chd[dir1.value()] == null) {
				break;
			}
			auto dir2 = pred(root->chd[dir1.value()]);
			if (!dir2.has_value() ||
				root->chd[dir1.value()]->chd[dir2.value()] == null)
			{
				zig(dir1.value());
				break;
			}
			if (dir1.value() == dir2.value()) {
				zigzig(dir1.value());
			}
			else {
				zig(dir1.value());
				zig(dir2.value());
			}
		}
		finish(LEFT);
		finish(RIGHT);
		if (root != null)
			root->pop();
	}
	void select(int k) {
		auto pred = [&k](Node* node)->optional<Direct> {
			int pos = node->chd[LEFT]->size;
			if (k == pos) {
				return nullopt;
			}
			else if (k < pos) {
				return LEFT;
			}
			else {
				k -= pos + 1;
				return RIGHT;
			}
		};
		splay(pred);
	}
	void lowerbound(int x) {
		splay([x](Node* node) {
			return (x <= node->key ? LEFT : RIGHT);
		});
		if (x > root->key && root != null)
			select(root->chd[0]->size + 1);
	}
	void insert(int x) {
		lowerbound(x);
		Node *oldroot = root;
		if (root->key >= x) {
			root = new Node(x, oldroot->chd[0], oldroot);
			oldroot->chd[0] = null;
		}
		else {
			root = new Node(x, oldroot, null);
		}
		if (oldroot != null) {
			oldroot->pop();
		}
		root->pop();
	}
	void remove(int x) {
		lowerbound(x);
		Node *oldroot = root;
		if (root->chd[1] != null) {
			root = root->chd[1];
			select(0);
			root->chd[0] = oldroot->chd[0];
			root->pop();
		}
		else {
			root = root->chd[0];
		}
		delete oldroot;
	}
	int at(int k) {
		select(k);
		return root->key;
	}
	int rank(int x) {
		lowerbound(x);
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
			splay.insert(x);
		}
		else if (opt == 2) {
			splay.remove(x);
		}
		else if (opt == 3) {
			printf("%d\n", splay.rank(x));
		}
		else if (opt == 4) {
			printf("%d\n", splay.at(x - 1));
		}
		else if (opt == 5) {
			splay.lowerbound(x);
			if (splay.root->key >= x) {
				Node* ptr = splay.root->chd[0];
				while (ptr->chd[1] != splay.null) {
					ptr = ptr->chd[1];
				}
				printf("%d\n", ptr->key);
			}
			else {
				printf("%d\n", splay.root->key);
			}
		}
		else if (opt == 6) {
			splay.lowerbound(x + 1);
			if (splay.root->key >= x + 1) {
				printf("%d\n", splay.root->key);
			}
			else {
				printf("NONE\n");
			}
		}
	}

	return 0;
}
