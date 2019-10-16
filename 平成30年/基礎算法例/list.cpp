// 双方向リスト・待ち行列・スタックの実装の例
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
using namespace std;

typedef int Data;

// 双方向連結リスト
class List {
	struct ListNode {
		// 前へのポインタ、次へのポインタ
		ListNode *prev, *next;

		// 値
		Data val;

		ListNode(Data value, ListNode* prev, ListNode* next)
			:val(value), prev(prev), next(next) { }

		ListNode() = default;

		// =====以下のコードは配列によるポインタの実装のみ=====

		// 引数付きのnew演算子のオーバーライド
		// グローバルの空間を使うとholder不要。ここあくまで一例。
		void* operator new(size_t, List* holder) {
			return holder->NewNode();
		}

		// 引数付きのdelete演算子のオーバーライド
		void operator delete(void* p, List* holder) noexcept{
			holder->DeleteNode(reinterpret_cast<ListNode*>(p));
		}

		// ===== =====
	};

	/*
	 * guard == nil。また、このように
	 * l1 --- l2 --- l3 --- ... --- ln
	 *  ^                            ^
	 *  |                            |
	 * guard.next                   guard.prev
	 * リストの首尾を記す
	 */

	ListNode guard;

	size_t size;

	// =====以下のコードは配列によるポインタの実装のみ=====

	// staticと宣言すれば共用できる
	// pointerを用いると実行時size指定できる
	static const size_t POOL_SIZE = 100;

	// 使わっていないリストノードを単方向連結リストで記録
	ListNode pool[POOL_SIZE];

	void InitPool() {
		// 全てのノードは未だ使われてない
		for (int i = 0; i < POOL_SIZE - 1; i++) {
			pool[i].next = &pool[i + 1];
		}
		pool[POOL_SIZE - 1].next = nullptr;
	}

	ListNode* NewNode() {
		ListNode* ret = pool[0].next;
		pool[0].next = ret->next;
		return ret;
	}

	void DeleteNode(ListNode* p) noexcept{
		p->next = pool[0].next;
		pool[0].next = p;
	}
	// ===== =====

public:
	typedef ListNode* Pos;

	List():guard(0, &guard, &guard), size(0) {
		InitPool();
	}

	size_t Size() {
		return size;
	}

	Pos Insert(Pos p, Data v) {
		// ListNode* ret = new ListNode(v, p, p->next);
		ListNode* ret = new(this) ListNode(v, p, p->next);
		// guardあれば深く考えず、以下のようなコード書ける
		ret->next->prev = ret;
		ret->prev->next = ret;
		size++;
		return ret;
	}

	void Delete(Pos p) {
		p->next->prev = p->prev;
		p->prev->next = p->next;
		size--;
		// delete p;
		ListNode::operator delete (p, this);
	}

	Pos Find(int k) {
		Pos ret = guard.next;
		// 0から数える
		if (k >= size) {
			return nullptr;
		}
		for (; k >= 0; k--) {
			ret = ret->next;
		}
		return ret;
	}

	// StackPush == push_front
	List* StackPush(Data v) {
		Insert(&guard, v);
		return this;
	}

	// QueuePush == push_back
	List* QueuePush(Data v) {
		Insert(guard.prev, v);
		return this;
	}

	// Pop == pop_front
	Data Pop() {
		Data ret = guard.next->val;
		Delete(guard.next);
		return ret;
	}
};

int main() {
	List a;

	a.StackPush(1)->StackPush(2)->StackPush(3)->StackPush(4)->StackPush(5);
	while (a.Size() > 0) {
		printf("%d ", a.Pop());
	}
	printf("\n");

	a.QueuePush(1)->QueuePush(2)->QueuePush(3)->QueuePush(4)->QueuePush(5);
	while (a.Size() > 0) {
		printf("%d ", a.Pop());
	}
	printf("\n");

	return 0;
}