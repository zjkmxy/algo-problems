/*
基本的思想是：
1 1 1 ... 1 0 0 ... 0 的区段，p2是不变的。因此在其内部，胜率p是一定的。
因为p2是单调增的，所以胜率p是单调增的。
然后我们让每个区段的胜率p等于这个区段获胜的统计值，结果就是最好的。
如果有两个区段胜利统计值减少了，把后面的区段并入前面的区段，保证每个区段胜率p单调增。
最后是考虑到p和p2是不同的，因为p2开始一定是0，结尾一定是1。所以用alpha和p1调整满度和零位即可。
-1 0的情况就是alpha==0的情况。
*/
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 1000005;

//胜率
struct PROB{
	int w, n;	//胜利场次，总场次
	PROB(int w, int n):w(w),n(n){};
	PROB():w(0),n(0){}
	bool operator<=(const PROB& rhs)const{
		return ll(w) * ll(rhs.n) <= ll(n) * ll(rhs.w);
	};
}p[MAXN];

int main(){
	int a, i, m, n;
	scanf("%d", &n);
	for(m = 0, i = 0; i < n; i ++){
		scanf("%d", &a);
		if(m == 0 || (a == 1 && p[m-1].n > p[m-1].w))
			m ++;
		p[m-1].n ++;
		p[m-1].w += a;
	}
	for(n = 0, i = 0; i < m; i ++){
		while(n > 0 && p[i] <= p[n-1]){
			p[i].w += p[n-1].w;
			p[i].n += p[n-1].n;
			n --;
		}
		p[n ++] = p[i];
	}
	if(p[0].w == 0 && p[n-1].w == p[n-1].n){
		printf("-1 0\n");
	}else{
		double pn = static_cast<double>(p[n-1].w) / p[n-1].n;
		double p1 = static_cast<double>(p[0].w) / p[0].n;
		double alpha = 1. - (pn - p1) / 1.0;
		p1 /= alpha;
		printf("%.10lf %.10lf\n", p1, alpha);
	}
	return 0;
}
