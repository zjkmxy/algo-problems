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

//���������
inline ll Area(const P& a, const P& b, const P& c)
{
	return (b - a).det(c - a);
}

//�Ƚϵ���������
inline bool Comp(const P& a, const P& b, const P& p, const P& q){
	return Area(a, b, p) < Area(a, b, q);
}

//��һ����
inline int nxt(int i, int n){
	i ++;
	return (i >= n ? 0 : i);
}

//����������������
retVal cal(int n){
	int i, j, k, kk;
	int jmax, kmax;
	//ijk���������α꣬kk��i�ĵ�һ�������
	ll ans = -1;
	retVal ret;
	for(i = 0, jmax = 1, kmax = 2; i < n; ++ i) {
		//j = nxt(i, n);
		//k = kk;
		//Ϊʲôi+1��kk�Կ�����⣬Ϊʲôjmax��kmax�Խ��Ͳ���������Ǿ�����ȷ
		j = jmax;
		k = kmax;
		//Ѱ�Ҷ�����
		while (k != i && Comp(t[i], t[j], t[k], t[nxt(k, n)])) {
			k = nxt(k, n);
		}
		if (k == i)
			continue;
		kk = nxt(k, n);
		//ö��j��
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
	int flag = (q[1] - q[0]).det(q[2] - q[0]) >= 0; //ʱ��˳��ת��ʶ
	if (!flag) {
		std :: reverse(q, q + n);
	}

	//ע�����������-1����Ȼ����һ�������ε�����͹���
	//ans��ȫ������������γɵ����Ž�
	//ans2�Ǻ�ȫ������ͻ��С�������γɵ����Ž�
	ll ans = -1, ans2 = -1;
	retVal ret, ret2;

	//���������������
	std::copy(q, q + n, t);
	ret = cal(n);
	res[0] = std :: get<0>(ret);
	res[1] = std :: get<1>(ret);
	res[2] = std :: get<2>(ret);

	//�������С�������Σ���Ȼ���غ�������ģ�
	//С������һ�������ڵ��������γɵ�
	ll minval = 0x3fffffffffffffff;
	for(int i = 0; i < n; ++ i){
		int pre = i - 1, next = nxt(i, n);
		if(pre < 0)
			pre += n;
		ll small = Area(q[pre], q[i], q[next]);
		if(i == res[0] || i == res[1] || i == res[2]){
			//�ཻ�ĳ�������һ�����ཻ�����������Σ�����һ������
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
			//���ཻ�ĳ��ϣ�������Сֵ
			if(minval > small){
				minval = small;
				res[3] = pre;
				res[4] = i;
				res[5] = next;
			}
		}
	}

	//ans�����ֵ��ȥ���õ���Сֵ
	ans = std:: get<3>(ret) - minval;
	if(ans < ans2){
		//ans2�Ǻ�����������������ͻ������µ�����3�����ܽ������ŵ�һ��
		std::copy(res2, res2 + 6, res);
		ans = ans2;
	}

	if (!flag) {
		for (int i = 0; i < 6; ++ i) {
			res[i] = n - res[i];
		}
	}else{
		//ע��������Ҫ+1
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
