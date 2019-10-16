#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <random>
using namespace std;

int T = 200;
const int N = 50;
const int C = 16;
const int V = 1000;
const int M = 10000;
default_random_engine re(GetTickCount());
uniform_int_distribution<> uv(1, V);
uniform_int_distribution<> um(1, M);
uniform_int_distribution<> uc(1, C);

int main(){
	int i;
	freopen("dat.in", "w", stdout);
	printf("%d\n", T);
	while(T --){
		printf("\n%d %d\n", N, um(re));
		for(i = 1; i <= N; i ++){
			printf("%d %d %d\n", uc(re), uv(re), uv(re));
		}
	}
	return 0;
}
