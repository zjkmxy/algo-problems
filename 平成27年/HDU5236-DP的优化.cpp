/*
��Ŀ���⣺
Ҫ������һƪN���ַ������£������зǸ�����i��
ÿ����i+0.1��ʱ��������һ�������ַ�
ÿ����i+0.9��ʱ��P�ĸ��ʱ������ص���ͷ������һ�����̵㣩
ÿ����i����һ�λ������ѡ����X�������̣����߲���
��ӡ����ƪ����֮��������һ�β������
�������N,P,Xѡ����Ѳ���ʹ����������ƪ����ʱ�򰴼���������С�����������

��Ŀ������
1.��F[n]��ʾ��������n���ַ������̵�������������
F[0]=0��������DP���ƿ��Եõ�һ��O(n)�Ĺ�ʽһ·����F[1]..F[n]������
�ر�ע������F�ǵ��������ģ������ǰ�����
����a > b >= c > d���У�F[a] + F[d] > F[b] + F[c]
���������֮���Ż��Ĺؼ�

2.��dp[i]��ʾ���볤��Ϊi�������������С�������������Լ���
dp[0] = 0
dp[i] = min{dp[i-j] + F[j] + X}  j < i
dp[i] = min{F[i] + X, dp[i-j] + dp[j]}  j < i
���������̶�ѡһ����

3.����sΪ��С����Ҫ���̲����ŵĵ㣬��dp[s] < F[s] + X����Ȼs���ڣ����ܱ�N�󣩡�
��ô��������k>=s��һ���У�dp[k] < F[k] + X
֤��
�����裺dp[s] = F[s-s'] + F[s'] + 2X��s >= 2 * s'
dp[k] <= dp[k-s'] + F[s'] + X <= F[k-s'] + F[s'] + 2X
ע�⣺F[k] + F[s-s'] > F[s] + F[k-s']
������ʽ�ã�
dp[k] < F[k] + F[s-s'] + F[s'] - F[s] + 2X = F[k] + dp[s] - F[s] < F[k] + X

4.��s,s'���ϣ���s' = floor(s / 2)
֤���ɰ���������ʽ��dp[s]ȡ������ֵʱs'��(s-s')�������1����˱�����ʽ��

5.dp[n]����չ����m��dp[pi]��ӵĺͣ��������pi��pj������������1����ÿһ����������s
֤��
������Ȼ����pi < pj - 1����pi����1��pj����1��������ţ���dp[n]����ì��
���ĳһ��pi����s��ֱ������dp[s]��ת�Ƶ��dp[pi]����
ע�⵽���������е�pi������pi=s������dp[pi]=F[pi]+X

�㷨��ƣ�
1.���ȼ���s������i<=s��F[i]��Ϊ������F[s]=dp[s]-X�������ı���
2.ͨ��ö��dp[N]չ��Ϊdp[pi]��pi��������dp����ֱ�Ӽ���dp[N]�����������
���㷨ʱ�临�Ӷ�ֻ��P��X�йض���N�޹أ���ʱ��Ч��ΪO(s)��s��С��Ϊ����O(1)��
*/
#include <cstdio>
#include <limits>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXS = 30;  //P = 0.1, X = 100ʱS���Ϊ28���������Sһ�㲻����10
int N, X, S;
double P, F[MAXS];

//����F����O(S)
void CalcF(){
	int i;
	double a = 1, b = 1. / (P - 1.);
	S = numeric_limits<int>::max();
	F[0] = 0.;
	F[1] = - b / a;
	for(i = 2; i <= N && i <= S; i ++){
		a = (a - P) / (1 - P);
		b = (b - 1) / (1 - P);
		F[i] = - b / a;
		if(F[i] > F[i >> 1] + F[i - (i >> 1)] + X){
			F[i] = F[i >> 1] + F[i - (i >> 1)] + X;
			S = i;
		}
	}
}

//��������ⷽʽO(N*S)
double BFSolve(){
	static double dp[MAXN];
	int i, j;
	dp[0] = 0;
	for(i = 1; i <= N; i ++){
		if(i <= S){
			dp[i] = F[i] + X;
		}else{
			dp[i] = numeric_limits<double>::max();
		}
		for(j = 1; j <= S && j < i; j ++){
			dp[i] = min(dp[i], dp[i - j] + dp[j]);
		}
	}
	return dp[N];
}

//�Ľ��ļ��㷽��O(S)
double Solve(){
	int i, a, b;
	double ans = numeric_limits<double>::max();
	if(N < S)
		return F[N] + X;
	for(i = 1; i <= S; i ++){
		a = N / i;
		b = N % i;
		//ע��N,a,b�ȿ�������N = a * i + bҲ��������N = (a + 1) * i - (i - b)
		//���ֲ�ͬ����ⷽʽ�������������ֲ�ͬ�ķ��鷽ʽ
		if(a >= b && i < S){
			//a-b��i����b��(i+1)
			ans = min(ans, (a - b) * F[i] + b * F[i + 1] + a * X);
		}
		if(a + b + 1 >= i){
			//(i-b)��(i-1)����(a+1-(i-b))��i
			ans = min(ans, (a + 1 - i + b) * F[i] + (i - b) * F[i - 1] + (a + 1) * X);
		}
	}
	return ans;
}

int main(){
	int T, i;
	scanf("%d", &T);
	for(i = 1; i <= T; i ++){
		scanf("%d%lf%d", &N, &P, &X);
		CalcF();
		printf("Case #%d: %.6lf\n", i, Solve());
	}
	return 0;
}
