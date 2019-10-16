/*
代码不改了，简单写下思路

首先，如果只有1个人去地铁，可以证明贪心是足够的。（和单地铁多公交的题不同）
然而两个人的时候，如第三个用例，贪心跪的原因在于，同一天的2个人要用2张不同的票。
也就是说，在两个人的时候，我们可能回提前买票，留下之前的票，而不是用完所有的次数后再买，供两个人同时用。
3 2 3
1 1 1
0 0 1
第一天买1张，第二天买1张。
于是我们可以把买票当天之后连续B天的需求量状压下来：
1 1 2
然后我们的这张票一定会尽量用完A次的，但是用A次的时候方式有所不同。
我们可以在这B天任意安排：给一个1用，或者把一个2降到1
但是不管哪种操作，都一定是同类操作中最靠前的若干个动手
于是我们枚举降2的个数S，这一次买票可以把前(A-S)个1消去，然后把前S个2降为1，作为下一轮的状态。
状态的总数不多，用map存一下判重，然后暴搜即可。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
#include<cassert>
#include<unordered_map>
#include<iomanip>
using namespace std;

typedef long long ll;

const int MAXN = 300;
int N;
int V[MAXN];

int A, B; // number of passages, length

unordered_map<ll, int> dist;
queue<ll> q;

ll canonize(int n, int m1, int m2) {
	while(n < N && !((m1 & 1) || (m2 & 1))) {
		if(V[n] == 1) {
			m1 += (1 << B);
		} else if(V[n] == 2) {
			m2 += (1 << B);
		}
		n++;
		m1 >>= 1;
		m2 >>= 1;
	}
	assert(!(m1 & m2));
	return (ll(n) << B | m1) << B | m2;
}

void insert(int n, int m1, int m2, int d) {
	assert(!(m1 & m2));
	ll s = canonize(n, m1, m2);
	if(dist.count(s)) return;
	dist[s] = d;
	q.push(s);
}

int suff[1 << 20][25];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> A >> B;
	for(int s = 0; s < 2; s++) {
		for(int i = 0; i < N; i++) {
			int v; cin >> v; V[i] += v;
		}
	}

	for(int m = 0; m < (1 << B); m++) {
		for(int i = 0, v = m; v; i++, v &= (v - 1)) {
			suff[m][i] = v;
		}
	}

	N += B;

	insert(0, 0, 0, 0);

	while(true) {
		ll v = q.front();
		int d = dist[v];

		int m2 = v & ((1 << B) - 1);
		v >>= B;
		int m1 = v & ((1 << B) - 1);
		v >>= B;
		int n = v;

		//cerr << n << ' ' << m1 << ' ' << m2 << ' ' << d << '\n';

		if(n == N) {
			break;
		}

		q.pop();

		d++;

		assert(!(m1 & m2));
		assert(1 & (m1 | m2));

		for(int i = 0; i <= A; i++) {
			int nm2 = suff[m2][A - i];
			int nm1 = suff[m1][i] | (m2 ^ nm2);
			insert(n, nm1, nm2, d);
		}
	}

	cout << dist[q.front()] << '\n';

	return 0;
}