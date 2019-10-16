/*
标程2是出题人原本设想的做法。
还记得贪心选取法有效的几个条件吗？（下文采用所谓的“通俗解释”敬请注意）

首先，一个必要条件是部分满足性。
也就是说，如果任取N个物品没超标，那么从这些物品中扔掉若干个，也是合法的。
有2个典型的例子是最小生成树和最短路（指单点到所有点的最短路树形图）。
最小生成树问题中，只要取的边不形成环就是合法。在选好的一组边中任意删边也不会形成环。
最短路问题中，每一条取得的边，必须从起点在只走取过的边的情况下可达；
因而如果S->b->c->d是合法选取，删去边b->c后，c点不可达，选取反而不合法了。
不满足这个性质的问题显然是不可能贪心的，
因为最优解往往没办法通过“一个一个取”的方式得到。

充分而不必要的条件就很多了。
一个是，先假设取到理论最优解，用贪心的策略改进它。
如果改进后的结果不变，那么贪心策略有效。
算法课上钢哥应该讲过怎么写证明，这里不再赘述。

另一个重要的充分条件是“拟阵”，也就是说在合法的前提下，
不论你怎么选，选到不能再选为止，总会得到相等的个数。
满足拟阵性质的问题一定可以贪心得到最优解。
这里再举2个典型的例子：最小生成树和二分图最大权匹配。
最小生成树中，只要取的边数小于N-1且无环（合法），一定可以在加入一条边，
使得加入新的边后依然无环。因而这个问题是拟阵，Kruskal贪心算法成立。
最大权匹配中，可能最优解是{1->2,3->4,5->6}，但是你取完{1->4,3->6}后，
发现并没有5->2这条边；这时已经无法取更多了，但是依然比最优解少一个。
所以最大权匹配问题不是拟阵，也不能贪心。

回到这个问题，每个配方的贪心是有两种策略的：
第一种是，逐个项地考虑。比如配方需要Ax2+Bx1，就先在满足A的素材中取2个质量最大的，
然后从剩下的素材中取满足B且质量最大的。
这个算法显然不对，公开用例2的第一个配方就是反例。
另一种策略是，把需要的项理解为“上限”，即A不超过2个，B不超过1个。
然后从所有素材中一个个取质量最大的，但是不指定已经取得的素材到底归在A下还是归在B下。
易见这么做符合“拟阵”，可以立即判定贪心结果最优。

于是我们先排序所有素材，每次尝试取一个，然后判断是否合法，不合法就退掉。（和Kruskal一样的）
怎么判断是否合法？其实是个最大流问题。不过具体到本题一共就20个点，用深搜暴力掉就可以。
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <queue>
#include <list>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//括起来的内容和标程1是一样的
const int MAXN = 15005;
typedef int Catagory;
typedef int Recipe;
typedef int Quality;
struct Ingredient{
  int cnt;
  Catagory cata[4];
  static Ingredient dat[301];
  static int N, M;

  bool Satisfy(Recipe req){
    if (req > M)
      return (this - dat) == (req - M);
    else
      return find(cata, cata + cnt, req) < (cata + cnt);
  }

  void Read(){
    scanf("%d", &cnt);
    for (int j = 0; j < cnt; j++)
      scanf("%d", cata + j);
  }

  static void GetDatabase(){
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++)
      dat[i].Read();
  }
};
Ingredient Ingredient::dat[301];
int Ingredient::N = 0;
int Ingredient::M = 0;

pair<Ingredient*, Quality> ing[MAXN];
int P;
pair<Recipe, int> rec[4];
int rcnt;
//////////////////////////////////////////////////////////////////////////

bool Cmp(const int& lhs, const int& rhs){
  //质量大的排在前
  return ing[lhs].second > ing[rhs].second;
}
int bakQueue[MAXN];   //可选素材的后备队列
vector<int> selected; //已选素材

//Legal的子过程，通过DFS和回溯完成判断
bool Legal_DFS(int i, int residue[]){
  if (i == selected.size())
    return true;
  Ingredient* cur = ing[selected[i]].first;
  for (int j = 0; j < rcnt; j++){
    if (cur->Satisfy(rec[j].first) && residue[j] > 0){
      residue[j] --;
      if (Legal_DFS(i + 1, residue))
        return true;
      residue[j] ++;
    }
  }
  return false;
}

//判断在当前的取用状况是否超标
bool Legal(){
  int residue[4];
  for (int i = 0; i < rcnt; i++)
    residue[i] = rec[i].second;
  return Legal_DFS(0, residue);
}

Quality Solve(){
  int i, j;
  int cnt, now;
  Quality sum;

  //连续贪心取用
  cnt = now = 0;
  sum = 0;
  for (j = 0; j < rcnt; j++)
    cnt += rec[j].second;
  selected.clear();
  selected.reserve(cnt);
  for (i = 0; i < P && now < cnt; i++){
    int cur = bakQueue[i];
    selected.push_back(cur);
    if (!Legal()){
      selected.pop_back();
      continue;
    }

    now++;
    sum += ing[cur].second;
  }

  if (now < cnt)
    return -1;
  else
    return sum / cnt;
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
  //一遍扫描，准备所有的素材
  for (i = 0; i < P; i++)
    bakQueue[i] = i;
  sort(bakQueue, bakQueue + P, Cmp);

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
