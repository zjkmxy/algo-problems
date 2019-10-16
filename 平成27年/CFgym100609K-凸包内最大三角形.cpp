#include <cstdio>
#include <algorithm>
#include <vector>
#include <tuple>

typedef long long ll;
const int MAXN = 5555;
struct point{
	int x, y;

	point() {}
	point(int x, int y): x(x), y(y) {}

	point operator + (const point &p) const {
		return point(x + p.x, y + p.y);
	}

	point operator - (const point &p) const {
		return point(x - p.x, y - p.y);
	}

	ll det(const point &p) const {
		return 1ll * x * p.y - 1ll * p.x * y;
	}
};

typedef point P;
typedef std :: tuple<int, int, int, ll> retVal;

P q[MAXN], t[MAXN];
int id[MAXN];

//三角形面积
inline ll Area(const P& a, const P& b, const P& c)
{
	return (b - a).det(c - a);
}

//比较第三点优劣
inline bool Comp(const P& a, const P& b, const P& p, const P& q){
	return Area(a, b, p) < Area(a, b, q);
}

//下一个点
inline int nxt(int i, int n){
	i ++;
	return (i >= n ? 0 : i);
}

//计算三角形最大面积
retVal cal(int n){
	int i, j, k, kk;
	int jmax, kmax;
	//ijk是三顶点游标，kk是i的第一个对踵点
	ll ans = -1;
	retVal ret;
	for(i = 0, jmax = 1, kmax = 2; i < n; ++ i) {
		//j = nxt(i, n);
		//k = kk;
		//为什么i+1和kk对可以理解，为什么jmax和kmax对解释不清楚，但是就是正确
		j = jmax;
		k = kmax;
		//寻找对踵点对
		while (k != i && Comp(t[i], t[j], t[k], t[nxt(k, n)])) {
			k = nxt(k, n);
		}
		if (k == i)
			continue;
		kk = nxt(k, n);
		//枚举j点
		while( j!= kk && k != i) {
			while( k != i && Comp(t[i], t[j], t[k], t[nxt(k, n)])) {
				k = nxt(k, n);
			}
			ll tmp = Area(t[i], t[j], t[k]);
			if (tmp > ans) {
				ans = tmp;
				ret = std :: make_tuple(i, j, k, ans);
				jmax = j;
				kmax = k;
			}
			j = nxt(j, n);
		}
	}
	return ret;
}

int res[6], res2[6];

int main()
{
#ifndef _DEBUG
	freopen("kids.in", "r", stdin);
	freopen("kids.out", "w", stdout);
#endif
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++ i) {
		scanf("%d%d", &q[i].x, &q[i].y);
	}
	int flag = (q[1] - q[0]).det(q[2] - q[0]) >= 0; //时针顺序反转标识
	if (!flag) {
		std :: reverse(q, q + n);
	}

	//注意这里必须是-1，不然遇到一个正方形的情况就挂了
	//ans是全局最大三角形形成的最优解
	//ans2是和全局最大冲突的小三角形形成的最优解
	ll ans = -1, ans2 = -1;
	retVal ret, ret2;

	//先求出最大的三角形
	std::copy(q, q + n, t);
	ret = cal(n);
	res[0] = std :: get<0>(ret);
	res[1] = std :: get<1>(ret);
	res[2] = std :: get<2>(ret);

	//再求出最小的三角形（当然点重合是另算的）
	//小三角形一定是相邻的三个点形成的
	ll minval = 0x3fffffffffffffff;
	for(int i = 0; i < n; ++ i){
		int pre = i - 1, next = nxt(i, n);
		if(pre < 0)
			pre += n;
		ll small = Area(q[pre], q[i], q[next]);
		if(i == res[0] || i == res[1] || i == res[2]){
			//相交的场合另算一个不相交的最大的三角形，和这一个做差
			int cnt = 0;
			for (int j = 0; j < n; ++ j) {
				if (j != i) {
					t[cnt] = q[j];
					id[cnt ++] = j;
				}
			}
			retVal ret2 = cal(cnt);
			if (std :: get<3>(ret2) - small > ans2) {
				ans2 = std:: get<3>(ret2) - small;
				res2[0] = id[std :: get<0>(ret2)];
				res2[1] = id[std :: get<1>(ret2)];
				res2[2] = id[std :: get<2>(ret2)];
				res2[3] = pre;
				res2[4] = i;
				res2[5] = next;
			}
		}else{
			//不相交的场合，更新最小值
			if(minval > small){
				minval = small;
				res[3] = pre;
				res[4] = i;
				res[5] = next;
			}
		}
	}

	//ans是最大值减去能用的最小值
	ans = std:: get<3>(ret) - minval;
	if(ans < ans2){
		//ans2是和最大的三角形真正冲突的情况下的所有3个可能解中最优的一个
		std::copy(res2, res2 + 6, res);
		ans = ans2;
	}

	if (!flag) {
		for (int i = 0; i < 6; ++ i) {
			res[i] = n - res[i];
		}
	}else{
		//注意这里标号要+1
		for (int i = 0; i < 6; ++ i) {
			res[i] = 1 + res[i];
		}
	}

	if ((ans & 1) == 0)   printf("%I64d.0\n", ans >> 1);
	else    printf("%I64d.5\n", ans >> 1);

	for (int i = 0; i < 3; ++ i) {
		printf("%d", res[i]);
		if (i == 2) printf("\n");
		else    printf(" ");
	}

	for (int i = 0; i < 3; ++ i) {
		printf("%d", res[i + 3]);
		if (i == 2) printf("\n");
		else    printf(" ");
	}
	return 0;
}
