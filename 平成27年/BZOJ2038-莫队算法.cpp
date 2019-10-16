#include <cstdio>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <string>
#include <map>
using namespace std;

const int MAXN = 50005;
typedef long long ll;
ll cnt[MAXN], ans;
int arr[MAXN];
int N, M, W;
int L, R;
ll gcd(ll a, ll b){
	if(b == 0)
		return a;
	else
		return gcd(b, a % b);
}
struct SEG{
	int id, l, r, pos;
	ll sumq;
	bool operator<(const SEG& rhs)const{
		if(pos != rhs.pos)
			return pos < rhs.pos;
		else
			return r < rhs.r;
	}
	void print()const{
		ll len = r - l + 1;
		ll ansb = len * (len - 1);
		ll ansa = sumq - len;
		ll k = gcd(ansa, ansb);
		if(ansb > 0)
			printf("%lld/%lld\n", ansa / k, ansb / k);
		else
			printf("1/0\n");
	}
}query[MAXN];
int comp(const void* A, const void* B){
	return ((SEG*)A)->id - ((SEG*)B)->id;
}
void update(int pos, ll val){
	ans -= cnt[pos] * cnt[pos];
	cnt[pos] += val;
	ans += cnt[pos] * cnt[pos];
}
void input(){
	scanf("%d%d", &N, &M);
	for(int i = 1; i <= N; i ++){
		scanf("%d", &arr[i]);
	}
	W = static_cast<int>(sqrt(N));
	for(int i = 0; i < M; i ++){
		query[i].id = i;
		scanf("%d%d", &query[i].l, &query[i].r);
		query[i].pos = query[i].l / W;
	}
}
void deal(SEG &q){
	for(; R < q.r; R ++)
		update(arr[R + 1], 1);
	for(; R > q.r; R --)
		update(arr[R], -1);
	for(; L < q.l; L ++)
		update(arr[L], -1);
	for(; L > q.l; L --)
		update(arr[L - 1], 1);
	q.sumq = ans;
}
int main(){
	input();
	L = 1;
	R = 0;
	sort(query, query + M);
	for(int i = 0; i < M; i ++){
		deal(query[i]);
	}
	qsort(query, M, sizeof(SEG), comp);
	for(int i = 0; i < M; i ++){
		query[i].print();
	}
	return 0;
}
