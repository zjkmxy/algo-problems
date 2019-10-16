/*
为响应B站诸位大神“少一点套路，多一点真诚”的号召，我一向是出原创精品题的。
当然这道也不例外。
本题数据库就是A17数据库的一部分，表格是从苏菲贴吧下的巴哈姆特的人整理的。
而标程1的DP算法由14级的姜老板所提出。
向他们表示感谢。

每个查询肯定是分开处理的。因为配方要求素材数固定，所以用到的素材质量和最大就可以。
考虑到每个素材可能属于多个类别，亦即可能同时满足配方中的多个种类需求，
这里我们需要明确“每个素材到底作为哪一项原料被使用”。
令dp[i][x][y][z][w]表示：
已经处理了前i个素材，还剩x个1类原料，y个2类，z个3类，w个4类的情况下，所能取到的最大质量和。
DP方程显而易见。
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
using namespace std;

const int MAXN = 15005;
typedef int Catagory; //素材大类
typedef int Recipe;   //配方需求
typedef int Quality;  //素材质量
struct Ingredient{
  int cnt;            //该素材属于几个类别
  Catagory cata[4];   //具体是哪几个
  static Ingredient dat[301]; //各种各样的素材（全233个）
  static int N, M;

  //本素材能否满足配方中req类的需求
  bool Satisfy(Recipe req){
    if (req > M)
      return (this - dat) == (req - M);
    else
      return find(cata, cata + cnt, req) < (cata + cnt);
  }

  //读入一个素材描述
  void Read(){
    scanf("%d", &cnt);
    for (int j = 0; j < cnt; j++)
      scanf("%d", cata + j);
  }

  //读入共用数据库
  static void GetDatabase(){
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++)
      dat[i].Read();
  }
};
Ingredient Ingredient::dat[301];
int Ingredient::N = 0;
int Ingredient::M = 0;

pair<Ingredient*, Quality> ing[MAXN]; //本组用例的所有素材（类别，质量）
int P;
pair<Recipe, int> rec[4];   //本次查询配方的原料要求（类别，个数）
int rcnt;

Quality Solve(){
  static Quality dp[2][6][6][6][6];
  int i, x, y, z, w;
  int rd = 0, wt = 1;

  for (i = rcnt; i < 4; i++)
    rec[i] = make_pair(0, 0);

  memset(dp, -1, sizeof(dp));
  dp[rd][rec[0].second][rec[1].second][rec[2].second][rec[3].second] = 0;
  for (i = 0; i < P; i++, rd ^= 1, wt ^= 1){
    for (x = 0; x <= rec[0].second; x++){
      for (y = 0; y <= rec[1].second; y++){
        for (z = 0; z <= rec[2].second; z++){
          for (w = 0; w <= rec[3].second; w++){
            if (dp[rd][x][y][z][w] < 0)
              continue;
            //1
            if (x > 0 && ing[i].first->Satisfy(rec[0].first)){
              dp[wt][x - 1][y][z][w] = max(dp[wt][x - 1][y][z][w], dp[rd][x][y][z][w] + ing[i].second);
            }
            //2
            if (y > 0 && ing[i].first->Satisfy(rec[1].first)){
              dp[wt][x][y - 1][z][w] = max(dp[wt][x][y - 1][z][w], dp[rd][x][y][z][w] + ing[i].second);
            }
            //3
            if (z > 0 && ing[i].first->Satisfy(rec[2].first)){
              dp[wt][x][y][z - 1][w] = max(dp[wt][x][y][z - 1][w], dp[rd][x][y][z][w] + ing[i].second);
            }
            //4
            if (w > 0 && ing[i].first->Satisfy(rec[3].first)){
              dp[wt][x][y][z][w - 1] = max(dp[wt][x][y][z][w - 1], dp[rd][x][y][z][w] + ing[i].second);
            }
            //不用
            dp[wt][x][y][z][w] = max(dp[wt][x][y][z][w], dp[rd][x][y][z][w]);
          }//w
        }//z
      }//y
    }//x
  }//i

  if (dp[rd][0][0][0][0] == -1)
    return -1;
  else
    return dp[rd][0][0][0][0] / (rec[0].second + rec[1].second + rec[2].second + rec[3].second);
}

bool domain(){
  int i, idx, Q;
  Quality q;

  if (scanf("%d%d", &P, &Q) == EOF)
    return false;

  for (i = 0; i < P; i++){
    scanf("%d%d", &idx, &q);
    ing[i] = make_pair(Ingredient::dat + idx, q);
  }
  while (Q--){
    scanf("%d", &rcnt);
    for (i = 0; i < rcnt; i++){
      scanf("%d%d", &idx, &q);
      rec[i] = make_pair(idx, q);
    }

    q = Solve();
    printf("%d\n", q);
  }

  return true;
}

int main(){
  Ingredient::GetDatabase();
  while (domain());
  return 0;
}
