/*
利用高度截面，用棱台做
维护x,y方向上的线段长度，和随高度的变化率
*/
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAXN = 400000;
struct NODE{
	double h;
	double x, dx;
	double y, dy;
	bool operator<(const NODE& rhs)const{
		return h < rhs.h;
	}
}node[MAXN];
int mx, my;
double x, dx, y, dy, ans;
int nx, ny, cnt;

void Prepare(){
	int i;
	int x0, x1, h0, h1;
	double dh, dk;
	cnt = 0;
	scanf("%d", &nx);
	scanf("%d%d", &x0, &h0);
	mx = h0;
	for(i = 1; i < nx; i ++){
		scanf("%d%d", &x1, &h1);
		mx = max(mx, h1);
		x += x1 - x0;
		dh = h1 - h0;
		if(dh == 0){
			node[cnt].h = h0;
			node[cnt].x += (x0 - x1);
			cnt ++;
		}else{
			dk = (x1 - x0) / dh;
			node[cnt].h = h0;
			node[cnt].dx = - dk;
			cnt ++;
			node[cnt].h = h1;
			node[cnt].dx = + dk;
			cnt ++;
		}
		x0 = x1;
		h0 = h1;
	}

	scanf("%d", &ny);
	scanf("%d%d", &x0, &h0);
	my = h0;
	for(i = 1; i < ny; i ++){
		scanf("%d%d", &x1, &h1);
		my = max(my, h1);
		y += x1 - x0;
		dh = h1 - h0;
		if(dh == 0){
			node[cnt].h = h0;
			node[cnt].y += (x0 - x1);
			cnt ++;
		}else{
			dk = (x1 - x0) / dh;
			node[cnt].h = h0;
			node[cnt].dy = - dk;
			cnt ++;
			node[cnt].h = h1;
			node[cnt].dy = + dk;
			cnt ++;
		}
		x0 = x1;
		h0 = h1;
	}

	sort(node, node + cnt);
}

void Solve(){
	int i;
	double h = 0.;
	double dh;
	ans = 0.;
	for(i = 0; i < cnt; i ++){
		dh = node[i].h - h;
		if(dh > 0)
			dh = dh;
		ans += dh * (x * y + dh * ((x * dy + y * dx) / 2. + dh * dx * dy / 3.));
		x += dh * dx;
		y += dh * dy;
		x += node[i].x;
		y += node[i].y;
		dx += node[i].dx;
		dy += node[i].dy;
		h = node[i].h;
	}
}

int main(){
	freopen("lonely.in", "r", stdin);
	freopen("lonely.out", "w", stdout);
	Prepare();
	if(mx != my){
		printf("Invalid plan\n");
	}else{
		Solve();
		printf("%.18lf\n", ans);
	}
	return 0;
}