/*
题意理解题。
题目考的是纳什均衡，但是因为题面完整地解释了2x2双矩阵下什么是纳什均衡
（纯策略纳什均衡见“假定两家的决策者”一段，混合策略纳什均衡见“然而多数情况下”一段）
因此即使事先并不知道纳什均衡也可以做。

这题源于合肥比赛题那道博弈题，改成了双方同时决策，且败者费用不会回收的版本。
矩阵博弈虽然一定存在混合策略纳什均衡，但是在题面中解释这个问题太麻烦了。
于是只好把问题简化成了每次2x2的双矩阵博弈，解释起来方便些，正好收益和代价也可以用同一种货币计算了。
（原题中只能最大化收益，不能考虑成本，否则貌似是无解的）

很容易看出这题是倒推DP。根据题目说明，问题存在两种不同情况：纯策略最优解存在和不存在。
注意到，只要任何一方（不管是自己还是对面）存在纯策略最优，那么这一轮的最优选择是唯一确定的。
这一点需要在代码中写清。
除此之外一定是混合策略了，因为双方谁的收益都无法提高，
亦即我方的派遣概率x，应该使对方的期望收益函数G(x,y)相对对方的派遣概率y其导数恒为0；
亦即对方不论派遣还是不派遣，在我方的派遣概率x下，期望收益都不会变化。
解一元一次线性方程就可以得到x的值。

细节很多注意不要写错。
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
using namespace std;

const int MAXN = 103;

double dp[MAXN][MAXN][MAXN];     //dp[场次][A人数][B人数]=A的期望收益
double profit[MAXN], cost[MAXN]; //纯利润，代价

//到第rnd轮，A剩pa人，B剩pb人的时候，A这一轮出人的概率
double Solve(int rnd, int pa, int pb){
  //没人的情况
  if (pa <= 0){
    return 0;
  }
  //对方没人的情况
  if (pb <= 0){
    if (dp[rnd + 1][pa - 1][0] + profit[rnd] > dp[rnd + 1][pa][pb]){
      return 1.0;
    }
    else{
      return 0.0;
    }
  }
  //计算己方的收益（1,0表示是否出人）
  double w11, w10, w01, w00;
  w11 = dp[rnd + 1][pa - 1][pb - 1] + cost[rnd];
  w10 = dp[rnd + 1][pa - 1][pb] + profit[rnd];
  w01 = dp[rnd + 1][pa][pb - 1];
  w00 = dp[rnd + 1][pa][pb];
  //计算对方的收益
  double z11, z10, z01, z00;
  z11 = dp[rnd + 1][pb - 1][pb - 1] + cost[rnd];
  z10 = dp[rnd + 1][pb][pa - 1];
  z01 = dp[rnd + 1][pb - 1][pa] + profit[rnd];
  z00 = dp[rnd + 1][pb][pa];
  //己方没的选的情况（纯策略最优）
  if (w01 >= w11 && w00 >= w10){
    return 0.0;
  }
  if (w11 >= w01 && w10 >= w00){
    return 1.0;
  }
  //对方没的选的情况
  if (z10 >= z11 && z00 >= z01){
    return (w00 >= w10) ? 0.0 : 1.0;
  }
  if (z11 >= z10 && z01 >= z00){
    return (w01 >= w11) ? 0.0 : 1.0;
  }
  //此外的情况，计算均衡点概率并返回
  double p = (z00 - z01) / (z11 + z00 - z01 - z10);
  assert(p >= 1e-6 && p <= (1 - 1e-6));
  return p;
}

//填写dp[rnd][pa][pb]（收益的计算是重复的）
void FillDp(int rnd, int pa, int pb){
  //计算己方的收益（1,0表示是否出人）
  double w11, w10, w01, w00;
  w11 = dp[rnd + 1][pa - 1][pb - 1] + cost[rnd];
  w10 = dp[rnd + 1][pa - 1][pb] + profit[rnd];
  w01 = dp[rnd + 1][pa][pb - 1];
  w00 = dp[rnd + 1][pa][pb];
  //计算双方的行动概率
  double qa, qb;
  qa = Solve(rnd, pa, pb);
  qb = Solve(rnd, pb, pa);
  //填写Dp值
  dp[rnd][pa][pb] = qa * qb * w11 + qa * (1 - qb) * w10
    + (1 - qa) * qb * w01 + (1 - qa) * (1 - qb) * w00;
}

bool domain(){
  int A, B, N, K;
  int i, j, k;
  int v, c;

  if (scanf("%d%d%d", &N, &A, &B) == EOF){
    return false;
  }
  for (i = 1; i <= N; i++){
    scanf("%d%d", &v, &c);
    cost[i] = -c;
    profit[i] = v - c;
  }

  memset(dp, 0, sizeof(dp));
  //这里注意DP的人数上限是双方人数的最大值，不能只考虑自己
  K = max(A, B);
  for (i = N; i >= 1; i--){
    for (j = 0; j <= K; j++){
      for (k = 0; k <= K; k++){
        FillDp(i, j, k);
      }
    }
  }

  printf("%.2lf%%\n", Solve(1, A, B) * 100);
  return true;
}

int main(){
  while (domain());
  return 0;
}
