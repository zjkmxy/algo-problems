#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include <ctime>
#include <random>
using namespace std;

default_random_engine re(time(0));
uniform_int_distribution<int> num(0, 1000);
int N = 10;
int W = 20;
int M = 100;
int T = 10;

map<int, float> corr[15];
void domain(){
  printf("%d %d\n", N, M);
  for (int i = 0; i < N; i++){
    corr[i].clear();
  }
  for (int j = 0; j <= N * 3; j++){
    int a = uniform_int_distribution<>(0, N - 1)(re);
    int x = uniform_int_distribution<>(0, N - 1)(re);
    float y = num(re) / 1000.0f;
    if (corr[a].find(x) != corr[a].end()){
      j--;
      continue;
    }
    corr[a][x] = y;
  }
  for (int i = 0; i < N; i++){
    printf("%d", corr[i].size());
    for (auto it = corr[i].begin(); it != corr[i].end(); it++){
      printf(" %d %.3f", it->first + 1, it->second);
    }
    printf("\n");
  }

  float p1 = num(re) / 1000.0f;
  float q1 = num(re) / 1000.0f;

  for (int j = 0; j < M; j++){
    int a = uniform_int_distribution<>(1, N)(re);
    int b = uniform_int_distribution<>(1, N)(re);
    int K = uniform_int_distribution<>(1, N / 3)(re);
    float p = p1;
    float q = q1;
    if (a == b){
      p = 1.0f;
      b = 0;
      q = 0;
    }
    printf("%d %.3f %d %.3f %d\n", a, p, b, q, K);
  }
}

int main(){
  while (T--){
    domain();
  }
  return 0;
}