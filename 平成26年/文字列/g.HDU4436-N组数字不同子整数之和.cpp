#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int ALB = 11;
const int MAXN = 200005;
typedef long ll;
const int MODN = 2012;
const int MPN  = 10;
char str[MAXN];
ll sum[MAXN], pw[MAXN], ssum[MAXN], spw[MAXN];
int sharp[MAXN], nshp;
int N, L, ans;

void pre(){
	int i;
	pw[0] = spw[0] = 1;
	for(i = 1; i < MAXN; i ++){
		pw[i] = pw[i - 1] * MPN % MODN;
		spw[i] = (spw[i - 1] + pw[i]) % MODN;
	}
}

inline ll get(int l, int len){
	ll ret = sum[l + len - 1] - sum[l - 1] * pw[len] % MODN;
	return ret < 0 ? ret + MODN : ret;
}

inline ll get2(int l, int len1, int len2){
	ll ret, tmp;
	ret = ssum[l + len2 - 1] - ssum[l + len1 - 2];
	if(ret < 0)
		ret += MODN;
	tmp = spw[len2] - spw[len1 - 1];
	if(tmp < 0)
		tmp += MODN;
	ret -= sum[l - 1] * tmp % MODN;
	if(ret < 0)
		ret += MODN;
	return ret;
}

class SAM{
	struct NODE{
		NODE *par, *go[ALB];
		int val;	//当前状态max的值
		int rv;
		NODE(int val):par(nullptr),val(val){
			fill_n(go, ALB, nullptr);
		}
		NODE(){};
	}node[MAXN<<1], *last;
	int ncnt;
public:
	void Init(){
		ncnt = 1;
		last = node;
		node[0] = NODE(0);
	}
	void Append(int x, int pos){
		NODE *p = last, *np = &node[ncnt ++];
		*np = NODE(p->val + 1);
		np->rv = pos;
		for(; p && !p->go[x]; p = p->par)
			p->go[x] = np;
		if(p){
			NODE *q = p->go[x];
			if(p->val + 1 == q->val){
				np->par = q;
			}else{
				NODE *nq = &node[ncnt ++];
				*nq = *q;
				nq->val = p->val + 1;
				q->par = nq;
				np->par = nq;
				for(; p && p->go[x] == q; p = p->par)
					p->go[x] = nq;
			}
		}else{
			np->par = node;
		}
		last = np;
	}

	void calc(){
		NODE* p;
		int len, mxlen;

		for(p = node + 1; p < node + ncnt; p ++){
			//#,0开头的不统计
			if(str[p->rv] == '#' || str[p->rv] == '0')
				continue;
			len = p->par->val + 1;
			//通过#来计算边界
			//这里其实可以改造成纯O(N)的，懒得写了。log就log吧。
			mxlen = *upper_bound(sharp, sharp + nshp, p->rv) - p->rv;
			mxlen = min(mxlen, p->val);
			if(len > mxlen)
				continue;
			ans = (ans + get2(p->rv, len, mxlen)) % MODN;
		}
	}
}sam;


bool domain(){
	int i;

	if(scanf("%d", &N) == EOF)
		return false;

	ans = 0;
	nshp = 0;
	L = 1;
	str[0] = '#';
	sharp[nshp ++] = 0;
	for(i = 0; i < N; i ++){
		scanf("%s", str + L);
		L += strlen(str + L);
		str[L] = '#';
		sharp[nshp ++] = L;
		str[L + 1] = 0;
		L ++;
	}

	for(i = 1; i < L; i ++){
		if(str[i] == '#')
			sum[i] = 0;
		else
			sum[i] = (sum[i - 1] * MPN + str[i] - '0') % MODN;
		ssum[i] = (ssum[i - 1] + sum[i]) % MODN;
	}

	sam.Init();
	for(i = L - 1; i >= 1; i --){
		if(str[i] == '#')
			sam.Append(10, i);
		else
			sam.Append(str[i] - '0', i);
	}
	sam.calc();

	printf("%d\n", ans);

	return true;
}

int main(){
	pre();
	while(domain());
	return 0;
}
