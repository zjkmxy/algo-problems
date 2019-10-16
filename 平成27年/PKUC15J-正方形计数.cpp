#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <algorithm>
using namespace std;

typedef long long ll;

//����������ң�����ӣ�
int Scan(){
	int res = 0;
	char ch;
	bool flag = false;

	ch = getchar();
	if(ch <= 0)
		return -1;
	if(ch == '-')
		flag = true;
	else if(isdigit(ch))
		res = ch - '0';
	for(ch = getchar(); isdigit(ch); ch = getchar())
		res = res * 10 + (ch - '0');
	return flag ? -res : res;  
}

//x������ѡ�����������
inline ll c2(const ll x){
	return (x * (x - 1)) >> 1;
}

const int MAXN = 100005;
const int GSIZE = 300;		//�����׼
//��
typedef struct POINT{
	int x, y;
	POINT(int ox = 0, int oy = 0):x(ox),y(oy){}
	bool operator<(const POINT& rhs)const{
		if (x == rhs.x)
			return y < rhs.y;
		else
			return x < rhs.x;
	}
	bool operator==(const POINT& rhs)const{
		return x == rhs.x && y == rhs.y;
	}
	inline void read(){
		x = Scan();
		y = Scan();
	}
} *PPOINT;
POINT pnt[MAXN];
int N;
//��
struct GRP{
	PPOINT st, ed;	//��������յ�
	//���ұ�������û���ض���yֵ
	inline bool find(int y)const{
		return binary_search(st, ed, POINT(st->x, y));
	}
	//������㣺�����������������θ���
	static ll calcBig(const GRP& a, const GRP& b){
		int cnt = 0;	//����乲y�ĵ���
		for(PPOINT p = a.st, q = b.st; p < a.ed; ++ p){
			for(; q < b.ed && q->y < p->y; ++ q);
			if(p->y == q->y)
				++ cnt;
		}
		return c2(cnt);
	}
	//С��������ļ���
	ll smallBig(const GRP& big)const{
		int cnt = 0;
		for(PPOINT p = st; p < ed; ++ p)
			if(big.find(p->y))
				++ cnt;
		return c2(cnt);
	}
};
GRP big[MAXN], small[MAXN];	//��С��
int bg, sg;					//��С����

//С��ļ���
ll calcSmall(){
	static POINT pnt2[MAXN];
	static PPOINT lp[MAXN], prev[MAXN];
	static int cnt[MAXN], n2;
	ll ans = 0;

	//����ʮ������
	n2 = 0;
	for(int i = 0; i < sg; i ++)
		for(PPOINT p = small[i].st; p < small[i].ed; ++ p)
			pnt2[n2 ++] = POINT(p->y, p->x);
	sort(pnt2, pnt2 + n2);
	fill(lp, lp + MAXN, PPOINT(NULL));
	for(int i = 0; i < n2; i ++){
		prev[i] = lp[pnt2[i].y];
		lp[pnt2[i].y] = pnt2 + i;
	}
	for(int i = 0, j; i < n2; i = j){
		//���м���
		for(j = i; j < n2 && pnt2[j].x == pnt2[i].x; j ++){
			for(PPOINT p = prev[j]; p; p = prev[p - pnt2]){
				ans += cnt[p->x];
				cnt[p->x] ++;
			}
		}
		//��������
		for(j = i; j < n2 && pnt2[j].x == pnt2[i].x; j ++){
			for(PPOINT p = prev[j]; p; p = prev[p - pnt2]){
				cnt[p->x] = 0;
			}
		}
	}
	return ans;
}

//������ɢ��
void groupAll(){
	static int yv[MAXN], ycnt;

	for(int i = 0; i < N; i ++)
		yv[i] = pnt[i].y;
	sort(yv, yv + N);
	ycnt = unique(yv, yv + N) - yv;
	for(int i = 0; i < N; i ++)
		pnt[i].y = lower_bound(yv, yv + ycnt, pnt[i].y) - yv;

	sort(pnt, pnt + N);
	bg = sg = 0;
	for(int i = 0, j; i < N; i = j){
		for(j = i; j < N && pnt[j].x == pnt[i].x; ++ j);
		if(j - i > GSIZE){
			big[bg].st = pnt + i;
			big[bg].ed = pnt + j;
			bg ++;
		}else{
			small[sg].st = pnt + i;
			small[sg].ed = pnt + j;
			sg ++;
		}
		for(int q = i; q < j; q ++)
			pnt[q].x = bg + sg;
	}
}

bool domain(){
	ll ans = 0;

	N = Scan();
	if(N == EOF)
		return false;
	for(int i = 0; i < N; i ++)
		pnt[i].read();
	groupAll();
	for(int i = 0; i < bg; i ++)
		for(int j = 0; j < i; j ++)
			ans += GRP::calcBig(big[i], big[j]);
	for(int i = 0; i < sg; i ++)
		for(int j = 0; j < bg; j ++)
			ans += small[i].smallBig(big[j]);
	ans += calcSmall();
	printf("%lld\n", ans);
	return true;
}

int main(){
	while(domain());
	return 0;
}