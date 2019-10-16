/*
��Ŀ��Ҫ��nXm������ÿ����Ⱦcɫ��Ҫ�������������������в���ȫͬ���󷽰�����

��n��ÿһ�п�����һ��m��Ԫ�ص��������൱�ڶ��������м��ϼ�������������Ҫ���в�ͬ
��ѡ��ȫ��Ϊc^m�ֲ�ͬ��������

˼·������
1.Stirling����
���ȿ������û��m��������ͬ�����ƣ����ǵķ���������A[c^m,n]��������ԭ���Ĵ���f(m)��
���Ƕ��н��з��飨���ֵȼ��ࣩ���õȼ����ڲ���ȣ���ͬ�ȼ���֮�䲻�ȡ�
���ֳ�i���ȼ������������S(m,i)������i����ͬʱ�Ĵ�f(i)�õ���
A[c^m][n] = sum(i=1..m){ S(m,i) * f(i) }
��Stirling���ݵõ���
f(m) = sum(i=1..m){ s(m,i) * A[c^i][n] }  s(n,k)�ǵ�һ��Stirling��

2.���ϼ���
ȫ����ѡ��k��Ԫ�صļ��ϣ���ѡ��k��Ԫ�ص�������ͬ�����У�ֻ��һ��k!ϵ�������ǰ����ǿ�������ͬ�Ķ�����
������뷨��ö��ÿһ��(i,j)��ʾ������Ai==AjȻ�����ݳ⡣
��һ�����ǵ�ÿ��ö�����ɶ���ȹ�ϵʵ�����Ƕ����л����˵ȼ��࣬��������ֱ��ö�ٵȼ��ࡣ
ÿһ���ȼ����Ӧ�ݳ�Ķ��ַ��������dyh���ġ�
���ǵ�������ÿһ���ȼ���Ĵ�С�������أ���Ӱ��Ľ����ǵȼ���ĸ�����
����ֱ��ö�ٵȼ�����������ǵ�һ��Stirling�����õ��𰸡�

3.DP����
���ᡣ���˱��˴���Ҳû������

��Burnside���������
�����m��ȫ���û���Ϊ�û�Ⱥ����Burnside����õ�
g(m)=sum(i=1..m){ |s(m,i)| * A[c^i][n] } / m!
��f(m)�ı��ʽֻ�з��Ų�ͬ
ע��f(m)��ʾ������ȫ�����Ǻ�ȵģ��任���������ȵķ�������
��g(m)��ʾ������ȫ���任�²������ͬ�ķ�������
�������û���ձ��ԡ�
*/
#include <cstdio>
#include <cmath>
using namespace std;

typedef long long ll;
const int MAXN = 4001;
const ll MODN = 1000000007;
class CountTables{
public:
	ll s[MAXN][MAXN]; //��һ��Stirling��
	//s[n,k]
	CountTables(){
		int n, k;
		s[1][1] = 1;
		for(n = 2; n < MAXN; n ++){
			for(k = 1; k <= n; k ++){
				s[n][k] = (s[n - 1][k - 1] - (n - 1) * s[n - 1][k]) % MODN;
				if(s[n][k] < 0)
					s[n][k] += MODN;
			}
		}
	}
	//A[n,k]
	ll aray(int n, int k){
		ll ans = 1;
		if(n < k) n += MODN;
		for(; k > 0; n --, k --)
			ans = ans * n % MODN;
		return ans;
	}
public:
	int howMany(int N, int M, int C){
		ll way = 1, ans = 0;
		int i;

		for(i = 1; i <= M; i ++){
			way = way * C % MODN;
			ans = (ans + s[M][i] * aray(way, N)) % MODN;
		}
		return ans % MODN;
	}
};


////////////////////

CountTables ct;

int main(){
	int n, m, c, ans, my;
	while(scanf("%d%d%d%d", &n, &m, &c, &ans) != EOF){
		my = ct.howMany(n, m, c);
		if(my == ans){
			puts("OK");
		}else{
			printf("%d expected but %d found.\n", ans, my);
		}
	}
	return 0;
}
