/*
������˼���ǣ�
1 1 1 ... 1 0 0 ... 0 �����Σ�p2�ǲ���ġ���������ڲ���ʤ��p��һ���ġ�
��Ϊp2�ǵ������ģ�����ʤ��p�ǵ������ġ�
Ȼ��������ÿ�����ε�ʤ��p����������λ�ʤ��ͳ��ֵ�����������õġ�
�������������ʤ��ͳ��ֵ�����ˣ��Ѻ�������β���ǰ������Σ���֤ÿ������ʤ��p��������
����ǿ��ǵ�p��p2�ǲ�ͬ�ģ���Ϊp2��ʼһ����0����βһ����1��������alpha��p1�������Ⱥ���λ���ɡ�
-1 0���������alpha==0�������
*/
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 1000005;

//ʤ��
struct PROB{
	int w, n;	//ʤ�����Σ��ܳ���
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
