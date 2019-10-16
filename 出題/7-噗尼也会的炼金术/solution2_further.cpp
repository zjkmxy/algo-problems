/*
已经读完前2个程序了吗？

如果你对这个问题还保持着兴趣，我们来进一步讨论一下时间效率吧。
首先我们把数据扩大，假设每个素材属于100类，每个道具炼成需要100种素材，每种至多100个。
标程1显然无法在有生之年给出答案。

对标程2来说，我们当然需要把最大流算法加上。因为素材的种类数只有233种，
所以素材一边的节点数不是100*100，而是233。因而复杂度不会超过(335)^3。实际常数很小。
对每次查询，最坏结果是排序+枚举*网络流判定=15000*log(15000)+15000*335^3。
大概1个小时就能给出答案了嗯。

另外，实际问题和理论复杂度有时候是不同的：对大部分配方来说，实际能用到的素材往往很少。
针对这一点，也许我们能给出优化。
这里我们给每个配方的每一项素材建立堆，然后把每种素材填入对应的堆里。重复的就重复填入。
然后每次计算当前还能取那几种素材，抽选对应的堆，在他们的最大元素中选出一个全局最大。
也许我们还可以用败者树优化。
这个最大元素显然应该取用。我们将它从堆中弹出。
如果它同时属于多个堆，那么显然它同时是多个堆的顶，一起弹出即可。
这个算法的理论复杂度比原始的标程2高，那么实际有没有可能反而更快呢？
答案是并没有。在HDU上交3个标程发现用时1>>2f>2：
标程1    1388ms
标程2    31ms
标程2改  109ms
但是如果标程2不是每组用例排序一次查询多次，而是每次查询都排序的话，那么要花187ms，比2f高。
时间是个有趣的东西嗯。
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

struct Cmp{
  bool operator()(const int& lhs, const int& rhs) const{
    //质量大的排在堆顶
    return ing[lhs].second < ing[rhs].second;
  }
};
priority_queue<int, vector<int>, Cmp> bakHeap[4]; //备选素材堆
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

//判断在当前的取用状况下，再额外取用一个rec[cata].first类别的素材，是否超标
bool Legal(int cata){
  int residue[4];
  for (int i = 0; i < rcnt; i++)
    residue[i] = rec[i].second;
  residue[cata] --;
  return Legal_DFS(0, residue);
}

Quality Solve(){
  list<int> bakCata;
  int i, j;
  int cnt, now;
  Quality sum;
  list<int>::iterator it;

  //一遍扫描，准备所有的素材
  for (j = 0; j < rcnt; j++){
    while (!bakHeap[j].empty())
      bakHeap[j].pop();
  }
  for (i = 0; i < P; i++){
    for (j = 0; j < rcnt; j++){
      if (ing[i].first->Satisfy(rec[j].first)){
        bakHeap[j].push(i);
      }
    }
  }

  //准备类型
  for (j = 0; j < rcnt; j++)
    bakCata.push_back(j);

  //连续贪心取用
  cnt = now = 0;
  sum = 0;
  for (j = 0; j < rcnt; j++)
    cnt += rec[j].second;
  selected.clear();
  selected.reserve(cnt);
  for (; now < cnt; now++){
    //找到最大值；才4个就没必要用败者树了
    int cur = -1, curQual = -1, tmp;
    curQual = -1;
    for (it = bakCata.begin(); it != bakCata.end(); it++){
      j = *it;
      if (bakHeap[j].empty())
        continue;
      tmp = bakHeap[j].top();
      if (ing[tmp].second >= curQual){
        curQual = ing[tmp].second;
        cur = tmp;
      }
    }

    //如果没的选了，则失败
    if (curQual < 0)
      return -1;

    //进行这一次选择，清理堆
    sum += curQual;
    selected.push_back(cur);
    for (it = bakCata.begin(); it != bakCata.end(); it++){
      j = *it;
      if (ing[cur].first->Satisfy(rec[j].first)){
        bakHeap[j].pop();
      }
    }

    //然后判断还能取那些类别的素材
    for (it = bakCata.begin(); it != bakCata.end();){
      j = *it;
      if (Legal(j))
        it++;
      else
        it = bakCata.erase(it);
    }
  }

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
