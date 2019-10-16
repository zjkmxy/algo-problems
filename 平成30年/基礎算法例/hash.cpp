// ハッシュ
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<string>
#include<algorithm>
#include<iostream>
#include<memory>
using namespace std;

typedef unsigned long ul;

struct StrHash {
	int k, v;
	StrHash(int k = -1, int v = 0) :k(k), v(v) {}
	bool operator==(const StrHash& rhs) const {
		return k == rhs.k && v == rhs.v;
	}
};

// 内部ハッシュ
struct Hash {
	int siz, step;

	unique_ptr<StrHash[]> arr;

	Hash(int size, int step) :siz(size), step(step), arr(new StrHash[size]) {}

	// dataを挿入し、重複するかを出力
	bool Insert(const StrHash& data) {
		int y = data.k % siz;
		while (arr[y].k >= 0 && !(arr[y] == data)) {
			y += step;
			if (y >= siz)
				y -= siz;
		}
		if (arr[y].k >= 0) {
			return false;
		}
		else {
			arr[y] = data;
			return true;
		}
	}

	//内部ハッシュは普通に要素を削除はしない
};

// 文字列のハッシュ
// 二つのセットを使うと衝突確率は更に低い
StrHash calcHashVal(string str) {
	static const long long mul1 = 127;
	static const long long mul2 = 131;
	static const int mod1 = 1000000007;
	static const int mod2 = 1000000009;
	StrHash ret(0, 0);
	for (char ch : str) {
		ret.k = (ret.k * mul1 + static_cast<long long>(ch)) % mod1;
		ret.v = (ret.v * mul2 + static_cast<long long>(ch)) % mod2;
	}
	return ret;
}

// 入力の文字列は初めて出るかどうかを出力するプログラム
int main() {
	Hash hash(239567, 1302);
	string str;

	ios_base::sync_with_stdio(false);
	while (cin >> str) {
		cout << (hash.Insert(calcHashVal(str)) ? "Not Exist" : "Exist") << endl;
	}

	return 0;
}
