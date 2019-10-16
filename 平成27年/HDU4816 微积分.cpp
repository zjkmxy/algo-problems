#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

typedef double dbl;
const int MAXN = 200005;
struct NODE{
	int X, Y;
	dbl k;
}ar[MAXN];
int N, L, D;
dbl ans;

inline dbl LadArea(dbl x0, dbl x1, dbl y0, dbl y1){
	return (y1 + y0) * (x1 - x0) / 2.0;
}

void deal(){
	int l, r;
	dbl cur, yl, yr, xl, xr;	//当前窗口
	dbl best, gomax;

	//内含部分面积
	l = r = 1;
	cur = 0.;
	for(r = 2; ar[r].X < D * 2; r ++){
		cur += LadArea(ar[r - 1].X, ar[r].X, ar[r - 1].Y, ar[r].Y);
	}
	yl = ar[1].Y;
	yr = ar[r - 1].Y + ar[r].k * (D * 2. - ar[r - 1].X);
	xl = 0;
	xr = D * 2.;
	cur += LadArea(ar[r - 1].X, xr, ar[r - 1].Y, yr);
	ans = max(ans, cur);

	//滑动窗口枚举
	for(; r <= N; ){
		//最远滑动距离
		gomax = min(ar[l + 1].X - xl, ar[r].X - xr);
		//最优中分点（滑动距离）
		best = - (yr - yl) / double(ar[r].k - ar[l + 1].k);
		if(best <= gomax && best >= 0){
			ans = max(ans, cur + 0.5 * (ar[r].k - ar[l + 1].k) * best * best + (yr - yl) * best);
		}

		//走到最远
		cur += 0.5 * (ar[r].k - ar[l + 1].k) * gomax * gomax + (yr - yl) * gomax;
		ans = max(ans, cur);
		//计算移动窗
		if(ar[l + 1].X - xl < ar[r].X - xr){
			l ++;
			xl = ar[l].X;
			yl = ar[l].Y;
			xr += gomax;
			yr += ar[r].k * gomax;
		}else{
			xl += gomax;
			yl += ar[l + 1].k * gomax;
			xr = ar[r].X;
			yr = ar[r].Y;
			r ++;
		}
	}

	ans = ans / D / 2.;
}

void domain(){
	int i;

	scanf("%d%d", &N, &L);
	ans = 0.;
	for(i = 1; i <= N; i ++){
		scanf("%d%d", &ar[i].X, &ar[i].Y);
		if(i > 1)
			ar[i].k = (ar[i].Y - ar[i - 1].Y) / dbl(ar[i].X - ar[i - 1].X);
	}
	scanf("%d", &D);
	deal();

	printf("%.3lf\n", ans);
}

int main(){
	int T;

	scanf("%d", &T);
	while(T --){
		domain();
	}
	return 0;
}
