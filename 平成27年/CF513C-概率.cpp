#include <cstdio>
#include <cstdlib>
#include <cstring>

const long MAXN = 10000l;

long L[5], R[5], n;
double prob[MAXN + 1];

inline double calcProbLE(int i, long b){
	if(b < L[i]){
		return 0.;
	}else if(b > R[i]){
		return 1.;
	}else{
		return static_cast<double>(b - L[i] + 1) / static_cast<double>(R[i] - L[i] + 1);
	}
}

inline double calcProbEqu(int i, long b){
	return (b >= L[i] && b <= R[i]) ? 1. / static_cast<double>(R[i] - L[i] + 1) : 0. ;
}

double calcProb(int b){
	int i, j;
	double ret = 0., tmp = 1., tmp2 = 1.;

	// = =
	//题解有误，此处两个应该为乘法
	for(tmp = 1., i = 0; i < n; i ++){
		tmp *= calcProbLE(i, b);
	}
	ret += tmp;
	for(tmp = 1., i = 0; i < n; i ++){
		tmp *= calcProbLE(i, b - 1);
	}
	ret -= tmp;
	for(i = 0; i < n; i ++){
		for(tmp = 1., j = 0; j < n; j ++){
			if(j == i){
				tmp *= calcProbEqu(j, b);
			}else{
				tmp *= calcProbLE(j, b) - calcProbEqu(j, b);
			}
		}
		ret -= tmp;
	}

	// > =
	for(i = 0; i < n; i ++){
		for(tmp = 1., j = 0; j < n; j ++){
			if(j != i){
				tmp *= calcProbLE(j, b);
			}
		}
		for(tmp2 = 1., j = 0; j < n; j ++){
			if(j != i){
				tmp2 *= calcProbLE(j, b - 1);
			}
		}
		ret += (tmp - tmp2) * (1. - calcProbLE(i, b));
	}

	return ret;
}

int main(){
	int i;
	double ret = 0.;

	scanf("%d", &n);
	for(i = 0; i < n; i ++){
		scanf("%d%d", &L[i], &R[i]);
	}
	for(i = 1; i <= MAXN; i ++){
		ret += static_cast<double>(i) * calcProb(i);
	}
	printf("%.9lf\n", ret);
	return 0;
}
