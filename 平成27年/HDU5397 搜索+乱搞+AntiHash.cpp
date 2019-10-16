#include <cstdio>
#include <cctype>
#include <algorithm>
#include <queue>
using namespace std;

const int MAXN = 10002;
typedef long long ll;
ll P, Q, R, base;
ll v1, v2;
int coe[MAXN], len;
struct NODE{
	int l, r;	//同相子树，反相子树
	int pos;	//叶子节点的位置
}node[MAXN];
int N;

struct OP{
	int p;
	ll v;
	bool operator<(const OP& rhs)const{
		if(v != rhs.v)
			return v < rhs.v;
		else
			return p > rhs.p;
	};
	OP(){};
	OP(int p, ll v):p(p), v(v){};
};
priority_queue<OP> q;

#ifdef _MSC_VER
inline ll ml(ll a,ll b){
	ll ret = 0, t;
	for(; b; b >>= 1){
		if(b & 1){
			t = ret + a;
			if(t >= R || t < ret)
				ret = t - R;
			else
				ret = t;
		}
		t = a + a;
		if(t >= R || t < a)
			a = t - R;
		else
			a = t;
	}
	return ret;
}
#else
inline ll ml(ll a,ll b){
	ll ret;
	__asm__ __volatile__ ("\timulq %%rbx\n\tidivq %%rcx\n" :"=d"(ret):"a"(a),"b"(b),"c"(R));
	return ret;
}
#endif

void build(){
	int i;
	ll tmp = ml(base, base);
	OP cur;
	tmp = ml(tmp, tmp);

	//(())
	v1 = Q;
	v1 = ml(v1, base);
	v1 += Q;
	if(v1 > R)
		v1 -= R;
	v1 = ml(v1, base);
	v1 += P;
	if(v1 > R)
		v1 -= R;
	v1 = ml(v1, base);
	v1 += P;
	if(v1 > R)
		v1 -= R;

	//()()
	v2 = Q;
	v2 = ml(v2, base);
	v2 += P;
	if(v2 > R)
		v2 -= R;
	v2 = ml(v2, base);
	v2 += Q;
	if(v2 > R)
		v2 -= R;
	v2 = ml(v2, base);
	v2 += P;
	if(v2 > R)
		v2 -= R;

	//构图入队
	N = 1;
	while(q.size() > 0)
		q.pop();
	for(i = 0; i < 2500; i ++){
		coe[i] = 0;
		node[N].l = 0;
		node[N].pos = i + 1;
		if(v1 >= v2){
			node[N].r = 1;
			cur = OP(N, v1 - v2);
		}else{
			node[N].r = -1;
			cur = OP(N, v2 - v1);
		}
		v1 = ml(v1, tmp);
		v2 = ml(v2, tmp);
		q.push(cur);
		N ++;
	}
	len = 0;
}

void dfs(int rt, bool rev){
	if(node[rt].pos != 0){
		coe[node[rt].pos - 1] = rev ? - node[rt].r : node[rt].r;
		len = max(len, node[rt].pos);
	}else{
		dfs(node[rt].l, rev);
		dfs(node[rt].r, !rev);
	}
}

void run(){
	OP c1, c2, cur;

	while(q.size() > 1){
		c1 = q.top();
		q.pop();
		c2 = q.top();
		q.pop();
		if(c1.v >= c2.v){
			node[N].l = c1.p;
			node[N].r = c2.p;
			cur.v = c1.v - c2.v;
		}else{
			node[N].l = c2.p;
			node[N].r = c1.p;
			cur.v = c2.v - c1.v;
		}
		cur.p = N ++;
		q.push(cur);
		if(cur.v == 0)
			break;
	}
	dfs(cur.p, false);
}

void print(){
	int i;
	for(i = 0; i < len; i ++){
		if(coe[i] == 1){
			printf("(())");
		}else{
			printf("()()");
		}
	}
	printf("\n");
	for(i = 0; i < len; i ++){
		if(coe[i] == -1){
			printf("(())");
		}else{
			printf("()()");
		}
	}
	printf("\n");
	if(len == 0)
		len = *(int*)NULL;
}

bool domain(){
	if(scanf("%I64d%I64d%I64d%I64d", &P, &Q, &R, &base) == EOF)
		return false;
	P %= R, Q %= R, base %= R;
	build();
	run();
	print();
	fflush(stdout);
	return true;
}

int main(){
	while(domain());
	return 0;
}
