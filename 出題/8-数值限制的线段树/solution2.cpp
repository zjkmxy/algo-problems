/*
这道题情况和HDU5306是不同的，这个思路在HDU5306会TLE

HDU5306如果不去记录子树tag最大值，直接暴力是会被卡到N^2的,数据如下：
0 N 0 N 0 N 0 N ...
然后每次从N依次递减对[1,N]区间做min操作

但是这道题一共只有100，在上述数据暴力100次之后所有数字都一样了
所以上题最坏O(N^2)的暴力算法，这道题就变成了O(N(log(N)+RANGE))
RANGE=100于是可以过的。
何况本来随机数据下的平均复杂度也没到O(N^2)
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

typedef long long ll;
const int MAXN = 100005;
const int DUMMY = -1;
const int FULL = 100;
struct Segtree{
  struct NODE{
    int L, R;
    NODE *l, *r;

    int sum, add;
    int tag;      //覆写标记
    int mn, mx;

    void popup(){
      mn = min(l->mn, r->mn);
      mx = max(l->mx, r->mx);
      sum = l->sum + r->sum;
      tag = DUMMY;
      if (l->tag == r->tag && l->tag != DUMMY){
        tag = l->tag;
      }
    }

    //用val填充整个区间
    void Fill(int val){
      tag = val;
      add = 0;
      sum = val * (R - L + 1);
      mx = mn = val;
    }

    //预处理超越限制的值
    //如果是正超越就改成FULL - val
    //如果是负超越就改成- val
    //然后通过Popup修正答案
    void Fix(int val){
      if (mx + val < FULL && mn + val > 0)
        return;
      if (tag == DUMMY){
        pushdown();
        l->Fix(val);
        r->Fix(val);
        popup();
      }
      else
        Fill(max(-val, min(FULL - val, tag)));
    }

    //区间加上一个数val
    void Plus(int val){
      Fix(val);
      //整个区间都一样则修改tag，否则修改add
      if (tag == DUMMY)
        add += val;
      else
        tag += val;
      sum += val * (R - L + 1);
      mx += val;
      mn += val;
    }

    //下传标记
    void pushdown(){
      if (tag != DUMMY){
        //区间覆盖存在时，add一定是0
        l->Fill(tag);
        r->Fill(tag);
      }
      else if (add != 0){
        //否则执行惰性加法运算
        //注意反复交替负溢出、正溢出的情况，是需要多次Fix的
        //不然可能会出错
        //一个错误的例子是：把所有负溢出的数修正到0然后不管它们
        //那样的话，比如当前节点先-FULL，然后加24，子节点正确答案是+24
        //但是当前节点的add是24-FULL<0，如果子节点被修正到0，那么将无法获得+24
        //因而子节点必须在-FULL一步被修正到FULL
        l->Plus(add);
        r->Plus(add);
        add = 0;
      }
    }
  }node[MAXN << 1];
  int ncnt;

  NODE* Build(int l, int r){
    NODE* ret = &node[ncnt++];
    ret->L = l;
    ret->R = r;
    if (l == r){
      ret->l = ret->r = nullptr;
    }
    else{
      int mid = (l + r) >> 1;
      ret->l = Build(l, mid);
      ret->r = Build(mid + 1, r);
    }
    ret->add = ret->mn = ret->mx = ret->sum = 0;
    ret->tag = 0;
    return ret;
  }

  void Plus(NODE* rt, int l, int r, int val){
    if (l <= rt->L && rt->R <= r){
      rt->Plus(val);
    }
    else{
      rt->pushdown();
      if (l <= rt->l->R)
        Plus(rt->l, l, min(rt->l->R, r), val);
      if (r >= rt->r->L)
        Plus(rt->r, max(rt->r->L, l), r, val);
      rt->popup();
    }
  }

  int GetSum(NODE* rt, int l, int r){
    if (l <= rt->L && rt->R <= r){
      return rt->sum;
    }
    else{
      int ret = 0;
      rt->pushdown();
      if (l <= rt->l->R)
        ret += GetSum(rt->l, l, min(rt->l->R, r));
      if (r >= rt->r->L)
        ret += GetSum(rt->r, max(rt->r->L, l), r);
      return ret;
    }
  }
}segt;

int main(){
  int N, Q, c, x, y, z;
  while (scanf("%d%d", &N, &Q) != EOF){
    segt.ncnt = 0;
    segt.Build(1, N);

    while (Q--){
      scanf("%d%d%d", &c, &x, &y);
      if (c == 1){
        scanf("%d", &z);
        if (z == 0)
          continue;
        segt.Plus(segt.node, x, y, z);
      }
      else if (c == 2){
        scanf("%d", &z);
        if (z == 0)
          continue;
        segt.Plus(segt.node, x, y, -z);
      }
      else{
        printf("%d\n", segt.GetSum(segt.node, x, y));
      }
    }
  }
  return 0;
}
