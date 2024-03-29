// 集合族の併合
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<functional>
#include<vector>
using namespace std;

// Union-Find Set
// パス圧縮を使えばrankなど使わなくてもいい
struct US {
	// 親節点
	US* p;

	US() :p(this) {}

	// パス圧縮のroot探索
	US* root() {
		if (p != this)
			p = p->root();
		return p;
	}

	// thisの集合とbの集合との併合
	inline bool merge(US* b) {
		b = b->root();
		auto tmp = root();
		p->p = b;
		return tmp != b;
	}
};

// 枝
struct EDGE {
	// 両端と重み
	int x, y, w;
	bool operator<(const EDGE& rhs) const {
		return w < rhs.w;
	}
	EDGE(int x, int y, int w) :x(x), y(y), w(w) {}
};

// ここは最小木のKruskalアルゴリズム
int main() {
	int N, a, b, w;
	US* us;
	vector<EDGE> e;

	scanf("%d", &N);
	us = new US[N];
	while (scanf("%d%d%d", &a, &b, &w) != EOF)
		e.push_back(EDGE(a, b, w));

	sort(e.begin(), e.end());
	int ans = 0;
	for (const EDGE& j : e) 
		ans += us[j.x].merge(&us[j.y]) ? j.w : 0;
	printf("%d\n", ans);

	return 0;
}