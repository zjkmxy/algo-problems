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

inline char getc(){
  static const int BUFLEN = 1 << 15;
  static char B[BUFLEN], *S = B, *T = B;
  if (S == T) S = B, T = S + fread(B, 1, BUFLEN, stdin);
  return (S == T) ? 0 : *(S++);
}
int ReadInt(){
  char ch; int aa = 0;
  do ch = getc(); while (ch && !isdigit(ch));
  if (!ch)
    return 0;
  do aa = aa * 10 + ch - '0', ch = getc();
  while (isdigit(ch));
  return aa;
}

int main(){
  int N, Q, c, x, y, z;
  for (N = ReadInt(), Q = ReadInt(); N > 0; N = ReadInt(), Q = ReadInt()){
    segt.ncnt = 0;
    segt.Build(1, N);

    while (Q--){
      c = ReadInt(), x = ReadInt(), y = ReadInt();
      if (c == 1){
        z = ReadInt();
        if (z == 0)
          continue;
        segt.Plus(segt.node, x, y, z);
      }
      else if (c == 2){
        z = ReadInt();
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